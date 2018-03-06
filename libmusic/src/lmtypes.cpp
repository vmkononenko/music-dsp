#include "lmtypes.h"


note_t& operator+(note_t& note, int32_t term)
{
    int32_t tmp = static_cast<int32_t>(note) + term;

    if (note > note_Max) {
        tmp -= note_Max;
    } else if (note < note_Min) {
        tmp += note_Max;
    }

    note = static_cast<note_t>(tmp);

    return note;
}
