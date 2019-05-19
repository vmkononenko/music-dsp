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
 * recursive_filter.cpp
 *
 * Recursive filter implementation
 */

#include "recursive_filter.h"


using namespace std;


std::vector<amplitude_t> RecursiveFilter::leftToRight(amplitude_t *x, uint32_t samples)
{
    vector<amplitude_t> y(samples);

    for (int32_t n = 0; n < (int32_t)samples; n++) {
        y[n] = mB[0] * x[n];
        for (int32_t i = 1; ((i <= mPoles) && (n - i >= 0)); i++) {
            y[n] += mB[i] * x[n - i] - mA[i] * y[n - i];
        }
    }

    return y;
}

std::vector<amplitude_t> RecursiveFilter::rightToLeft(amplitude_t *x, uint32_t c)
{
    vector<amplitude_t> y(c);

    for (int32_t n = (int32_t)(c - 1); n >= 0; n--) {
        y[n] = mB[0] * x[n];
        for (int32_t i = 1; ((i <= mPoles) && (n + i < (int32_t)c)); i++) {
            y[n] += mB[i] * x[n + i] - mA[i] * y[n + i];
        }
    }

    return y;
}

vector<amplitude_t> RecursiveFilter::process(amplitude_t *td, uint32_t samples)
{
    vector<amplitude_t> l2r = leftToRight(td, samples);
    vector<amplitude_t> r2l = rightToLeft(l2r.data(), l2r.size());

    return r2l;
}

RecursiveFilter::~RecursiveFilter() {}
