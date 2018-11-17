/*
 * chord_tlp_collection.cpp
 *
 * Chord template collection
 */

#include <cfloat>
#include <vector>

#include "chord_tpl_collection.h"
#include "lmtypes.h"


using namespace std;

namespace anatomist {

ChordTplCollection::ChordTplCollection()
{
    __initChordTpls();
}

ChordTplCollection::~ChordTplCollection()
{
    __clearChordTpls();
}

void ChordTplCollection::__clearChordTpls()
{
    for (auto tpl : tpls_) {
        delete tpl;
    }
}

void ChordTplCollection::__initChordTpls()
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
