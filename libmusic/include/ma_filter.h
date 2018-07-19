/**
 * @file        ma_filter.h
 * @brief       Defines API for moving average filter
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "filter.h"


class MAFilter : public Filter {

private:
    uint16_t __mDF;

public:
    MAFilter();

    MAFilter(uint16_t df);

    std::vector<amplitude_t> process(amplitude_t *td, uint32_t samples) override;

    uint16_t getDownsampleFactor() override;

    ~MAFilter();
};
