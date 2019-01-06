/**
 * @file        q_transform.h
 * @brief       Constant Q Transform API
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "CQBase.h"
#include "CQSpectrogram.h"

#include "lmtypes.h"

#define BINS_PER_OCTAVE_DEFAULT 36

typedef std::vector<std::vector<amplitude_t>> log_spectrogram_t;


typedef class QTransform {

private:
    CQSpectrogram   *cq_spectrogram_;
    freq_hz_t       f_min_;
    freq_hz_t       f_max_;
    uint32_t        win_size_;
    uint32_t        interval_;

    log_spectrogram_t ConvertRealBlock_(CQBase::RealBlock &block, uint32_t hop_size);

    void Denoise_(log_spectrogram_t &block);

public:

    QTransform(uint8_t bpo, uint32_t samplerate, freq_hz_t f_low,
               freq_hz_t f_high, uint32_t win_size);

    QTransform(uint32_t samplerate, freq_hz_t f_low, freq_hz_t f_high,
               uint32_t win_size);

    ~QTransform();

    log_spectrogram_t GetSpectrogram(amplitude_t *td, uint32_t td_len,
                                     uint32_t offset, uint32_t hop_size);

    uint32_t SpectrogramInterval();

    uint8_t BinsPerSemitone();

    uint32_t FreqToBin(freq_hz_t f);

    freq_hz_t BinToFreq(uint32_t idx);

} log_spectrum_t;

/** @} */
