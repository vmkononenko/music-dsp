/**
 * @file        fft.h
 * @brief       Fast Fourier Transform API
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmpriority_queue.h"
#include "transform.h"

#ifndef FFT_TEST_FRIENDS
#define FFT_TEST_FRIENDS
#endif


class FFT : public Transform {

FFT_TEST_FRIENDS;

private:
    /**
     * Default constructor
     *
     * Keeping it so tests are able to create dummy instances
     */
    FFT();

    uint32_t BitReverse_(uint32_t, uint8_t);
    void SortBitReversal_(std::vector<complex_t> &, uint32_t, uint32_t);
    void Forward_(std::vector<complex_t> &, uint32_t, uint32_t);
    void Forward_(std::vector<complex_t> &);
    void Exch_(std::vector<complex_t> &, uint32_t, uint32_t);
    void Butterfly_(std::vector<complex_t> &input, uint32_t, uint32_t);

    /**
     * In-place conversion of a frequency domain from rectangular to polar notation
     *
     * If non-NULL \p fd_magnitudes is passed as a parameter instead of
     * doing in-place conversion to polar input fd is not touched. Corresponding
     * \p req_len polar magnitudes are written to freqDomainMagnitudes and
     * phases are omitted.
     *
     * @param fd frequency  domain in rectangular notation
     * @param fd_magnitudes
     * @param pq            if non-NULL write results in a form of priority queue
     * @param req_len       convert only first reqLen points
     * @param start_idx     ignore first \p start_idx elements during conversion.
     *                      This is an optimization for cutting off lower frequencies.
     *                      Instead of calculating values and then setting them
     *                      to zero we just don't calculate them.
     * @return number of points converted
     */
    uint32_t ToPolar_(std::vector<complex_t> &fd, amplitude_t *fd_magnitudes,
                     PriorityQueue *pq, uint32_t req_len, uint32_t start_idx);

    /**
     * Same as ToPolar(fd, nullptr, nullptr, fd.size(), 0)
     */
    void ToPolar_(std::vector<complex_t> &fd);

    /**
     * Same as return ToPolar(fd, fd_magnitudes, nullptr, req_len, start_idx);
     */
    uint32_t ToPolar_(std::vector<complex_t> &fd, amplitude_t *fd_magnitudes,
                      uint32_t req_len, uint32_t start_idx);

    /**
     * Reduce noise in FFT results by applying moving average
     *
     * @param fd_magnitudes frequency domain including padding of \p (window - 1)
     * @param fd_len        length of the \p freqDomainMagnitudes
     *                      excluding padding
     * @param window        moving average window size
     */
    void Avg_(amplitude_t *fd_magnitudes, uint32_t fd_len, uint32_t window);

    /**
     * Convert FFT to harmonic product spectrum
     *
     * @param fd_magnitudes frequency domain
     * @param fd_len        length of the \p fd_magnitudes
     */
    void ToHPS_(amplitude_t *fd_magnitudes, uint32_t fd_len);

    /**
     *  Attenuate frequencies lower than \p freq to 0
     *
     *  Attenuate instead of cutting of the range to be able to do conversion
     *  between frequencies and indexes of FFT points
     *
     *  @param f_low_idx    index to cut from
     */
    void AttLowFreqs(uint32_t f_low_idx);

    /**
     * Inverse DFT calculation
     */
    void Inverse(std::vector<complex_t> & input);

public:
    FFT(amplitude_t *td, uint32_t td_len, uint32_t samplerate, freq_hz_t f_low,
        freq_hz_t f_high, bool polar, bool hps);

    FFT(amplitude_t *td, uint32_t td_len, uint32_t samplerate, bool polar);

    ~FFT();

    uint32_t FreqToIdx(freq_hz_t, double (*roundFunc)(double)) override;

    freq_hz_t IdxToFreq(uint32_t) override;

    /**
     * Same as Inverse(fd_.r)
     */
    void Inverse();
};

/** @} */
