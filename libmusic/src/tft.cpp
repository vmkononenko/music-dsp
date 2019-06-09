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
 * fft.cpp
 *
 *  TFT class implementation
 */

#include <algorithm>

#include "lmhelpers.h"
#include "tft.h"

namespace anatomist {

TFT::TFT(freq_hz_t f_min, freq_hz_t f_max, uint16_t bpo, uint32_t sample_rate,
         uint16_t win_size, uint16_t hop_size) :
                                            f_min_(f_min),
                                            f_max_(f_max),
                                            bpo_(bpo),
                                            sample_rate_(sample_rate),
                                            win_size_(win_size),
                                            hop_size_(hop_size)
{
    spectrogram_ = log_spectrogram_t(0, fd_t(0));
    interval_ = win_size;
}

log_spectrogram_t TFT::GetSpectrogram()
{
    return spectrogram_;
}

uint32_t TFT::SpectrogramInterval()
{
    return interval_;
}

void TFT::Denoise_(log_spectrogram_t &block)
{
    for (auto & col : block) {
        std::vector<amplitude_t> devs;
        amplitude_t thr_uni, sigma, mad;
        amplitude_t median = Helpers::median(col);

        for (auto val : col) {
            devs.push_back(std::abs(val - median));
        }

        mad = Helpers::median(devs);

        sigma = mad / 0.6745;

        thr_uni = sigma * sqrt(2 * log10(interval_));

        for (auto & bin : col) {
            if (std::abs(bin) < thr_uni) {
                bin = 0;
            }
        }
    }
}

}
