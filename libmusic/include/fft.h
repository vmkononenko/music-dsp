/**
 * @file        fft.h
 * @brief       Fast Fourier Transform API
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmpriority_queue.h"
#include "lmtypes.h"

#ifndef FFT_TEST_FRIENDS
#define FFT_TEST_FRIENDS
#endif


class FFT {

FFT_TEST_FRIENDS;

private:
    uint32_t __bitReverse(uint32_t, uint8_t);
    void __sortBitReversal(std::vector<complex_t> &, uint32_t, uint32_t);
    void __forward(std::vector<complex_t> &, uint32_t, uint32_t);
    void __forward(std::vector<complex_t> &);
    void __exch(std::vector<complex_t> &, uint32_t, uint32_t);
    void __butterfly(std::vector<complex_t> &input, uint32_t, uint32_t);

public:
    void forward(std::vector<complex_t> &);

    /**
     * Converts frequency domain from rectangular to polar notation
     *
     * @param input frequency domain in rectangular notation
     */
    void toPolar(std::vector<complex_t> &);

    uint32_t toPolar(std::vector<complex_t> &input,
                     amplitude_t *freqDomainMagnitudes, uint32_t reqLen);

    /**
     * Converts frequency domain from rectangular to polar notation
     *
     * If non-NULL freqDomainMagnitudes is passed as a parameter instead of
     * doing in-place conversion to polar input is not touched. Corresponding
     * reqLen polar magnitudes are written to freqDomainMagnitudes and phases
     * are omitted.
     *
     * @param input frequency domain in rectangular notation
     * @param freqDomainMagnitudes
     * @param pq        if non-NULL write results in a form of priority queue
     * @param reqLen    convert only first reqLen points
     * @return number of points converted
     */
    uint32_t toPolar(std::vector<complex_t> &input, amplitude_t *freqDomainMagnitudes,
                     PriorityQueue *pq, uint32_t reqLen);


    /**
     * Convert FFT to harmonic product spectrum
     *
     * @param freqDomainMagnitudes  frequency domain
     * @param len                   length of the \p freqDomainMagnitudes
     */
    void toHPS(amplitude_t *freqDomainMagnitudes, uint32_t len);

    /**
     * Reduce noise in FFT results by applying moving average
     *
     * @param freqDomainMagnitudes  frequency domain including padding of
     *                              \p window - 1
     * @param len                   length of the \p freqDomainMagnitudes
     *                              excluding padding
     * @param window                moving average window size
     */
    void avg(amplitude_t *freqDomainMagnitudes, uint32_t len, uint32_t window);

    /**
     * Inverse DFT calculation
     *
     * @param input frequency domain in rectangular notation
     */
    void inverse(std::vector<complex_t> &);
};

/** @} */
