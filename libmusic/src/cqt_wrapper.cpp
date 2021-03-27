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

#include <algorithm>
#include <cmath>

#include "CQParameters.h"

#include "cqt_wrapper.h"
#include "lmhelpers.h"

namespace anatomist {

using namespace std;

CQTWrapper::CQTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint16_t bpo,
                       uint32_t sample_rate, uint16_t win_size, uint16_t hop_size) :
            TFT(f_low, f_high, bpo, sample_rate, win_size, hop_size)
{
    CQParameters p(sample_rate, f_low, f_high, bpo);

    /*
     * From the paper:
     * values q < 1 can be seen to implement oversampling of the frequency axis,
     * analogously to the use of zero padding when calculating the DFT.
     * For example q = 0.5 corresponds to oversampling factor of 2.
     */
    p.q = 0.5;

    cq_spectrogram_ = new CQSpectrogram(p, CQSpectrogram::InterpolateLinear);

    if (!cq_spectrogram_->isValid()) {
        throw new runtime_error("Failed to construct a Q-Transform");
    }

    f_min_ = cq_spectrogram_->getMinFrequency();
    f_max_ = cq_spectrogram_->getMaxFrequency();
    interval_ = round(1.0 * win_size / cq_spectrogram_->getColumnHop())
                * cq_spectrogram_->getColumnHop();

}

CQTWrapper::CQTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
                       uint16_t win_size, uint16_t hop_size) :
            CQTWrapper(f_low, f_high, BINS_PER_OCTAVE_DEFAULT, sample_rate,
                       win_size, hop_size) {}

CQTWrapper::~CQTWrapper()
{
    delete cq_spectrogram_;
}

void CQTWrapper::Process(td_t td, uint32_t offset)
{
    CQBase::RealBlock output_block, output;

    if (hop_size_ > 1) {
        for (uint32_t sample = offset; sample < td.size(); sample += hop_size_) {
            uint32_t len = min(win_size_, static_cast<uint16_t>(td.size() - sample));
            CQBase::RealSequence input(td.begin() + sample, td.begin() + sample + len);
            output_block = cq_spectrogram_->process(input);

            if (!output_block.empty()) {
                output.insert(output.end(), output_block.begin(), output_block.end());
                output_block.clear();
            }
        }
    } else {
        output_block = cq_spectrogram_->process(td);
        output.insert(output.end(), output_block.begin(), output_block.end());
    }

    output_block = cq_spectrogram_->getRemainingOutput();
    output.insert(output.end(), output_block.begin(), output_block.end());
    LM_PEEP(CQTProcess_OUTPUT, output);
    output.erase(output.begin(), output.begin() + cq_spectrogram_->getLatency() /
                                                  cq_spectrogram_->getColumnHop());

    for (auto &col : output) {
        reverse(col.begin(), col.end());
    }

    spectrogram_ = ConvertRealBlock_(output);
}

log_spectrogram_t CQTWrapper::ConvertRealBlock_(CQBase::RealBlock &block)
{
    if (block.empty()) {
        throw invalid_argument("Empty input block");
    }

    log_spectrogram_t lsg;
    uint32_t cols_per_window = interval_ / cq_spectrogram_->getColumnHop();

    if (cols_per_window <= 1) {
        return block;
    }

    for (uint32_t i = 0; i < block.size(); i += cols_per_window) {
        uint32_t columns = min(cols_per_window, static_cast<uint32_t>(block.size()) - i);
        vector<amplitude_t> col;

        for (uint32_t j = 0; j < block[i].size(); j++) {
            amplitude_t row_value = 0;
            for (uint32_t c = 0; c < columns; c++) {
                row_value += block[i + c][j];
            }
            col.push_back(row_value);
        }

        lsg.push_back(col);
    }
    LM_PEEP(CQT_lsg, lsg);
    Denoise_(lsg);
    LM_PEEP(CQT_lsg_denoised, lsg);
    return lsg;
}

uint8_t CQTWrapper::BinsPerSemitone()
{
    return cq_spectrogram_->getBinsPerOctave() / notes_Total;
}

uint32_t CQTWrapper::FreqToBin(freq_hz_t f)
{
    if ((f < f_min_) || (f > f_max_))
    {
        throw invalid_argument("CQTWrapper::FreqToBin(): f is out of range");
    }

    return round(static_cast<double>(log2(f / f_min_) * cq_spectrogram_->getBinsPerOctave()));
 }

freq_hz_t CQTWrapper::BinToFreq(uint32_t idx)
{
    return cq_spectrogram_->getBinFrequency(cq_spectrogram_->getTotalBins() - idx - 1);
}

}
