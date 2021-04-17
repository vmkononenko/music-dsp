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
 * @file        chord_tpl.h
 * @brief       Header file for chord type templates
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <map>
#include <vector>

#include "lmtypes.h"
#include "pitch_cls_profile.h"


namespace anatomist {

typedef enum {
    nps_present,
    nps_present_flat,
    nps_present_flat_flat,
    nps_present_sharp,
    nps_not_present,
    /* introduce acronyms to keep the formulas shorter */
    nps_P   = nps_present,
    nps_PF  = nps_present_flat,
    nps_PFF = nps_present_flat_flat,
    nps_PS  = nps_present_sharp,
    nps_NP  = nps_not_present
} note_presense_state_t;

typedef float tpl_score_t;


typedef class ChordTpl {

private:
    static std::map<chord_quality_t, std::vector<std::vector<note_presense_state_t>>> chord_qlty_tpls_;

    note_t                      root_note_;
    note_t                      bass_note_ = note_Unknown;
    int8_t                      bass_interval_ = -1;
    chord_quality_t             chord_quality_;
    std::vector<amplitude_t>    tpl_;

    void InitTpl_(note_t note, chord_quality_t cq, uint8_t slash_subtype);

    /**
     * Initialize template instance for N chord
     */
    void InitN_();

    /**
     * Post initialization step
     */
    void PostInit_(float boost);

public:
    /**
     * Constructor
     *
     * Used for theoretical template generation
     */
    ChordTpl(note_t note, chord_quality_t cq, uint8_t slash_subtype);

    /**
     * Constructor
     *
     * Used for creating a class instance for HMM training results
     */
    ChordTpl(note_t note, chord_quality_t cq, std::vector<prob_t> &tpl);

    tpl_score_t GetScore(pcp_t *pcp);

    tpl_score_t GetSalience(pcp_t *pcp);

    note_t RootNote();

    note_t BassNote();

    chord_quality_t Quality();

    static size_t SlashSubtypesCnt(chord_quality_t q);

    friend std::ostream& operator<<(std::ostream& os, const ChordTpl& tpl);

} chord_tpl_t;

}

/** @} */
