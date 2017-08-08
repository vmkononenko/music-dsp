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
