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
 * @file        pcpbuffer.h
 * @brief       Defines API for PCPBuf (Pitch Class Profile buffer)
 *
 * Accumulates a set of PCPs until chord change is detected or flush()
 * is called.
 *
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "pitch_cls_profile.h"

namespace anatomist {

class PCPBuf {

private:
    std::vector<pcp_t *> mProfiles;

public:
    /**
     * Add new PCP to the buffer
     *
     * @param   pcp   pointer to the PCP to add
     */
    void add(pcp_t *pcp);

    bool vectorChange(pcp_t *pcp);

    /**
     * Return combined PCP based on the accumulated data
     */
    pcp_t * getCombinedPCP();

    std::vector<pcp_t *> getProfiles();

    std::size_t size();

    /**
     * Clean up all accumulated data
     *
     * After flushing the buffer is ready to collect new PCPs
     */
    void flush();
};

}
