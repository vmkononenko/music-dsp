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
    uint8_t formulaLen = sizeof(formula) / sizeof(formula[0]);
    vector<note_t> scale;
    note_t note = rootNote;

    scale.reserve(formulaLen * 2);
    scale.push_back(note);
    for (uint8_t i = 0; i < formulaLen; i++) {
        note = note + formula[i];
        scale.push_back(note);
    }

    scale.insert(scale.end(), scale.begin(), scale.end());

    return scale;
}
