/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file    music_scale.cpp
 * @brief   Implementation of the MusicScale class methods
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

    scale.reserve(formulaLen * 2 - 1);
    scale.push_back(note);
    for (uint8_t i = 0; i < formulaLen; i++) {
        note = note + formula[i];
        scale.push_back(note);
    }

    scale.insert(scale.end(), scale.begin() + 1, scale.end());

    return scale;
}
