/*
 * lmhelpers.h
 *
 * Header file for helpers
 */

#pragma once

#include <stdint.h>
#include <vector>

#include "lmtypes.h"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif

class Helpers {

TEST_FRIENDS;

public:
    static uint32_t nextPowerOf2(uint32_t);
    static std::vector<complex_t> timeDomain2ComplexVector(amplitude_t *, uint32_t);

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
};
