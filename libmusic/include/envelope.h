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
 * @file        envelope.h
 * @brief       API for envelope detection
 *
 * Extracts and keeps envelope of the input signal. Is based on squaring
 * and low pass filtering method:
 * http://www.mathworks.com/help/dsp/examples/envelope-detection.html
 *
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"


class Envelope {

private:
    Envelope();

    std::vector<amplitude_t> __mEnvelope;
    uint16_t __mDF = 1;     // total downsampling factor caused by filtering
    amplitude_t __mMaxAmplitude;

    void squareAndDouble(amplitude_t *td, uint32_t samples);

    void squareRoot(std::vector<amplitude_t> &input);

    amplitude_t max();

    amplitude_t mean(uint32_t startIdx, uint32_t endIdx);

public:
    Envelope(const amplitude_t *td, uint32_t samples);

    std::vector<amplitude_t> diff();

    uint16_t getDownsampleFactor();

    bool isSilence(uint32_t startIdx, uint32_t endIdx);

    friend std::ostream& operator<<(std::ostream& os, const Envelope& e);

    ~Envelope();
};
