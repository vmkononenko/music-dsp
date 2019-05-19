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
 * ma_filter.cpp
 *
 * Moving average filter implementation
 */

#include "config.h"
#include "ma_filter.h"

using namespace std;

MAFilter::MAFilter() : MAFilter(CFG_MA_FILT_DF_DEFAULT) {}

MAFilter::MAFilter(uint16_t df)
{
    __mDF = df;
}

vector<amplitude_t> MAFilter::process(amplitude_t *td, uint32_t samples)
{
    vector<amplitude_t> ret;
    amplitude_t tmp;
    uint32_t i, j;

    for(i = 0; i < samples; i += __mDF) {
        tmp = 0;
        for (j = 0; ((j < __mDF) && (i + j < samples)); j++) {
            tmp += td[i + j];
        }
        ret.push_back(tmp / (j + 1));
    }

    return ret;
}

uint16_t MAFilter::getDownsampleFactor()
{
    return __mDF;
}

MAFilter::~MAFilter() {}
