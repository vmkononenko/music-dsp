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

/*
 * chord_tlp_collection.cpp
 *
 * Chord template collection
 */

#include <cfloat>
#include <vector>

#include "chord_tpl_collection.h"
#include "chord_tpls_hmm.h"
#include "config.h"
#include "lmtypes.h"


using namespace std;

namespace anatomist {

ChordTplCollection::ChordTplCollection()
{
    InitChordTpls_();
}

ChordTplCollection::~ChordTplCollection()
{
    ClearChordTpls_();
}

void ChordTplCollection::ClearChordTpls_()
{
    for (auto tpl : tpls_) {
        delete tpl;
    }
}

void ChordTplCollection::InitChordTpls_()
{
#if (!defined(CHORD_TPLS_HMM_TRAINED) || CHORD_TPLS_HMM_TRAINED == 0) && CFG_USE_HMM_TPLS == 1
#error "Can't use HMM templates because of missing training results"
#elif defined(CHORD_TPLS_HMM_TRAINED) && CHORD_TPLS_HMM_TRAINED == 1 && CFG_USE_HMM_TPLS == 1
    InitFromHmm_();
#else
    InitTheoretical_();
#endif
}

void ChordTplCollection::InitFromHmm_()
{
    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        for (int q = cq_Min; q <= cq_Max; q++) {
            chord_quality_t cq = (chord_quality_t)q;

            chord_tpl_t *tpl = new ChordTpl(note, cq, hmm_training_res[note][cq]);
            tpls_.push_back(tpl);
        }
    }
}

void ChordTplCollection::InitTheoretical_()
{
    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        for (int q = cq_Min; q <= cq_Max; q++) {
            chord_quality_t cq = (chord_quality_t)q;
            size_t subtypes = ChordTpl::SlashSubtypesCnt(cq);
            for (uint8_t s = 0; s < subtypes; s++) {
                chord_tpl_t *tpl = new ChordTpl(note, cq, s);
                tpls_.push_back(tpl);
            }
        }
    }

    tpls_.push_back(new ChordTpl(note_Unknown, cq_unknown, 0));
}

size_t ChordTplCollection::Size()
{
    return tpls_.size();
}

chord_tpl_t * ChordTplCollection::GetTpl(uint32_t idx)
{
    if (idx >= tpls_.size()) {
        throw invalid_argument("ChordTplCollection::GetTpl(): bad index");
    }

    return tpls_[idx];
}

chord_t ChordTplCollection::getBestMatch(pcp_t *pcp)
{
    tpl_score_t scoreMin = FLT_MAX;
    note_t winningNote = note_Unknown;
    chord_quality_t winningQuality = cq_major;
    tpl_score_t score;

    for (const auto tpl : tpls_) {
        score = tpl->GetScore(pcp);

       if (score > scoreMin) { continue; }

       scoreMin = score;
       winningNote = tpl->RootNote();
       winningQuality = tpl->Quality();
    }

    return Chord(winningNote, winningQuality);
}

}
