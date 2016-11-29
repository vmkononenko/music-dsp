/*
 * fft.cpp
 *
 *  Chord recognition implementation
 */

#include <iostream> // TODO remove when __cutoffFreq is implemented

#include "chord_detector.h"
#include "lmhelpers.h"

#define FREQ_A0     ((freq_hz_t)27.5)
#define FREQ_C6     ((freq_hz_t)1046.5)
#define FREQ_C8     ((freq_hz_t)4186)

using namespace std;

ChordDetector::ChordDetector()
{
    __mFft = new FFT();
    __mPitchCalculator = new PitchCalculator();
}

void ChordDetector::__attLowFreq(amplitude_t *freqDomain, uint32_t len,
        freq_hz_t freq, uint32_t fftSize, uint32_t sampleRate)
{
    double cutFromIdx = freq * fftSize / sampleRate;

    cutFromIdx = floor(cutFromIdx);

    if ((cutFromIdx < 0) || (cutFromIdx >= len)) {
        return;
    }

    for (uint32_t i = 0; i <= cutFromIdx; i++) {
        freqDomain[i] = 0;
    }
}

uint32_t ChordDetector::__cutoffHighIdx(freq_hz_t freq, uint32_t sampleRate,
                                        uint32_t fftSize)
{
    return ceil(freq * fftSize / sampleRate);
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
    uint32_t firstPointsCnt;
    amplitude_t *freqDomain;

    /* TODO: define minimum FFT size for frequency calculation precision */
    fftSize = Helpers::nextPowerOf2(samples);
    x = Helpers::timeDomain2ComplexVector(timeDomain, fftSize);

    __mFft->forward(x);

    // TODO: check for +/-1 error
    firstPointsCnt = __cutoffHighIdx(FREQ_C8, sampleRate, fftSize);
    freqDomain = new amplitude_t[firstPointsCnt];
    // TODO: check for returned length
    __mFft->toPolar(x, freqDomain, firstPointsCnt);
    __attLowFreq(freqDomain, firstPointsCnt, FREQ_A0, fftSize, sampleRate);

    pitchFreq = __mPitchCalculator->getPitch(freqDomain, firstPointsCnt, fftSize,
                                           sampleRate);
    if (pitchFreq == FREQ_INVALID) {
        goto err;
    }

    chord.mainNote = __mPitchCalculator->pitchToNote(pitchFreq);

    return chord;

err:
    chord.mainNote = note_Unknown;
    return chord;
}

vector<note_t> ChordDetector::getScale(note_t mainNote, bool isMajor)
{
    /* whole, whole, half, whole, whole, whole, half */
    uint8_t formulaMajor[] = {2, 2, 1, 2, 2, 2, 1};
    /* whole, half, whole, whole, half, whole, whole */
    uint8_t formulaMinor[] = {2, 1, 2, 2, 1, 2, 2};
    uint8_t *formula = isMajor ? formulaMajor : formulaMinor;

    vector<note_t> scale;
    freq_hz_t pitch;

    scale.push_back(mainNote);
    pitch = __mPitchCalculator->noteToPitch(mainNote, OCTAVE_4);
    for (uint8_t i = 0; i < 7; /* size of formula */ i++) {
        pitch = __mPitchCalculator->getPitchByInterval(pitch, formula[i]);
        scale.push_back(__mPitchCalculator->pitchToNote(pitch));
    }

    return scale;
}
