/**
 * @file    chord_detector.cpp
 * @brief   Chord recognition implementation
 * @ingroup libmusic
 * @{
 */

#include "beat_detector.h"
#include "chord_detector.h"
#include "config.h"
#include "lmhelpers.h"
#include "lmlogger.h"
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

ChordDetector::FFTResults ChordDetector::__getFftResults(amplitude_t *timeDomain,
        uint32_t samples, uint32_t sampleRate)
{
    if ((timeDomain == nullptr) || (samples == 0) || sampleRate == 0 ||
        (samples > CFG_FFT_SIZE))
    {
        throw invalid_argument("__getFftResults() invalid argument");
    }

    FFTResults res;
    vector<complex_t> x;

    WindowFunctions::applyDefault(timeDomain, samples);

    res.fftSize = Helpers::nextPowerOf2(samples);
    x = Helpers::timeDomain2ComplexVector(timeDomain, samples, res.fftSize);

    __mFft->forward(x);

    /** @TODO check for +/-1 error */
    res.highFreqThresholdIdx = Helpers::freqToFftIdx(sampleRate/2, res.fftSize,
                                                     sampleRate, ceil);

    res.freqDomain = new amplitude_t[res.highFreqThresholdIdx];

    /** @TODO check for returned length */
    res.highFreqThresholdIdx = __mFft->toPolar(x, res.freqDomain, res.highFreqThresholdIdx);

    res.sampleRate = sampleRate;

    return res;
}

chord_t ChordDetector::__getChordFromFftResults(FFTResults& fftRes)
{
    if (fftRes.freqDomain == nullptr) {
        throw std::invalid_argument("freqDomain is NULL");
    }

    PitchClsProfile *pcp = new PitchClsProfile(fftRes.freqDomain, fftRes.fftSize,
            fftRes.sampleRate, fftRes.highFreqThresholdIdx);

    chord_t chord = __mChordTplColl->getBestMatch(pcp);

    delete pcp;

    return chord;
}

chord_t ChordDetector::getChord(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{

    FFTResults fftRes = __getFftResults(timeDomain, samples, sampleRate);
    chord_t ret = __getChordFromFftResults(fftRes);

    delete[] fftRes.freqDomain;

    return ret;
}

void ChordDetector::getSegments(std::vector<segment_t>& segments,
                                amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    uint32_t winSize, offset;

#ifdef CFG_DYNAMIC_WINDOW
    BeatDetector *bd = new BeatDetector(timeDomain, samples, sampleRate);

    winSize = bd->getIdxInterval();
    offset = bd->getOffset();

    delete bd;
#else
    winSize = CFG_WINDOW_SIZE;
    offset = 0;
#endif

    for (uint32_t sampleIdx = offset; sampleIdx < samples; sampleIdx += winSize) {
        uint32_t segLen = min(winSize, samples - sampleIdx);
        segment_t segment;
        segment.chord = getChord(timeDomain + sampleIdx, segLen, sampleRate);
        segment.startIdx = sampleIdx;
        segment.endIdx = sampleIdx + segLen - 1;
        segments.push_back(segment);
    }
}

PitchClsProfile ChordDetector::getPCP(amplitude_t *timeDomain, uint32_t samples,
                                      uint32_t sampleRate)
{
    FFTResults fftRes = __getFftResults(timeDomain, samples, sampleRate);
    PitchClsProfile pcp = PitchClsProfile(fftRes.freqDomain, fftRes.fftSize,
            fftRes.sampleRate, fftRes.highFreqThresholdIdx);

    delete[] fftRes.freqDomain;

    return pcp;
}

/** @} */

