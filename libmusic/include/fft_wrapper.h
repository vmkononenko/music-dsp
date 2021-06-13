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

/**
 * @file        fft_wrapper.h
 * @brief       Wrapper class for FFT implementing TFT interface
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include "pitch_calculator.h"
#include "tft.h"

namespace anatomist {

class FFTWrapper : public TFT {

private:
    PitchCalculator     &pc_ = PitchCalculator::getInstance();

    /**
     * Performs logarithmic pruning of FFT frequencies
     */
    fd_t FFTPruned(FFT *fft);

public:
    FFTWrapper(freq_hz_t f_min, freq_hz_t f_max, uint16_t bpo, uint32_t sample_rate,
               uint16_t win_size, uint16_t hop_size);

    FFTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
               uint16_t win_size, uint16_t hop_size);

    ~FFTWrapper() {}

    void Process(const td_t & td, uint32_t offset) override;

    uint8_t BinsPerSemitone() override;

    uint32_t FreqToBin(freq_hz_t f) override;

    freq_hz_t BinToFreq(uint32_t idx) override;
};

}

/** @} */
