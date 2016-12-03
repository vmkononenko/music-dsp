/*
 * fft.cpp
 *
 *  Chord recognition implementation
 */

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
    __initScales();
}

ChordDetector::~ChordDetector()
{
    delete __mFft;
    delete __mPitchCalculator;

    for (uint8_t i = 0; i < __mScales.size(); i++) {
        delete __mScales[i];
    }
}

void ChordDetector::__initScales()
{
    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;
        vector<note_t> scale;

        scale = getScale(note, false);
        __mScales.push_back(new MusicScale(scale, false));

        scale = getScale(note, true);
        __mScales.push_back(new MusicScale(scale, true));
    }
}

vector<note_t> ChordDetector::getScale(note_t mainNote, bool isMinor)
{
    /* whole, whole, half, whole, whole, whole, half */
    uint8_t formulaMajor[] = {2, 2, 1, 2, 2, 2, 1};
    /* whole, half, whole, whole, half, whole, whole */
    uint8_t formulaMinor[] = {2, 1, 2, 2, 1, 2, 2};
    uint8_t *formula = isMinor ? formulaMinor : formulaMajor;

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

void ChordDetector::__attLowFreq(amplitude_t *freqDomain, uint32_t len,
        freq_hz_t freq, uint32_t fftSize, uint32_t sampleRate)
{
    double cutFromIdx = Helpers::freqToFftIdx(freq, fftSize, sampleRate, floor);

    if ((cutFromIdx < 0) || (cutFromIdx >= len)) {
        return;
    }

    for (uint32_t i = 0; i <= cutFromIdx; i++) {
        freqDomain[i] = 0;
    }
}

bool ChordDetector::__isNotePresentInScales(vector<uint8_t> &scalesIndexes, note_t note)
{
    for (auto it = scalesIndexes.begin(); it != scalesIndexes.end(); it++) {
        if (__mScales[*it]->hasNote(note)) {
            return true;
        }
    }

    return false;
}

chord_t ChordDetector::__getChordFromFftResults(PriorityQueue *fftPQ,
        uint32_t fftSize, uint32_t sampleRate, uint32_t lowFreqThresholdIdx)
{
    if (fftPQ == nullptr) {
        throw std::invalid_argument("fftPQ is NULL");
    }

    chord_t chord;
    freq_hz_t freq;         // freq of the FFT point being analyzed
    freq_hz_t pitch;        // pitch corresponding to freq
    note_t pitchNote;       // note corresponding to pitch
    FftPoint curMax(0, 0);  // point with the highest amplitude in fftPQ
    vector<uint8_t> scalesIndexes;  // indexes in __mScales which are still
                                    // considered as candidates for the result

    for (uint8_t i = 0; i < __mScales.size(); i++) {
        scalesIndexes.push_back(i);
    }
    while (scalesIndexes.size() >= 1 && !fftPQ->isEmpty()) {
        curMax = fftPQ->delMax();

        /* skip points above lowFreqThresholdIdx */
        if (curMax.sampleNumber < lowFreqThresholdIdx) { continue; }

        freq = Helpers::fftIdxToFreq(curMax.sampleNumber, fftSize, sampleRate);
        pitch = __mPitchCalculator->getPitch(freq);

        /* skip frequencies we cannot rely on */
        if (pitch == FREQ_INVALID) { continue; }
        pitchNote = __mPitchCalculator->pitchToNote(pitch);

        /* if note is not contained in any of the remaining scales let's not
         * give up. Just skip it and pick the next one */
        if(!__isNotePresentInScales(scalesIndexes, pitchNote)) { continue; }

        for (auto it = scalesIndexes.begin(); it != scalesIndexes.end(); ) {
            if (!__mScales[*it]->hasNote(pitchNote)) {
                it = scalesIndexes.erase(it);
            } else {
                it++;
            }
        }
    }

    if (scalesIndexes.size() >= 1) {
        chord.mainNote = __mScales[scalesIndexes[0]]->getTonic();
        chord.isMinor = __mScales[scalesIndexes[0]]->isMinor();
    } else /* size() == 0 */ {
        chord.mainNote = note_Unknown;
        return chord;
    }

    return chord;
}

chord_t ChordDetector::getChord(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    chord_t chord;

    if ((timeDomain == nullptr) || (samples == 0) || sampleRate == 0) {
        chord.mainNote = note_Unknown;
        return chord;
    }

    vector<complex_t> x;
    PriorityQueue *pq = new PriorityQueue();
    uint32_t highFreqThresholdIdx;  // do not analyze points above this index
    uint32_t lowFreqThresholdIdx;   // do not analyze points below this index
    uint32_t fftSize;

    /* TODO: define minimum FFT size for frequency calculation precision */
    fftSize = Helpers::nextPowerOf2(samples);
    x = Helpers::timeDomain2ComplexVector(timeDomain, samples, fftSize);

    __mFft->forward(x);

    // TODO: check for +/-1 error
    highFreqThresholdIdx = Helpers::freqToFftIdx(sampleRate/2, fftSize, sampleRate, ceil);
    lowFreqThresholdIdx = Helpers::freqToFftIdx(FREQ_A0, fftSize, sampleRate, floor);

    // TODO: check for returned length
    __mFft->toPolar(x, NULL, pq, highFreqThresholdIdx);

    chord = __getChordFromFftResults(pq, fftSize, sampleRate, lowFreqThresholdIdx);

    delete pq;
    return chord;
}

