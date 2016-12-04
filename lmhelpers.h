/*
 * lmhelpers.h
 *
 * Header file for helpers
 */

#pragma once

#include <stdint.h>
#include <vector>

#include "lmtypes.h"

#ifndef HELPERS_TEST_FRIENDS
#define HELPERS_TEST_FRIENDS
#endif

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
     * Return string representation of the given note
     *
     * @param   note    note for which string representation is requested
     * @return string representation of the note
     */
    static std::string noteToString(note_t note);

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
};
