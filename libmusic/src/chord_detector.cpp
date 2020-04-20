/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

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
#include "cqt_wrapper.h"
#include "envelope.h"
#include "fft_wrapper.h"
#include "lmhelpers.h"
#include "lmlogger.h"
#include "tft.h"
#include "window_functions.h"

#define FREQ_E1     ((freq_hz_t)41.203)
#define FREQ_C6     ((freq_hz_t)1046.5)
#define FREQ_E2     ((freq_hz_t)82.4)
#define FREQ_C8     ((freq_hz_t)4186)

#define LOG_TAG "AA.ChordDetector"

using namespace std;

namespace anatomist {

ChordDetector::ChordDetector()
{
    LOGMSG_D(LOG_TAG, "Using window size %u, FFT size %u and %s window function",
             CFG_WINDOW_SIZE, CFG_FFT_SIZE, WindowFunctions::toString(CFG_WINDOW_FUNC));

    tpl_collection_ = new ChordTplCollection();
}

ChordDetector::~ChordDetector()
{
    delete tpl_collection_;
}

FFT * ChordDetector::GetFft_(td_t &td, uint32_t samplerate)
{
    if ((td.size() == 0) || (td.size() > CFG_FFT_SIZE) || (samplerate == 0))
    {
        throw invalid_argument("__getFftResults() invalid argument");
    }

    WindowFunctions::applyDefault(td);

    return new FFT(td, samplerate, FREQ_E2, FREQ_C8);
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

chord_t ChordDetector::getChord(amplitude_t *x, uint32_t samples,
                                uint32_t samplerate)
{
    td_t td(x, x + samples);
    FFT *fft = GetFft_(td, samplerate);
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

float ChordDetector::Tune_(tft_t *tft)
{
    UNUSED(tft);
    return 0;
}

chromagram_t ChordDetector::ChromagramFromSpectrogram_(tft_t *tft)
{
    log_spectrogram_t lsg = tft->GetSpectrogram();
    chromagram_t chromagram;

    for (uint32_t win_idx = 0; win_idx < lsg.size(); win_idx++) {
        chromagram.push_back(PitchClsProfile(lsg[win_idx], tft));
    }

    return chromagram;
}

#if 0
Viterbi::obs_matrix_t ChordDetector::GetScoreMatrix_(chromagram_t &chromagram)
{
    Viterbi::obs_matrix_t score_mtx(chromagram.size());

    for (uint32_t win_idx = 0; win_idx < chromagram.size(); win_idx++) {
        pcp_t *pcp = &chromagram[win_idx];
        tpl_score_t sum = 0;

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
#else
Viterbi::prob_matrix_t ChordDetector::GetScoreMatrix_(chromagram_t &chromagram)
{
    Viterbi::prob_matrix_t score_mtx(chromagram.size());

    for (uint32_t win_idx = 0; win_idx < chromagram.size(); win_idx++) {
        pcp_t *pcp = &chromagram[win_idx];
        tpl_score_t sum = 0;

        for (uint32_t tpl_idx = 0; tpl_idx < tpl_collection_->Size(); tpl_idx++) {
            tpl_score_t score = tpl_collection_->GetTpl(tpl_idx)->GetScore(pcp);

            if (score < 0) {
                score = 0;
            }

            if (tpl_idx == tpl_collection_->Size() - 1) {
                score *= 0.7;
            }

            score = pow(1.3, score);
            score_mtx[win_idx].push_back(score);
            sum += score;
        }

        if (sum > 0) {
            for (auto & score : score_mtx[win_idx]) {
                score = score / sum;
            }
        } else {
            score_mtx[win_idx][score_mtx[win_idx].size() - 1] = 1;
        }
    }

    return score_mtx;
}
#endif /* 0 */

void ChordDetector::Process_(vector<segment_t> *segments,
                             vector<amplitude_t> &td, uint32_t samplerate,
                             ResultsListener *listener, chromagram_t *c)
{
    uint32_t win_size, offset;

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

    uint32_t hop_size = win_size / CFG_HOPS_PER_WINDOW;
    Viterbi::prob_matrix_t score_mtx;
    vector<uint32_t> mtx_path;
    uint32_t seg_start_idx = 0;
#if !defined(CFG_TFT_TYPE) || (CFG_TFT_TYPE == TFT_TYPE_FFT)
    tft_t *tft = new FFTWrapper(FREQ_E1, FREQ_C6, samplerate, win_size, hop_size);
#else
    tft_t *tft = new CQTWrapper(FREQ_E1, FREQ_C6, samplerate, win_size, hop_size);
#endif
    vector<double> init_p;
    Viterbi::prob_matrix_t trans_p;
    uint32_t chords_total = tpl_collection_->Size();
    chromagram_t chromagram;

    if (listener != nullptr) {
        listener->onPreprocessingProgress(1);
    }

    tft->Process(td, offset);

    Tune_(tft);

    chromagram = ChromagramFromSpectrogram_(tft);

    if (c != nullptr) {
        *c = chromagram;
        return;
    }

    score_mtx = GetScoreMatrix_(chromagram);

    init_p = vector<double>(chords_total, 0);
    init_p[init_p.size() - 1] = 1;

    for (uint32_t i = 0; i < tpl_collection_->Size(); i++) {
#ifdef CFG_CHORD_SELF_TRANSITION_P
        double self_trans_p = CFG_CHORD_SELF_TRANSITION_P;
#else
        double self_trans_p = 1 / chords_total;
#endif /* CFG_CHORD_SELF_TRANSITION_P */
        double trans_other_p = (1 - self_trans_p) / (chords_total - (self_trans_p == 0 ? 0 : 1));
        vector<double> t = vector<double>(chords_total, trans_other_p);
        if (self_trans_p != 0) {
            if (self_trans_p < trans_other_p) {
                throw runtime_error("Self-transition probability is less than "
                        "transition probability to any other chord");
            }
            t[i] = self_trans_p;
        }
        trans_p.push_back(t);
    }

    mtx_path = Viterbi::GetPath(init_p, score_mtx, trans_p);

    if (mtx_path.size() != chromagram.size()) {
        throw runtime_error("__getSegments(): mtx_path.size() != chromagram.size()");
    }

    for (uint32_t res = 1; res < mtx_path.size(); res++) {
        if (mtx_path[res] != mtx_path[seg_start_idx] || res == mtx_path.size() - 1) {
            chord_tpl_t *tpl = tpl_collection_->GetTpl(mtx_path[seg_start_idx]);
            segment_t segment;

            segment.startIdx = seg_start_idx * tft->SpectrogramInterval();
            segment.endIdx = min(res * tft->SpectrogramInterval(), static_cast<uint32_t>(td.size() - 1));
            segment.chord = Chord(tpl->RootNote(), tpl->Quality());
            segment.silence = false;

            seg_start_idx = res;

            if (listener == nullptr) {
                segments->push_back(segment);
            } else {
                listener->onChordSegmentProcessed(segment, (segment.endIdx / (float)td.size()));
            }
        }
    }

    delete tft;

    if (listener != nullptr) {
        listener->onChordAnalysisFinished();
    }
}

void ChordDetector::getSegments(std::vector<segment_t>& segments,
                                amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate)
{
    td_t td(timeDomain, timeDomain + samples);
    Process_(&segments, td, sampleRate, nullptr, nullptr);
}

void ChordDetector::getSegments(amplitude_t *timeDomain, uint32_t samples,
                                uint32_t sampleRate, ResultsListener *listener)
{
    td_t td(timeDomain, timeDomain + samples);
    Process_(nullptr, td, sampleRate, listener, nullptr);
}

pcp_t * ChordDetector::GetPCP(amplitude_t *x, uint32_t samples, uint32_t samplerate)
{
    td_t td(x, x + samples);
    FFT *fft = GetFft_(td, samplerate);
    pcp_t *pcp = new PitchClsProfile(fft);

    delete fft;

    return pcp;
}

chromagram_t ChordDetector::GetChromagram(amplitude_t *x, uint32_t samples,
                                          uint32_t samplerate)
{
    chromagram_t chromagram;
    td_t td(x, x + samples);

    Process_(nullptr, td, samplerate, nullptr, &chromagram);

    return chromagram;
}

}

/** @} */

