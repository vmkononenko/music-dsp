/**
 * @file        cqt_wrapper.h
 * @brief       Wrapper class for ext/cqtt constant Q implementation
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include "CQBase.h"
#include "CQSpectrogram.h"

#include "lmtypes.h"
#include "tft.h"

namespace anatomist {

typedef class CQTWrapper : public TFT {

private:
    CQSpectrogram   *cq_spectrogram_;

    log_spectrogram_t ConvertRealBlock_(CQBase::RealBlock &block);

public:

    CQTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint16_t bpo,
               uint32_t sample_rate, uint16_t win_size, uint16_t hop_size);

    CQTWrapper(freq_hz_t f_low, freq_hz_t f_high, uint32_t sample_rate,
               uint16_t win_size, uint16_t hop_size);

    ~CQTWrapper();

    void Process(td_t td, uint32_t offset) override;

    uint8_t BinsPerSemitone() override;

    uint32_t FreqToBin(freq_hz_t f) override;

    freq_hz_t BinToFreq(uint32_t idx) override;

} log_spectrum_t;

}

/** @} */
