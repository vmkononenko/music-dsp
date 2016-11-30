/**
 * music_scale.cpp
 *
 * Implementation of the MusicScale class methods
 */

#include <algorithm>

#include "music_scale.h"

using namespace std;

MusicScale::MusicScale(vector<note_t> &notes, bool isMinor) : __mIsMinor(isMinor)
{
    __mTonic = notes[0];

    for (uint8_t i = 0; i < notes.size(); i++) {
        __mNotes.push_back(make_pair(notes[i], i));
    }

    sort(__mNotes.begin(), __mNotes.end(), __sortByNote);
}

bool MusicScale::__sortByNote(note_pair_t p1, note_pair_t p2)
{
    return (p1.first < p2.first);
}

note_t MusicScale::getTonic()
{
    return __mTonic;
}

bool MusicScale::hasNote(note_t note)
{
    uint8_t start = 0, end = __mNotes.size() - 1, mid;
    note_t tmpNote;

    while (start <= end) {
        mid = start + (end - start) / 2;
        tmpNote = __mNotes[mid].first;

        if (tmpNote < note) {
            start = mid + 1;
        } else if (tmpNote > note) {
            end = mid - 1;
        } else {
            return true;
        }
    }

    return false;
}
