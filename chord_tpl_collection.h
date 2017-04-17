/*
 * chord_tlp_collection.h
 *
 * Chord template collection
 */

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "lmtypes.h"
#include "chord_tpl.h"
#include "pitch_cls_profile.h"


class ChordTplCollection {

private:
    std::map<note_t, std::vector <chord_tpl_t *>>   __mChordTpls;

    void __clearChordTpls();
    void __initChordTpls();

public:
    /**
     * Constructor
     */
    ChordTplCollection();

    /**
     * Destructor
     */
    ~ChordTplCollection();

    /**
     *
     * @param pcp
     * @return
     */
    chord_t getBestMatch(pcp_t *pcp);
};
