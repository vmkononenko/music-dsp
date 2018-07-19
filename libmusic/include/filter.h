/**
 * @file        filter.h
 * @brief       Defines interface for filters
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"


class Filter {

public:
    virtual std::vector<amplitude_t> process(amplitude_t *td, uint32_t samples) = 0;

    virtual uint16_t getDownsampleFactor() { return 1; };

    virtual ~Filter() {};
};
