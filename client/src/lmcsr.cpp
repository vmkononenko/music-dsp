/*
 * Copyright 2021 Sergiy Kibrik
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

#include <iostream>
#include <iomanip>
#include <sndfile.h>

#include "lmhelpers.h"
#include "chord_detector.h"

using namespace anatomist;
using namespace std;

static inline bool empty(const segment_t &s)
{
    return s.startIdx > s.endIdx;
}

static segment_t intersection(const segment_t &ls, const segment_t &rs)
{
    segment_t res;
    res.startIdx = max(ls.startIdx, rs.startIdx);
    res.endIdx = min(ls.endIdx, rs.endIdx);
    return res;
}

class CSRListener : public ChordDetector::ResultsListener {
    public:
         CSRListener(vector<segment_t> &sl) : seglist_(sl), matchlist_()
         {
         }
         size_t getMatchDuration() const {
             size_t count = 0;
             for (auto &s: matchlist_)
                 count += s.endIdx - s.startIdx + 1;
             return count;
         }
         virtual void onPreprocessingProgress(float progress)
         {
             UNUSED(progress);
         }
         virtual void onChordAnalysisFinished()
         {
         }
         virtual void onChordSegmentProcessed(segment_t &seg, float progress)
         {
             UNUSED(progress);
             for (auto &s : seglist_) {
                  segment_t k = intersection(s, seg);
                  if (empty(k))
                      continue;
                  if (s.chord.match(seg.chord))
                      matchlist_.push_back(k);
             }
         }
    private:
        vector<segment_t> seglist_;
        vector<segment_t> matchlist_;
};

static void usage()
{
    cerr << "usage: lmcsr <audio file> <text file>" << endl;
}

static td_t toMono(const amplitude_t *data, size_t frames, size_t ch)
{
    td_t channel(frames);
    auto i = channel.begin();
    auto t = data - ch;
    while (i != channel.end())
        *i++ = *(t += ch);
    return channel;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        usage();
        return -1;
    }

    SF_INFO sfinfo = {};
    SNDFILE *sf = sf_open(argv[1], SFM_READ, &sfinfo);
    if (!sf) {
        cerr << "SF:" << sf_strerror(nullptr) << endl;
        return sf_error(nullptr);
    }

    unique_ptr<amplitude_t> td_in(new double[sfinfo.frames * sfinfo.channels]);
    sf_readf_double(sf, td_in.get(), sfinfo.frames);
    sf_close(sf);
    sf = nullptr;
    ifstream ifs(argv[2], ifstream::in);
    if (!ifs.is_open()) {
        cerr << "failed to open " << argv[2] << endl;
        return -1;
    }

    double startSec, endSec;
    string chord_name;
    vector<segment_t> seglist;
    while(ifs >> startSec >> endSec >> chord_name) {
        segment_t seg;
        seg.startIdx = uint32_t(startSec * sfinfo.samplerate);
        seg.endIdx = uint32_t(endSec * sfinfo.samplerate);
	if (chord_name != "N")
            seg.chord = Chord(chord_name);
        seglist.push_back(seg);
    }
    ifs.close();

    CSRListener csr(seglist);
    td_t td = toMono(td_in.get(), sfinfo.frames, sfinfo.channels);
    ChordDetector cd;
    cd.getSegments(td.data(), td.size(), sfinfo.samplerate, &csr);
    cout << fixed << setprecision(2) << double(csr.getMatchDuration()) / td.size() << endl;
    return 0;
}
