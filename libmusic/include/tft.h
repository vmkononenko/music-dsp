/**
 * @file        tft.h
 * @brief       Abstract class for transforms of a signal to frequency domain
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"

#define BINS_PER_OCTAVE_DEFAULT 36

namespace anatomist {

typedef std::vector<fd_t> log_spectrogram_t;


typedef class TFT {

protected:
    /**
     * Lower boundaries of the frequency domain results
     */
    freq_hz_t     f_min_;

    /**
     * Upper boundaries of the frequency domain results
     */
    freq_hz_t     f_max_;

    /**
     * Bins per octave in the frequency domain results
     */
    const uint16_t      bpo_;

    /**
     * Sample rate of the data to be fed to \ref Process()
     */
    const uint32_t      sample_rate_;

    /**
     * Window size to slice time domain data into
     */
    const uint16_t      win_size_;

    /**
     * Hop size between two subsequent windows
     */
    const uint16_t      hop_size_;

    /**
     * Spectrogram - result of the processing
     */
    log_spectrogram_t   spectrogram_;

    /**
     * Interval between two subsequent columns in \ref spectrogram_
     *
     * Measured in samples corresponding to \ref sample_rate_
     */
    uint32_t        interval_;

    void Denoise_(log_spectrogram_t &block);

public:
    /**
     * Constructor
     */
    TFT(freq_hz_t f_min, freq_hz_t f_max, uint16_t bpo, uint32_t sample_rate,
        uint16_t win_size, uint16_t hop_size);

    /**
     * Destructor
     */
    virtual ~TFT() {}

    virtual log_spectrogram_t GetSpectrogram();

    virtual uint32_t SpectrogramInterval();

    virtual void Process(td_t td, uint32_t offset) = 0;

    virtual uint8_t BinsPerSemitone() = 0;

    virtual uint32_t FreqToBin(freq_hz_t f) = 0;

    virtual freq_hz_t BinToFreq(uint32_t idx) = 0;
} tft_t;

}

/** @} */
