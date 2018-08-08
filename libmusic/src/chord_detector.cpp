/**
 * @file    chord_detector.cpp
 * @brief   Chord recognition implementation
 * @ingroup libmusic
 * @{
 */

#include <algorithm>

#include "beat_detector.h"
#include "chord_detector.h"
#include "config.h"
#include "envelope.h"
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

void ChordDetector::__attLowFreq(FFTResults& fftRes, freq_hz_t freq)
{
    uint32_t cutFromIdx = Helpers::freqToFftIdx(freq, fftRes.fftSize, fftRes.sampleRate, floor);

    cutFromIdx = min(cutFromIdx, fftRes.freqDomainLen);

    for (uint32_t i = 0; i <= cutFromIdx; i++) {
        fftRes.freqDomain[i] = 0;
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

    res.freqDomainLen = Helpers::freqToFftIdx(FREQ_C8, res.fftSize, sampleRate, ceil) + 1;

    res.freqDomain = new amplitude_t[res.freqDomainLen + CFG_FFT_AVG_WINDOW - 1];

    /** @TODO check for returned length */
    res.freqDomainLen = __mFft->toPolar(x, res.freqDomain,
                                        res.freqDomainLen + CFG_FFT_AVG_WINDOW - 1);
    __mFft->avg(res.freqDomain, res.freqDomainLen, CFG_FFT_AVG_WINDOW);
    __mFft->toHPS(res.freqDomain, res.freqDomainLen);
    __attLowFreq(res, FREQ_E2);
    res.sampleRate = sampleRate;

    return res;
}

chord_t ChordDetector::__getChordFromFftResults(FFTResults& fftRes)
{
    if (fftRes.freqDomain == nullptr) {
        throw std::invalid_argument("freqDomain is NULL");
    }

    pcp_t *pcp = __FFT2PCP(fftRes);
    chord_t chord = __mChordTplColl->getBestMatch(pcp);

    delete pcp;

    return chord;
}

pcp_t * ChordDetector::__FFT2PCP(FFTResults& fftRes)
{
    return new PitchClsProfile(fftRes.freqDomain, fftRes.fftSize,
                               fftRes.sampleRate, fftRes.freqDomainLen);
}

chord_t ChordDetector::getChord(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{

    FFTResults fftRes = __getFftResults(timeDomain, samples, sampleRate);
    chord_t ret = __getChordFromFftResults(fftRes);

    delete[] fftRes.freqDomain;

    return ret;
}

chord_t ChordDetector::__getChordFromPCPBuf(PCPBuf *pcpBuf)
{
    map<chord_t, uint32_t> counter;

    for (const auto pcp : pcpBuf->getProfiles()) {
        chord_t c = __mChordTplColl->getBestMatch(pcp);
        auto it = counter.find(c);

        if (it != counter.end()) {
            it->second++;
        } else {
            counter.insert(pair<chord_t, uint32_t>(c, 1));
        }
    }

    auto it = max_element(counter.begin(), counter.end(), Helpers::cmpPairBySecond<chord_t, uint32_t>);

    return it->first;
}

void ChordDetector::__processSegment(vector<segment_t> *segments, uint32_t startIdx,
                                     uint32_t endIdx, bool silence, PCPBuf *pcpBuf,
                                     ResultsListener *listener, uint32_t samplesTotal)
{
    if (startIdx >= endIdx) {
        throw invalid_argument("__processSegment(): invalid indices");
    }

    segment_t segment;

    segment.startIdx = startIdx;
    segment.endIdx = endIdx;
    segment.silence = silence;

    if (!silence) {
        segment.chord = __getChordFromPCPBuf(pcpBuf);
        pcpBuf->flush();
    }

    if (listener == nullptr) {
        segments->push_back(segment);
    } else {
        listener->onChordSegmentProcessed(segment, (endIdx / (float)samplesTotal));
    }
}

void ChordDetector::__getSegments(std::vector<segment_t> *segments,
                                  amplitude_t *timeDomain, uint32_t samples,
                                  uint32_t sampleRate, ResultsListener *listener)
{
    Envelope *e = new Envelope(timeDomain, samples);
    PCPBuf *pcpBuf = new PCPBuf();
    uint32_t winSize, offset;
    uint32_t nextSegIdx = 0, segEndIdx;

#ifdef CFG_DYNAMIC_WINDOW
    BeatDetector *bd = new BeatDetector(e, sampleRate);

    winSize = bd->getIdxInterval();
    offset = bd->getOffset();

    delete bd;
#else
    winSize = CFG_WINDOW_SIZE;
    offset = 0;
#endif

    if (listener != nullptr) {
        listener->onPreprocessingProgress(1);
    }

    for (uint32_t sampleIdx = offset; sampleIdx < samples; sampleIdx += winSize) {
        uint32_t len = min(winSize, samples - sampleIdx);

        segEndIdx = sampleIdx + len - 1;

        if (e->isSilence(sampleIdx, segEndIdx)) {
            if (pcpBuf->size() > 0) {
                __processSegment(segments, nextSegIdx, sampleIdx - 1, false,
                                 pcpBuf, listener, samples);
            }
            __processSegment(segments, sampleIdx, segEndIdx, true, pcpBuf,
                             listener, samples);
            nextSegIdx = segEndIdx + 1;
            continue;
        }

        FFTResults fftRes = __getFftResults(timeDomain + sampleIdx, len, sampleRate);
        pcp_t *pcp = __FFT2PCP(fftRes);

        if (pcpBuf->vectorChange(pcp)) {
            __processSegment(segments, nextSegIdx, sampleIdx - 1, false, pcpBuf,
                             listener, samples);
            nextSegIdx = sampleIdx;
        }

        pcpBuf->add(pcp);
    }

    if (listener != nullptr) {
        listener->onChordAnalysisFinished();
    }

    delete e;
    delete pcpBuf;
}

void ChordDetector::getSegments(std::vector<segment_t>& segments,
                                amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    __getSegments(&segments, timeDomain, samples, sampleRate, nullptr);
}

void ChordDetector::getSegments(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate, ResultsListener *listener)
{
    __getSegments(nullptr, timeDomain, samples, sampleRate, listener);
}

PitchClsProfile ChordDetector::getPCP(amplitude_t *timeDomain, uint32_t samples,
                                      uint32_t sampleRate)
{
    FFTResults fftRes = __getFftResults(timeDomain, samples, sampleRate);
    PitchClsProfile pcp = PitchClsProfile(fftRes.freqDomain, fftRes.fftSize,
            fftRes.sampleRate, fftRes.freqDomainLen);

    delete[] fftRes.freqDomain;

    return pcp;
}

/** @} */

