/**
 * @file        cheby1_filter.h
 * @brief       Defines API for Chebyshev type 1 filter
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
class Cheby1Filter : public RecursiveFilter {

public:
    Cheby1Filter() {
        mB = {

            2.051277523274928e-10, 8.205110093099713e-10, 1.230766513964957e-09,
            8.205110093099713e-10, 2.051277523274928e-10
        };

        mA = {
            1, -3.98634597829217, 5.959203624346703,
            -3.959368305507797, 0.986510662801611
        };

        mPoles = mA.size() - 1;
    };
};
