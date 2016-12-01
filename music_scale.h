/**
 * music_scale.h
 *
 * Header file for the class describing music scale
 */

#pragma once

#include <vector>

#include "lmtypes.h"


typedef std::pair<note_t, uint8_t> note_pair_t;

class MusicScale {

private:
    std::vector<note_pair_t>  __mNotes;
    note_t __mTonic;    // main note of the scale
    bool __mIsMinor;    // indicates if the scale is major or minor

    /**
     * Comparison function for std::sort
     *
     * @param   p1, p2  pair of <note, sequence number in scale>
     * @return true if note numeric value of p1 is < than note in p2
     */
    static bool __sortByNote(note_pair_t p1, note_pair_t p2);

public:

    /**
     * Constructor
     *
     * @param   notes   array of notes scale is built from
     * @param   len     length of the notes array
     * @param   isMinor indicates if the scale is major or minor
     */
    MusicScale(std::vector<note_t> &notes, bool isMinor);

    /**
     * Get main note of the scale (first degree)
     */
    note_t getTonic();

    /**
     * Check if note is present in scale
     *
     * @param   note    note to be checked
     * @return  true if note is present
     */
    bool hasNote(note_t note);
};
