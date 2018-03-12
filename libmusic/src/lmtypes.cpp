#include "lmtypes.h"


note_t& operator+(note_t& note, int32_t term)
{
    int32_t tmp = static_cast<int32_t>(note) + term;

    if (tmp > note_Max) {
        tmp -= note_Max;
    } else if (note < note_Min) {
        tmp += note_Max;
    }

    note = static_cast<note_t>(tmp);

    return note;
}

std::ostream& operator<<(std::ostream& os, const note_t& n)
{
    std::map<note_t, std::string> n2sMap;

    n2sMap[note_C]          = "C";
    n2sMap[note_C_sharp]    = "C#";
    n2sMap[note_D]          = "D";
    n2sMap[note_D_sharp]    = "D#";
    n2sMap[note_E]          = "E";
    n2sMap[note_F]          = "F";
    n2sMap[note_F_sharp]    = "F#";
    n2sMap[note_G]          = "G";
    n2sMap[note_G_sharp]    = "G#";
    n2sMap[note_A]          = "A";
    n2sMap[note_A_sharp]    = "A#";
    n2sMap[note_B]          = "B";
    n2sMap[note_Unknown]    = "??";

    os << n2sMap[n];

    return os;
}

std::ostream& operator<<(std::ostream& os, const chord_quality_t& q)
{
    std::map<chord_quality_t, std::string> q2sMap;

    q2sMap[cq_major]                = "";
    q2sMap[cq_minor]                = "m";
    q2sMap[cq_7th]                  = "7";
    q2sMap[cq_major_7th]            = "maj7";
    q2sMap[cq_minor_7th]            = "m7";
    q2sMap[cq_6th]                  = "6";
    q2sMap[cq_minor_6th]            = "m6";
    q2sMap[cq_diminished]           = "dim";
    q2sMap[cq_diminished_7th]       = "dim7";
    q2sMap[cq_half_diminished_7th]  = "m7b5";
    q2sMap[cq_9th]                  = "9";
    q2sMap[cq_major_9th]            = "maj9";
    q2sMap[cq_added_9th]            = "add9";
    q2sMap[cq_minor_9th]            = "m9";
    q2sMap[cq_suspended_4th]        = "sus4";
    q2sMap[cq_suspended_2nd]        = "sus2";
    q2sMap[cq_5th]                  = "5";

    os << q2sMap[q];

    return os;
}
