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

// TODO: implement this in one of the non-static classes to avoid map reinit
//       on stack every time
string Helpers::noteToString(note_t note)
{
    if (((note < note_Min) && (note > note_Max)) && (note != note_Unknown)) {
        throw std::invalid_argument("Invalid note");
    }

    std::map<note_t, string> n2sMap;

    n2sMap[note_C]          = "C";
    n2sMap[note_C_sharp]    = "C#";
    n2sMap[note_D]          = "D";
    n2sMap[note_D_sharp]    = "D#";
    n2sMap[note_E]          = "E";
    n2sMap[note_F]          = "F";
    n2sMap[note_F_sharp]    = "F#";
    n2sMap[note_G]          = "G";
    n2sMap[note_G_sharp]    = "G#";
    n2sMap[note_A]          = "A";
    n2sMap[note_A_sharp]    = "A#";
    n2sMap[note_B]          = "B";
    n2sMap[note_Unknown]    = "??";

    return string(n2sMap[note]);
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
