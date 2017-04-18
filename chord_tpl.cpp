/**
 * chord_tpl.cpp
 *
 * Implementation of the ChordTpl class
 */

#include "chord_tpl.h"
#include "music_scale.h"


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
    PitchCalculator& pc = PitchCalculator::getInstance();

    for (uint8_t i = 0; i < __mChordQltyTpls[cq].size(); i++) {
        note_t note;
        freq_hz_t pitch;
        note_presense_state_t presenseState = __mChordQltyTpls[cq][i];

        switch(presenseState) {
            case nps_present:
                note = scale[i];
                break;
            case nps_present_flat:
            case nps_present_sharp:
            {
                int8_t interval = (presenseState == nps_present_flat) ? -1 : 1;
                pitch = pc.noteToPitch(scale[i], OCTAVE_4);
                pitch = pc.getPitchByInterval(pitch, interval);
                note = pc.pitchToNote(pitch);
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

tpl_score_t ChordTpl::__euclideanDistance(pcp_t *pcp)
{
    tpl_score_t sum = 0;

    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        tpl_score_t diff = ((amplitude_t)__mTpl[note - note_Min] - pcp->getPitchCls(note));
        sum += diff * diff;
    }

    return sqrt(sum);
}

tpl_score_t ChordTpl::getPCPScore(pcp_t *pcp)
{
    return __euclideanDistance(pcp);
}

chord_quality_t ChordTpl::getQuality()
{
    return __mQuality;
}
