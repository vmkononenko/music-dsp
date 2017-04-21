/*
 * lmhelpers.cpp
 *
 * Implementation of helpers
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

string Helpers::noteToString(note_t note)
{
    return Chord::noteToString(note);
}

uint32_t Helpers::freqToFftIdx(freq_hz_t freq, uint32_t fftSize,
                               uint32_t sampleRate, double (*roundFunc)(double))
{
    return (*roundFunc)(freq * fftSize / sampleRate);
}

freq_hz_t Helpers::fftIdxToFreq(uint32_t idx, uint32_t fftSize, uint32_t sampleRate)
{
    return (idx * (double)sampleRate / fftSize);
}

bool Helpers::isPowerOf2(uint32_t n)
{
    return ((n & (n - 1)) != 0);
}
