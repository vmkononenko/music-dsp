/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

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
