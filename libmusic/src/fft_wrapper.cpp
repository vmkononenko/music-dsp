/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * fft_wrapper.cpp
 *
 *  FFTWrapper class implementation
 */

#include "fft.h"
#include "fft_wrapper.h"
#include "lmhelpers.h"
#include "pitch_calculator.h"
#include "window_functions.h"


using namespace std;

namespace anatomist {

FFTWrapper::FFTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint16_t bpo,
                       uint32_t sample_rate, uint16_t win_size, uint16_t hop_size) :
            TFT(f_low, f_high, bpo, sample_rate, win_size, hop_size)
{
    f_min_ = pc_.getPitch(f_low);
}

FFTWrapper::FFTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
                       uint16_t win_size, uint16_t hop_size) :
            FFTWrapper(f_low, f_high, BINS_PER_OCTAVE_DEFAULT, sample_rate,
                       win_size, hop_size) {}

void FFTWrapper::Process(td_t td, uint32_t offset)
{
    for (uint32_t sample_idx = offset; sample_idx < td.size(); sample_idx += hop_size_) {
        size_t len = min(static_cast<size_t>(win_size_), td.size() - sample_idx);
        td_t td_win(td.begin() + sample_idx, td.begin() + sample_idx + len);
        FFT *fft;

        WindowFunctions::applyDefault(td_win);

        fft = new FFT(td_win, sample_rate_, f_min_, f_max_);

        spectrogram_.push_back(FFTPruned(fft));

        delete fft;
    }

    Denoise_(spectrogram_);
}

fd_t FFTWrapper::FFTPruned(FFT *fft)
{
    uint32_t fft_bin;
    freq_hz_t f = f_min_;
    fd_t fd;
    uint32_t i = 0;

    while (f < f_max_) {
        fft_bin = fft->FreqToIdx(f, round);
        fd.push_back(fft->GetFreqDomain().p[fft_bin]);
        i++;
        if (i % (bpo_ / notes_Total)) {
            f =  pc_.getFreqByInterval(f, 1.0 / bpo_);
        } else {
            f = pc_.getPitch(f);
        }
    }

    return fd;
}

uint8_t FFTWrapper::BinsPerSemitone()
{
    return bpo_ / notes_Total;
}

uint32_t FFTWrapper::FreqToBin(freq_hz_t f)
{
    if ((f < f_min_) || (f > f_max_))
    {
        throw invalid_argument("FFTWrapper::FreqToBin(): f is out of range");
    }

    return round(static_cast<double>(log2(f / f_min_) * bpo_));
}

freq_hz_t FFTWrapper::BinToFreq(uint32_t idx)
{
    return f_min_ * pow(2, (1.0 * idx / bpo_));
}

}
