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


typedef enum {
    PCPBUF_ACCEPTING,       // ready to collect more PCPs
    PCPBUF_VECTOR_CHANGE,   // vector change detected
} pcpbuf_state_t;

class PCPBuf {

private:
    std::vector<pcp_t *> mProfiles;
    pcpbuf_state_t mState = PCPBUF_ACCEPTING;

public:
    /**
     * Add new PCP to the buffer
     *
     * @param   pcp   pointer to the PCP to add
     * @return  PCPBUF_ACCEPTING if PCP vector difference is small (i.e. no
     *          chord change detected), PCPBUF_VECTOR_CHANGE otherwise. After
     *          returning PCPBUF_VECTOR_CHANGE flush() must be called before
     *          adding new PCPs.
     */
    pcpbuf_state_t add(pcp_t *pcp);

    bool vectorChange(pcp_t *pcp);

    /**
     * Return combined PCP based on the accumulated data
     */
    pcp_t * getCombinedPCP();

    std::size_t size();

    /**
     * Clean up all accumulated data
     *
     * After flushing the buffer is ready to collect new PCPs
     */
    void flush();
};
