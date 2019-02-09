/*
 * fft_wrapper.cpp
 *
 *  FFTWrapper class implementation
 */

#include "fft.h"
#include "fft_wrapper.h"
#include "lmhelpers.h"
#include "window_functions.h"


using namespace std;

namespace anatomist {

FFTWrapper::FFTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint16_t bpo,
                       uint32_t sample_rate, uint16_t win_size, uint16_t hop_size) :
            TFT(f_low, f_high, bpo, sample_rate, win_size, hop_size) {}

FFTWrapper::FFTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
                       uint16_t win_size, uint16_t hop_size) :
            FFTWrapper(f_low, f_high, BINS_PER_OCTAVE_DEFAULT, sample_rate,
                       win_size, hop_size) {}

void FFTWrapper::Process(td_t td, uint32_t offset)
{
    for (uint32_t sample_idx = offset; sample_idx < td.size(); sample_idx += hop_size_) {
        uint32_t len = min(win_size_, static_cast<uint16_t>(td.size() - sample_idx));
        td_t td_win(td.begin() + sample_idx, td.begin() + sample_idx + len);
        FFT *fft;

        WindowFunctions::applyDefault(td_win);

        fft = new FFT(td_win, sample_rate_, f_min_, f_max_);

        spectrogram_.push_back(FFTPruned(fft));

        delete fft;
    }
}

fd_t FFTWrapper::FFTPruned(FFT *fft)
{
    UNUSED(fft);
    fd_t fd;

    return fd;
}

uint8_t FFTWrapper::BinsPerSemitone()
{
    return 0;
}

uint32_t FFTWrapper::FreqToBin(freq_hz_t f)
{
    UNUSED(f);
    return 0;
}

freq_hz_t FFTWrapper::BinToFreq(uint32_t idx)
{
    UNUSED(idx);
    return 0;
}

}
