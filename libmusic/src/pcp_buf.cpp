/**
 * pcp_buf.cpp
 *
 * PCP buffer implementation
 */

#include "config.h"
#include "pcp_buf.h"


using namespace std;

namespace anatomist {

bool PCPBuf::vectorChange(pcp_t *pcp)
{
    pcp_t *prev = mProfiles.empty() ? nullptr : mProfiles[0];

    if ((prev == nullptr) || ((pcp->euclideanDistance(*prev)) < CFG_PCP_CHANGE_THRESHOLD)) {
        return false;
    }

    return true;
}

void PCPBuf::add(pcp_t *pcp)
{
    if (pcp == nullptr) {
        throw invalid_argument("PCPBuf::add(): pcp is NULL");
    }
    mProfiles.push_back(pcp);
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

std::vector<pcp_t *> PCPBuf::getProfiles()
{
    return mProfiles;
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
}

}
