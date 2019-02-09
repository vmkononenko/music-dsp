/**
 * @file        fft_wrapper.h
 * @brief       Wrapper class for FFT implementing TFT interface
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include "tft.h"

namespace anatomist {

class FFTWrapper : public TFT {

private:
    /**
     * Performs logarithmic pruning of FFT frequencies
     */
    fd_t FFTPruned(FFT *fft);

public:
    FFTWrapper(freq_hz_t f_min, freq_hz_t f_max, uint16_t bpo, uint32_t sample_rate,
               uint16_t win_size, uint16_t hop_size);

    FFTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
               uint16_t win_size, uint16_t hop_size);

    ~FFTWrapper() {}

    void Process(td_t td, uint32_t offset) override;

    uint8_t BinsPerSemitone() override;

    uint32_t FreqToBin(freq_hz_t f) override;

    freq_hz_t BinToFreq(uint32_t idx) override;
};

}

/** @} */
