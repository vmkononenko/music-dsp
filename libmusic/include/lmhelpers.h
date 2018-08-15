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

    /**
     * Convert frequency in Hz to point index value in FFT array
     *
     * @param   freq        frequency in Hz
     * @param   fftSize     FFT size
     * @param   sampleRate  sample rate
     * @param   roundFunc   pointer to function which will perform rounding
     * @return  index value
     */
    static uint32_t freqToFftIdx(freq_hz_t freq, uint32_t fftSize,
                                 uint32_t sampleRate, double (*roundFunc)(double));

    /**
     * Calculate frequency in Hz from point index value in FFT array
     *
     * @param   idx         index value in FFT array
     * @param   fftSize     FFT size
     * @param   sampleRate  sample rate
     * @return  frequency in Hz
     */
    static freq_hz_t fftIdxToFreq(uint32_t idx, uint32_t fftSize, uint32_t sampleRate);

    template<typename T1, typename T2>
    static bool cmpPairBySecond(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2)
    {
       return p1.second < p2.second;
    }
};

/** @} */
