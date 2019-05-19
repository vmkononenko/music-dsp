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
 * @file        butterworth_filter.h
 * @brief       Defines API for the Butterworth filter
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "recursive_filter.h"

/**
 * 4 pole low pass filter coefficients
 * Calculated with Octave for cutoff frequency 0.0027211 (240 Hz for 22050)
 */
class ButterworthFilter : public RecursiveFilter {

public:
    ButterworthFilter() {
        mB = {
            3.300729733127236e-10, 1.320291893250894e-09, 1.980437839876342e-09,
            1.320291893250894e-09, 3.300729733127236e-10
        };

        mA = {
            1, -3.977661591581739, 5.933233938906785,
            -3.933481478165625, 0.9779091361217471
        };

        mPoles = mA.size() - 1;
    };
};
