/**
 * @file        transform.h
 * @brief       Abstract class for transforms of a signal to frequency domain
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"


/* Frequency domain in one of polar or rectangular notation */
typedef union {

private:
    void            *rect;  // rectangular notation

public:
    amplitude_t     *p;     // polar notation

    void set_r(std::vector<complex_t> *v)
    {
        rect = static_cast<void *>(v);
    }

    std::vector<complex_t> * r()
    {
        return static_cast<std::vector<complex_t> *>(rect);
    }
} freq_domain_t;


class Transform {

protected:

    freq_domain_t   fd_;
    uint32_t        fd_len_;
    uint32_t        size_;
    uint32_t        samplerate_;
    bool            polar_;

    Transform();

public:

    /**
     * Convert frequency in Hz to bin index in frequency domain array
     *
     * @param   freq    frequency in Hz
     * @param   round_f pointer to function which will perform rounding
     * @return  index value
     */
    virtual uint32_t FreqToIdx(freq_hz_t freq, double (*round_f)(double)) = 0;

    /**
     * Calculate frequency in Hz from bin index in frequency domain array
     *
     * @param   idx bin index in frequency domain array
     * @return  frequency in Hz
     */
    virtual freq_hz_t IdxToFreq(uint32_t idx) = 0;

    bool IsPolar();

    freq_domain_t & GetFreqDomain();

    uint32_t GetFreqDomainLen();

    uint32_t GetSize();

    uint32_t GetSampleRate();

    virtual ~Transform();
};
