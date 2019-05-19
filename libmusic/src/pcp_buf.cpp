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
 * @file     pcp_buf.cpp
 * @brief    PCP buffer implementation
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
