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

    note_t                  root_note_;
    note_t                  bass_note_ = note_Unknown;
    chord_quality_t         chord_quality_;
    std::vector<uint8_t>    tpl_;

    void __initTpl(note_t note, chord_quality_t cq, uint8_t slash_subtype);

    /**
     * Initialize template instance for N chord
     */
    void InitN_();

public:
    /**
     * Constructor
     */
    ChordTpl(note_t note, chord_quality_t cq, uint8_t slash_subtype);

    tpl_score_t GetScore(pcp_t *pcp);

    tpl_score_t GetSalience(pcp_t *pcp);

    note_t RootNote();

    note_t BassNote();

    chord_quality_t Quality();

    static size_t SlashSubtypesCnt(chord_quality_t q);

} chord_tpl_t;

}

/** @} */
