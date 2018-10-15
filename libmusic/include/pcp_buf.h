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
