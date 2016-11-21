/*
 * fft.cpp
 *
 *  Chord recognition implementation
 */

#include <iostream> // TODO remove when __cutoffFreq is implemented

#include "chord_detector.h"
#include "lmhelpers.h"

#define FREQ_A0     ((amplitude_t)27.5)
#define FREQ_C8     ((amplitude_t)27.5)


using namespace std;

ChordDetector::ChordDetector()
{
    __mFft = new FFT();
    __mPitchDetector = new PitchDetector();
}

void ChordDetector::__cutoffFreq(vector<complex_t> &x, uint32_t sampleRate,
                                 amplitude_t freq, bool cutHigh)
{
    double cutFromIdx = freq * sampleRate / x.size();

    if (cutHigh) {
        cutFromIdx = ceil(cutFromIdx);
    } else {
        cutFromIdx = floor(cutFromIdx);
    }

    if ((cutFromIdx < 0) || (cutFromIdx >= x.size())) {
        return;
    }

    if (cutHigh) {
        x.erase(x.begin() + cutFromIdx, x.end());
    } else {
        for (uint32_t i = 0; i <= cutFromIdx; i++) {
            x[i] = complex_t (0, 0);
        }
    }
}

chord_t ChordDetector::getChord(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    chord_t chord;

    if ((timeDomain == NULL) || (samples == 0)) {
        chord.mainNote = note_Unknown;
        return chord;
    }

    vector<complex_t> x;
    freq_hz_t pitchFreq;
    uint32_t fftSize;

    /* TODO: define minimum FFT size for frequency calculation precision */
    fftSize = Helpers::nextPowerOf2(samples);
    x = Helpers::timeDomain2ComplexVector(timeDomain, fftSize);

    __mFft->forward(x);
    __mFft->toPolar(x);
    __cutoffFreq(x, sampleRate, FREQ_A0, false);
    __cutoffFreq(x, sampleRate, FREQ_C8, true);

    pitchFreq = __mPitchDetector->getPitch(x, sampleRate);
    if (pitchFreq == FREQ_INVALID) {
        goto err;
    }

    chord.mainNote = __mPitchDetector->pitchToNote(pitchFreq);

    return chord;

err:
    chord.mainNote = note_Unknown;
    return chord;
}
