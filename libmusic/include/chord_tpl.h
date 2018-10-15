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
    static std::map<chord_quality_t, std::vector<note_presense_state_t>> __mChordQltyTpls;

    note_t                  __mRootNote;
    chord_quality_t         __mQuality;
    std::vector<uint8_t>    __mTpl;

    void __initTpl(note_t note, chord_quality_t cq);

public:
    /**
     * Constructor
     */
    ChordTpl(note_t note, chord_quality_t cq);

    /**
     *
     */
    tpl_score_t getPCPScore(pcp_t *pcp);

    chord_quality_t getQuality();

} chord_tpl_t;

}

/** @} */
