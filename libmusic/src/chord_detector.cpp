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

#define FREQ_E2     ((freq_hz_t)82.4)
#define FREQ_C8     ((freq_hz_t)4186)

#define LOG_TAG "AA.ChordDetector"

using namespace std;

ChordDetector::ChordDetector()
{
    LOGMSG_D(LOG_TAG, "Using window size %u, FFT size %u and %s window function",
             CFG_WINDOW_SIZE, CFG_FFT_SIZE, WindowFunctions::toString(CFG_WINDOW_FUNC));
}

FFT * ChordDetector::GetFft_(amplitude_t *td, uint32_t samples, uint32_t samplerate)
{
    if ((td == nullptr) || (samples == 0) || samplerate == 0 ||
        (samples > CFG_FFT_SIZE))
    {
        throw invalid_argument("__getFftResults() invalid argument");
    }

    WindowFunctions::applyDefault(td, samples);

    return new FFT(td, samples, samplerate, FREQ_E2, FREQ_C8, true, true);
}

chord_t ChordDetector::GetChordFromFft_(FFT *fft)
{
    if ((fft == nullptr) || !fft->IsPolar()) {
        throw std::invalid_argument("GetChordFromFft_: invalid fft");
    }

    pcp_t *pcp = new PitchClsProfile(fft);
    chord_t chord = __mChordTplColl->getBestMatch(pcp);

    delete pcp;

    return chord;
}

chord_t ChordDetector::getChord(amplitude_t *td, uint32_t samples,
                                uint32_t samplerate)
{

    FFT *fft = GetFft_(td, samples, samplerate);
    chord_t ret = GetChordFromFft_(fft);

    delete fft;

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
    PCPBuf *pcpHopsBuf = new PCPBuf();
    uint32_t winSize, offset, hopSize;
    uint32_t nextSegIdx = 0, segEndIdx;
    uint32_t winCnt = 0;

#ifdef CFG_DYNAMIC_WINDOW
    BeatDetector *bd = new BeatDetector(e, sampleRate);

    winSize = bd->getIdxInterval();
    offset = bd->getOffset();

    delete bd;
#else
    winSize = CFG_WINDOW_SIZE;
    offset = 0;
#endif

    hopSize = winSize / CFG_HOPS_PER_WINDOW;

    if (listener != nullptr) {
        listener->onPreprocessingProgress(1);
    }

    for (uint32_t sampleIdx = offset; sampleIdx < samples; sampleIdx += hopSize, ++winCnt) {
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

        FFT *fft = GetFft_(timeDomain + sampleIdx, len, sampleRate);
        pcp_t *pcp = new PitchClsProfile(fft);

        delete fft;

        if (CFG_HOPS_PER_WINDOW > 1)  {
            pcpHopsBuf->add(pcp);

            if (winCnt % CFG_HOPS_PER_WINDOW == 0) {
                pcp = pcpHopsBuf->getCombinedPCP();
                pcpHopsBuf->flush();
            } else {
                continue;
            }
        }

        if ((pcpBuf->vectorChange(pcp) && pcpBuf->size() >= 2) || (len < winSize)) {
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
    delete pcpHopsBuf;
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

pcp_t * ChordDetector::GetPCP(amplitude_t *x, uint32_t samples, uint32_t samplerate)
{
    FFT *fft = GetFft_(x, samples, samplerate);
    pcp_t *pcp = new PitchClsProfile(fft);

    delete fft;

    return pcp;
}

/** @} */

