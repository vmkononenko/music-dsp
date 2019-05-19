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
 * @file        cheby1_filter.h
 * @brief       Defines API for Recursive filter
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "filter.h"


class RecursiveFilter : public Filter {

private:
    std::vector<amplitude_t> leftToRight(amplitude_t *td, uint32_t samples);
    std::vector<amplitude_t> rightToLeft(amplitude_t *td, uint32_t samples);

protected:
    std::vector<double> mB;
    std::vector<double> mA;
    uint8_t             mPoles;

    RecursiveFilter() {};

public:
    /**
     *  Performs zero-phase filtering
     *
     *  Zero phase filtering is achieved by processing the input data
     *  in both the forward and reverse directions.
     *
     *  @param td       time domain of the input signal
     *  @param samples  length of the \p td
     *  @return         filtered data
     */
    std::vector<amplitude_t> process(amplitude_t *td, uint32_t samples) override;

    virtual ~RecursiveFilter();
};
