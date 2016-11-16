/*
 * lmhelpers.cpp
 *
 * Implementation of helpers
 */

#include "lmhelpers.h"

using namespace std;


uint32_t Helpers::nextPowerOf2(uint32_t n)
{
	return 1 << ((sizeof(uint32_t) * 8) - __builtin_clz(n - 1));
}

vector<complex_t> Helpers::timeDomain2ComplexVector(amplitude_t *timeDomain, uint32_t samples)
{
    vector<complex_t> x;

    for (uint32_t i = 0; i < samples; i++) {
        x.push_back(complex_t (timeDomain[i], 0));
    }

    return x;
}
