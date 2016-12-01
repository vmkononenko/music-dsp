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

        scale = getScale(note, false);
        __mScales.push_back(new MusicScale(scale, true));
    }
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
    uint32_t fftSize;
    uint32_t firstPointsCnt, lowFreqThreshold;
    PriorityQueue *pq = new PriorityQueue();

    /* TODO: define minimum FFT size for frequency calculation precision */
    fftSize = Helpers::nextPowerOf2(samples);
    x = Helpers::timeDomain2ComplexVector(timeDomain, fftSize);

    __mFft->forward(x);

    // TODO: check for +/-1 error
    firstPointsCnt = __cutoffHighIdx(FREQ_C6, sampleRate, fftSize);
    // TODO: check for returned length
    __mFft->toPolar(x, NULL, pq, firstPointsCnt);

    /* calculate lowFreqThreshold */
    double tmpThreshold = FREQ_A0 * fftSize / sampleRate;
    lowFreqThreshold = floor(tmpThreshold);

    /* main algorithm */
    vector<uint8_t> scalesIndexes;
    FftPoint curMax(0, 0);
    freq_hz_t freq, pitch;
    note_t pitchNote;
    for (uint8_t i = 0; i < __mScales.size(); i++) {
        scalesIndexes.push_back(i);
    }
    while (scalesIndexes.size() > 1) {
        curMax = pq->delMax();
        if (curMax.sampleNumber < lowFreqThreshold) { continue; }
        freq = curMax.sampleNumber * sampleRate / fftSize;
        pitch = __mPitchCalculator->getPitch(freq);
        pitchNote = __mPitchCalculator->pitchToNote(pitch);
        for (auto it = scalesIndexes.begin(); it != scalesIndexes.end(); ) {
            if (!__mScales[*it]->hasNote(pitchNote)) {
                it = scalesIndexes.erase(it);
            } else {
                it++;
            }
        }
    }

    if (scalesIndexes.size() == 1) {
        chord.mainNote = __mScales[scalesIndexes[0]]->getTonic();
        chord.isMinor = __mScales[scalesIndexes[0]]->isMinor();
    } else /* size() == 0 */ {
        chord.mainNote = note_Unknown;
        return chord;
    }

    delete pq;
    return chord;
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
