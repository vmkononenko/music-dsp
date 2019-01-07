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


public:
    void Process(td_t td, uint32_t offset);

};

}

/** @} */
