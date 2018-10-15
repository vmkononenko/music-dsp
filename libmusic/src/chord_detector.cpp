/**
 * @file    chord_detector.cpp
 * @brief   Chord recognition implementation
 * @ingroup libmusic
 * @{
 */

#include <algorithm>
#include <limits>

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

namespace anatomist {

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

    return new FFT(td, samples, samplerate, FREQ_E2, FREQ_C8, true, false);
}

chord_t ChordDetector::GetChordFromFft_(FFT *fft)
{
    if ((fft == nullptr) || !fft->IsPolar()) {
        throw std::invalid_argument("GetChordFromFft_: invalid fft");
    }

    pcp_t *pcp = new PitchClsProfile(fft);
    chord_t chord = tpl_collection_->getBestMatch(pcp);

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
        chord_t c = tpl_collection_->getBestMatch(pcp);
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

#if 0
void ChordDetector::Process_(std::vector<segment_t> *segments,
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

#endif /* 0 */

void ChordDetector::Tune_(log_spectrogram_t &lsg)
{
    UNUSED(lsg);
}

chromagram_t ChordDetector::ChromagramFromSpectrogram_(log_spectrogram_t &lsg,
                                                       QTransform *q_transform)
{
    chromagram_t chromagram;

    for (uint32_t win_idx = 0; win_idx < lsg.size(); win_idx++) {
        chromagram.push_back(PitchClsProfile(lsg[win_idx], q_transform));
    }

    return chromagram;
}

Viterbi::obs_matrix_t ChordDetector::GetScoreMatrix_(chromagram_t &chromagram)
{
    Viterbi::obs_matrix_t score_mtx(chromagram.size());

    for (uint32_t win_idx = 0; win_idx < chromagram.size(); win_idx++) {
        pcp_t *pcp = &chromagram[win_idx];
        tpl_score_t sum;

        for (uint32_t tpl_idx = 0; tpl_idx < tpl_collection_->Size(); tpl_idx++) {
            tpl_score_t score = 1 / tpl_collection_->GetTpl(tpl_idx)->GetScore(pcp);
            score_mtx[win_idx].push_back(score);
            sum += score;
        }

        for (auto & score : score_mtx[win_idx]) {
            score = score / sum;
        }
    }

    return score_mtx;
}

void ChordDetector::Process_(std::vector<segment_t> *segments,
                             amplitude_t *td, uint32_t samples,
                             uint32_t samplerate, ResultsListener *listener,
                             chromagram_t *c)
{
    uint32_t win_size, offset, hop_size;

#ifdef CFG_DYNAMIC_WINDOW
    Envelope *e = new Envelope(td, samples);
    BeatDetector *bd = new BeatDetector(e, sampleRate);

    win_size = bd->getIdxInterval();
    offset = bd->getOffset();

    delete bd;
#else
    win_size = CFG_WINDOW_SIZE;
    offset = 0;
#endif

    log_spectrogram_t lsg;
    Viterbi::obs_matrix_t score_mtx;
    vector<uint32_t> mtx_path;
    uint32_t seg_start_idx = 0, prev_path_idx;
    QTransform *q_transform = new QTransform(samplerate, 36, 1047);
    vector<double> init_p;
    vector<vector<double>> trans_p;
    uint32_t chords_total = tpl_collection_->Size();
    chromagram_t chromagram;

    hop_size = win_size / CFG_HOPS_PER_WINDOW;

    if (listener != nullptr) {
        listener->onPreprocessingProgress(1);
    }

    lsg = q_transform->GetSpectrogram(td, samples, win_size, offset, hop_size);

    Tune_(lsg);

    chromagram = ChromagramFromSpectrogram_(lsg, q_transform);

    if (c != nullptr) {
        *c = chromagram;
        return;
    }

    score_mtx = GetScoreMatrix_(chromagram);

    init_p = vector<double>(chords_total, 1.0 / chords_total);

    for (uint32_t i = 0; i < tpl_collection_->Size(); i++) {
        vector<double> t = vector<double>(chords_total, (1 - 0.1) / chords_total);
        t[i] = 0.1;
        trans_p.push_back(t);
    }

    mtx_path = Viterbi::GetPath(score_mtx, init_p, trans_p);

    if (mtx_path.size() != lsg.size()) {
        throw runtime_error("__getSegments(): mtx_path.size() != lsg.size()");
    }

    prev_path_idx = 0;
    for (uint32_t res = 0; res < mtx_path.size(); res++) {
        if (mtx_path[res] != prev_path_idx) {
            chord_tpl_t *tpl = tpl_collection_->GetTpl(prev_path_idx);
            segment_t segment;

            segment.startIdx = seg_start_idx;
            segment.chord = Chord(tpl->RootNote(), tpl->Quality());

            prev_path_idx = mtx_path[res];
            seg_start_idx++;

            if (listener == nullptr) {
                segments->push_back(segment);
            } else {
                listener->onChordSegmentProcessed(segment, (segment.endIdx / (float)samples));
            }
        }
    }

    delete q_transform;

    if (listener != nullptr) {
        listener->onChordAnalysisFinished();
    }
}

void ChordDetector::getSegments(std::vector<segment_t>& segments,
                                amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    Process_(&segments, timeDomain, samples, sampleRate, nullptr, nullptr);
}

void ChordDetector::getSegments(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate, ResultsListener *listener)
{
    Process_(nullptr, timeDomain, samples, sampleRate, listener, nullptr);
}

pcp_t * ChordDetector::GetPCP(amplitude_t *x, uint32_t samples, uint32_t samplerate)
{
    FFT *fft = GetFft_(x, samples, samplerate);
    pcp_t *pcp = new PitchClsProfile(fft);

    delete fft;

    return pcp;
}

chromagram_t ChordDetector::GetChromagram(amplitude_t *x, uint32_t samples,
                                          uint32_t samplerate)
{
    chromagram_t chromagram;

    Process_(nullptr, x, samples, samplerate, nullptr, &chromagram);

    return chromagram;
}

}

/** @} */

