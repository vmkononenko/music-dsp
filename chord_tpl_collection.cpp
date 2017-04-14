/*
 * chord_tlp_collection.cpp
 *
 * Chord template collection
 */

#include <vector>

#include "chord_tpl_collection.h"
#include "lmtypes.h"


using namespace std;

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
    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;
        for (auto tpl : __mChordTpls[note]) {
            delete tpl;
        }
        __mChordTpls[note].clear();
    }
    __mChordTpls.clear();
}

void ChordTplCollection::__initChordTpls()
{
    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        for (int q = cq_Min; q <= cq_Max; q++) {
            chord_quality_t cq = (chord_quality_t)cq;
            chord_tpl_t *tpl = new ChordTpl(note, cq);
            __mChordTpls[note].push_back(tpl);
        }
    }
}
