/**
 * @file        chord_tpl_collection.h
 * @brief       Chord template collection
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "lmtypes.h"
#include "chord_tpl.h"
#include "pitch_cls_profile.h"


namespace anatomist {

class ChordTplCollection {

private:
    std::vector<chord_tpl_t *>  tpls_;

    void ClearChordTpls_();
    void InitChordTpls_();

public:
    /**
     * Constructor
     */
    ChordTplCollection();

    /**
     * Destructor
     */
    ~ChordTplCollection();

    size_t Size();

    chord_tpl_t * GetTpl(uint32_t idx);

    /**
     *
     * @param pcp
     * @return
     */
    chord_t getBestMatch(pcp_t *pcp);

};

}

/** @} */
