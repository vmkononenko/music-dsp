/**
 * pcp_buf.cpp
 *
 * PCP buffer implementation
 */

#include "config.h"
#include "pcp_buf.h"


using namespace std;

bool PCPBuf::vectorChange(pcp_t *pcp)
{
    pcp_t *prev = mProfiles.empty() ? nullptr : mProfiles.back();

    if ((prev == nullptr) || ((pcp->euclideanDistance(*prev)) < CFG_PCP_CHANGE_THRESHOLD)) {
        return false;
    }

    return true;
}

pcpbuf_state_t PCPBuf::add(pcp_t *pcp)
{
    if (mState != PCPBUF_ACCEPTING) {
        throw logic_error("Not accepting new data - call flush()");
    }

    if (vectorChange(pcp)) {
        mState = PCPBUF_VECTOR_CHANGE;
    } else {
        mProfiles.push_back(pcp);
    }

    return mState;
}

pcp_t * PCPBuf::getCombinedPCP()
{
    pcp_t *ret = new PitchClsProfile();

    for (auto pcp : mProfiles) {
        *ret += *pcp;
    }

    *ret /= mProfiles.size();

    return ret;
}

size_t PCPBuf::size()
{
    return mProfiles.size();
}

void PCPBuf::flush()
{
    for (auto pcp : mProfiles) {
        delete pcp;
    }

    mProfiles.clear();
    mState = PCPBUF_ACCEPTING;
}
