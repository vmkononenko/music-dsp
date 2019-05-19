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
    void InitFromHmm_();
    void InitTheoretical_();

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
