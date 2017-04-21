/**
 * music_scale.cpp
 *
 * Implementation of the MusicScale class methods
 */

#include <algorithm>

#include "music_scale.h"

using namespace std;


vector<note_t> MusicScale::getMajorScale(note_t rootNote)
{
    /* whole, whole, half, whole, whole, whole, half */
    uint8_t formula[] = {2, 2, 1, 2, 2, 2, 1};

    vector<note_t> scale;
    PitchCalculator& pc = PitchCalculator::getInstance();
    freq_hz_t pitch;

    scale.push_back(rootNote);
    pitch = pc.noteToPitch(rootNote, OCTAVE_4);
    for (uint8_t i = 0; i < 7; /* size of formula */ i++) {
        pitch = pc.getPitchByInterval(pitch, formula[i]);
        scale.push_back(pc.pitchToNote(pitch));
    }

    return scale;
}
