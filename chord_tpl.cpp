/**
 * chord_tpl.cpp
 *
 * Implementation of the ChordTpl class
 */

#include "chord_tpl.h"


using namespace std;

map<chord_quality_t, vector<note_presense_state_t>> ChordTpl::__mChordQltyTpls = {
    {cq_Major,      {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP}},
    {cq_Minor,      {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_NP}},
    {cq_Major_7,    {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_P}},
    {cq_Minor_7,    {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_PF}},
    {cq_Dominant_7, {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_PF}},
    {cq_Major_6,    {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_P,  nps_NP}},
    {cq_Minor_6,    {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_P,  nps_NP}},
    {cq_Diminished, {nps_P, nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_NP}},
    {cq_Augmented,  {nps_P, nps_NP, nps_P,  nps_NP, nps_PS, nps_NP, nps_NP}}
};


ChordTpl::ChordTpl(note_t note, chord_quality_t cq)
{
    if ((note < note_Min) || (note > note_Max)) {
        throw std::invalid_argument("Invalid note");
    }
    if ((cq < cq_Min) || (cq > cq_Max) ||
        (__mChordQltyTpls.find(cq) == __mChordQltyTpls.end()))
    {
        throw std::invalid_argument("Invalid chord quality");
    }

    __mNote = note;
    __mQuality = cq;
    __mTpl->reserve(__mChordQltyTpls.at(cq_Major).size());
}
