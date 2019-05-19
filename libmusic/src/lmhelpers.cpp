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
 * @file    lmhelpers.cpp
 * @brief   Implementation of helpers
 */

#include <map>

#include "lmhelpers.h"

using namespace std;


uint32_t Helpers::nextPowerOf2(uint32_t n)
{
	return 1 << ((sizeof(uint32_t) * 8) - __builtin_clz(n - 1));
}

vector<complex_t> Helpers::timeDomain2ComplexVector(amplitude_t *timeDomain,
                                uint32_t timeDomainSize, uint32_t resultSize)
{
    vector<complex_t> x;
    uint32_t i;

    for (i = 0; i < timeDomainSize; i++) {
        x.push_back(complex_t (timeDomain[i], 0));
    }

    // pad the rest with zeroes
    for (; i < resultSize; i++) {
        x.push_back(complex_t (0, 0));
    }

    return x;
}

bool Helpers::isPowerOf2(uint32_t n)
{
    return ((n & (n - 1)) != 0);
}

bool Helpers::almostEqual(double a, double b, double eps)
{
    if (abs(a - b) < eps) {
        return true;
    }

    return false;
}
