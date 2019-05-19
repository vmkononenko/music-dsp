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
 * @file        music_scale.h
 * @brief       Header file for the class describing music scale
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"
#include "pitch_calculator.h"


class MusicScale {

public:
    /**
     * Build major scale from the given root note
     *
     * @param rootNote the note to build from
     * @return built scale
     */
    static std::vector<note_t> getMajorScale(note_t rootNote);
};

/** @} */
