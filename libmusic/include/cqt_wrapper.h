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
 * @file        cqt_wrapper.h
 * @brief       Wrapper class for ext/cqtt constant Q implementation
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include "CQBase.h"
#include "CQSpectrogram.h"

#include "lmtypes.h"
#include "tft.h"

namespace anatomist {

typedef class CQTWrapper : public TFT {

private:
    CQSpectrogram   *cq_spectrogram_;

    log_spectrogram_t ConvertRealBlock_(CQBase::RealBlock &block);

public:

    CQTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint16_t bpo,
               uint32_t sample_rate, uint16_t win_size, uint16_t hop_size);

    CQTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
               uint16_t win_size, uint16_t hop_size);

    ~CQTWrapper();

    void Process(const td_t & td, uint32_t offset) override;

    uint8_t BinsPerSemitone() override;

    uint32_t FreqToBin(freq_hz_t f) override;

    freq_hz_t BinToFreq(uint32_t idx) override;

} log_spectrum_t;

}

/** @} */
