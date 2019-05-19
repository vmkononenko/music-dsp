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
 * @file        lmhelpers.h
 * @brief       Header file for helpers
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <stdint.h>
#include <vector>

#include "lmtypes.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

// Introduced for cross-compilation for Android because log2 is
// not defined for all target platforms.
/** @TODO find a proper fix */
#define log2(x) (log(x) / log(2))

#ifndef HELPERS_TEST_FRIENDS
#define HELPERS_TEST_FRIENDS
#endif

/**
 * @ingroup libmusic
 */
class Helpers {

HELPERS_TEST_FRIENDS;

public:
    /**
     * Calculate first power of two number which is greater than n
     *
     * @param   n
     * @return  corresponding power of two value
     */
    static uint32_t nextPowerOf2(uint32_t n);

    /**
     * Check if number is a power of 2
     *
     * @param   n   number to check
     * @return true if power of 2, false otherwise
     */
    static bool isPowerOf2(uint32_t n);

    static std::vector<complex_t> timeDomain2ComplexVector(amplitude_t *, uint32_t, uint32_t);

    /**
     * Round number with requested precision
     *
     * @param   n       number to round up
     * @param   scale   numeric scale - number of digits after decimal point
     * @return  rounded number with scale
     */
    template<typename T> static T stdRound(T n, uint8_t scale)
    {
        if (scale == 0) {
            n = round(n);
        } else {
            n *= (pow(10, scale));
            n = round(n);
            n /= (pow(10, scale));
        }

        return n;
    }

    template<typename T1, typename T2>
    static bool cmpPairBySecond(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2)
    {
       return p1.second < p2.second;
    }

    static bool almostEqual(double a, double b, double eps);

    /**
     * Return median of the vector
     */
    template<typename T>
    static T median(const std::vector<T> v)
    {
        if (v.size() == 0) {
            throw std::invalid_argument("median(): empty vector");
        } else if (v.size() == 1) {
            return v[0];
        }

        std::vector<T> v_s = std::vector<T>(v);
        uint32_t mid_idx = v_s.size() / 2;
        T median;

        sort(v_s.begin(), v_s.end());

        if (v_s.size() % 2) {
            median = v_s[mid_idx];
        } else {
            median = (v_s[mid_idx] + v_s[mid_idx - 1]) / 2;
        }

        return median;
    }
};

/** @} */
