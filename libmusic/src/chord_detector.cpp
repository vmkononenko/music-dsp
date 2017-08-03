/*
 * fft.cpp
 *
 *  Chord recognition implementation
 */

#include "chord_detector.h"
#include "config.h"
#include "lmhelpers.h"
#include "lmlogger.h"
#include "pitch_cls_profile.h"
#include "window_functions.h"

#define FREQ_A0     ((freq_hz_t)27.5)
#define FREQ_E2     ((freq_hz_t)82.4)
#define FREQ_C6     ((freq_hz_t)1046.5)
#define FREQ_C8     ((freq_hz_t)4186)

#define LOG_TAG	"GT.ChordDetector"

using namespace std;

ChordDetector::ChordDetector()
{
    LOGMSG_D(LOG_TAG, "Using window size %u, FFT size %u and %s window function",
             CFG_WINDOW_SIZE, CFG_FFT_SIZE, WindowFunctions::toString(CFG_WINDOW_FUNC));

    __mFft = new FFT();
}

ChordDetector::~ChordDetector()
{
    delete __mFft;
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

chord_t ChordDetector::__getChordFromFftResults(amplitude_t *freqDomainMagnitudes,
        uint32_t fftSize, uint32_t sampleRate, uint32_t pointsCnt)
{
    if (freqDomainMagnitudes == nullptr) {
        throw std::invalid_argument("freqDomainMagnitudes is NULL");
    }

    PitchClsProfile *pcp = new PitchClsProfile(freqDomainMagnitudes, fftSize,
                                               sampleRate, pointsCnt);

    chord_t chord = __mChordTplColl->getBestMatch(pcp);

    delete pcp;

    return chord;
}

chord_t ChordDetector::getChord(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    if ((timeDomain == nullptr) || (samples == 0) || sampleRate == 0 ||
        (samples > CFG_FFT_SIZE))
    {
        throw invalid_argument("getChord() invalid argument");
    }

    vector<complex_t> x;
    amplitude_t *freqDomain;
    uint32_t highFreqThresholdIdx;  // do not analyze points above this index
    uint32_t fftSize;

    WindowFunctions::applyDefault(timeDomain, samples);

    /* TODO: define minimum FFT size for frequency calculation precision */
    //fftSize = Helpers::nextPowerOf2(samples);
    fftSize = CFG_FFT_SIZE;
    x = Helpers::timeDomain2ComplexVector(timeDomain, samples, fftSize);

    __mFft->forward(x);

    // TODO: check for +/-1 error
    highFreqThresholdIdx = Helpers::freqToFftIdx(sampleRate/2, fftSize, sampleRate, ceil);

    freqDomain = new amplitude_t[highFreqThresholdIdx];

    // TODO: check for returned length
    highFreqThresholdIdx = __mFft->toPolar(x, freqDomain, highFreqThresholdIdx);

    chord_t ret = __getChordFromFftResults(freqDomain, fftSize,
                                           sampleRate, highFreqThresholdIdx);

    delete[] freqDomain;

    return ret;
}

