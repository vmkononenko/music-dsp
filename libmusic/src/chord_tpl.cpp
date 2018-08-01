/**
 * chord_tpl.cpp
 *
 * Implementation of the ChordTpl class
 */

#include "chord_tpl.h"
#include "lmhelpers.h"
#include "music_scale.h"


using namespace std;

map<chord_quality_t, vector<note_presense_state_t>> ChordTpl::__mChordQltyTpls = {
    {cq_major,                  {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_minor,                  {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_7th,                    {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_NP}},
    {cq_major_7th,              {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_P,   nps_NP, nps_NP}},
    {cq_minor_7th,              {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_NP}},
    {cq_6th,                    {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_P,  nps_NP,  nps_NP, nps_NP}},
    {cq_minor_6th,              {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_P,  nps_NP,  nps_NP, nps_NP}},
    {cq_diminished,             {nps_P, nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_diminished_7th,         {nps_P, nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_PFF, nps_NP, nps_NP}},
    {cq_half_diminished_7th,    {nps_P, nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_PF,  nps_NP, nps_NP}},
    {cq_9th,                    {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_P }},
    {cq_major_9th,              {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_P,   nps_NP, nps_P }},
    {cq_added_9th,              {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_P }},
    {cq_minor_9th,              {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_P }},
    {cq_suspended_4th,          {nps_P, nps_NP, nps_NP, nps_P,  nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_suspended_2nd,          {nps_P, nps_P,  nps_NP, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_5th,                    {nps_P, nps_NP, nps_NP, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
};


ChordTpl::ChordTpl(note_t rootNote, chord_quality_t cq)
{
    if ((rootNote < note_Min) || (rootNote > note_Max)) {
        throw std::invalid_argument("Invalid note");
    }
    if ((cq < cq_Min) || (cq > cq_Max) ||
        (__mChordQltyTpls.find(cq) == __mChordQltyTpls.end()))
    {
        throw std::invalid_argument("Invalid chord quality");
    }

    __mRootNote = rootNote;
    __mQuality = cq;
    __mTpl.reserve(notes_Total);
    __mTpl.assign(notes_Total, 0);

    __initTpl(rootNote, cq);
}

void ChordTpl::__initTpl(note_t rootNote, chord_quality_t cq)
{
    vector<note_t> scale = MusicScale::getMajorScale(rootNote);

    for (uint8_t i = 0; i < __mChordQltyTpls[cq].size(); i++) {
        note_t note = note_Unknown;
        note_presense_state_t presenseState = __mChordQltyTpls[cq][i];

        switch(presenseState) {
            case nps_present:
                note = scale[i];
                break;
            case nps_present_flat:
            case nps_present_flat_flat:
            case nps_present_sharp:
            {
                int8_t interval;

                if (presenseState == nps_present_flat) {
                    interval = -1;
                } else if (presenseState == nps_present_flat_flat) {
                    interval = -2;
                } else {
                    interval = 1;
                }

                note = scale[i] + interval;

                break;
            }
            default:
                note = note_Unknown;
                break;
        }

        if (note != note_Unknown) {
            __mTpl[note - note_Min] = 1;
        }
    }
}

tpl_score_t ChordTpl::getPCPScore(pcp_t *pcp)
{
    return pcp->euclideanDistance<typeof(__mTpl[0])>(__mTpl);
}

chord_quality_t ChordTpl::getQuality()
{
    return __mQuality;
}
