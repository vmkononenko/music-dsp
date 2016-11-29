/**
 * music_scale.h
 *
 * Header file for the class describing music scale
 */

#pragma once

#include "lmtypes.h"

#define NOTES_IN_SCALE  ((uint8_t)7)


class MusicScale {

private:
    note_t  *__mNotesUnsorted;  // array of notes in the original order
    note_t  *__mNotesSorted;    // array of sorted notes to perform quick search
    bool    __mIsMinor;         // indicates if the scale is major or minor

public:

    /**
     * Constructor
     *
     * @param   notes   array of notes scale is built from
     * @param   len     length of the notes array
     * @param   isMinor indicates if the scale is major or minor
     */
    MusicScale(note_t *notes, bool isMinor);

    /**
     * Get main note of the scale (first degree)
     */
    note_t getTonic();
};
