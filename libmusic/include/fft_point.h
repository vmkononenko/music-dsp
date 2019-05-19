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
 * @file        fft_point.h
 * @brief       FftPoint class definition
 * @addtogroup  libmusic
 * @{
 */

#include "lmtypes.h"


class FftPoint {

private:
    FftPoint();

public:
    uint32_t      sampleNumber;   /** index in FFT frequency domain array */
    amplitude_t   magnitude;      /** magnitude value in polar notation */

    /**
     * Constructor
     *
     * @param   sn          sample number, corresponding index in FFT frequency
     *                      domain array
     * @param   magnitude   magnitude value in polar notation
     */
    FftPoint(uint32_t sn, amplitude_t m) : sampleNumber(sn), magnitude(m) {}

    friend bool operator<(const FftPoint& point1, const FftPoint& point2)
    {
        return (point1.magnitude < point2.magnitude);
    }
};

/** @} */
