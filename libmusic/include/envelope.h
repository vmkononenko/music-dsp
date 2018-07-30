/**
 * @file        envelope.h
 * @brief       API for envelope detection
 *
 * Extracts and keeps envelope of the input signal. Is based on squaring
 * and low pass filtering method:
 * http://www.mathworks.com/help/dsp/examples/envelope-detection.html
 *
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"


class Envelope {

private:
    Envelope();

    std::vector<amplitude_t> __mEnvelope;
    uint16_t __mDF = 1;     // total downsampling factor caused by filtering

    void squareAndDouble(amplitude_t *td, uint32_t samples);

    void squareRoot(std::vector<amplitude_t> &input);

public:
    Envelope(amplitude_t *td, uint32_t samples);

    std::vector<amplitude_t> diff();

    uint16_t getDownsampleFactor();

    friend std::ostream& operator<<(std::ostream& os, const Envelope& e);

    ~Envelope();
};