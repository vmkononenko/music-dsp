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

#include "config.h"
#include "lmtypes.h"
#include "music_scale.h"

note_t operator+(note_t note, int term)
{
    int tmp = static_cast<int>(note) + term % notes_Total;

    if (tmp > static_cast<int>(note_Max))
        tmp -= note_Max;
    else if (tmp < static_cast<int>(note_Min))
        tmp += note_Max;

    return static_cast<note_t>(tmp);
}

std::ostream& operator<<(std::ostream& os, const note_t& n)
{
    std::map<note_t, std::string> n2sMap;

    n2sMap[note_C]          = "C";
    n2sMap[note_C_sharp]    = "C#";
    n2sMap[note_D]          = "D";
    n2sMap[note_D_sharp]    = "D#";
    n2sMap[note_E]          = "E";
    n2sMap[note_F]          = "F";
    n2sMap[note_F_sharp]    = "F#";
    n2sMap[note_G]          = "G";
    n2sMap[note_G_sharp]    = "G#";
    n2sMap[note_A]          = "A";
    n2sMap[note_A_sharp]    = "A#";
    n2sMap[note_B]          = "B";
#if defined(CFG_HARTE_SYNTAX) && CFG_HARTE_SYNTAX == 1
    n2sMap[note_Unknown]    = "N";
#else
    n2sMap[note_Unknown]    = "??";
#endif /* CFG_HARTE_SYNTAX */

    os << n2sMap[n];

    return os;
}

std::ostream& operator<<(std::ostream& os, const chord_quality_t& q)
{
    std::map<chord_quality_t, std::string> q2sMap;

#if defined(CFG_HARTE_SYNTAX) && CFG_HARTE_SYNTAX == 1
    q2sMap[cq_maj]          = "maj";
    q2sMap[cq_min]          = "min";
    q2sMap[cq_5]            = "5";
    q2sMap[cq_7]            = "7";
    q2sMap[cq_maj7]         = "maj7";
    q2sMap[cq_min7]         = "min7";
    q2sMap[cq_sus2]         = "sus2";
    q2sMap[cq_sus4]         = "sus4";
    q2sMap[cq_hdim7]        = "hdim7";
    q2sMap[cq_aug]          = "aug";
    q2sMap[cq_dim]          = "dim";
    q2sMap[cq_dim7]         = "dim7";
    q2sMap[cq_maj_add9]     = "maj(9)";
    q2sMap[cq_min_add9]     = "min(9)";
    q2sMap[cq_maj6]         = "maj6";
    q2sMap[cq_min6]         = "min6";
    q2sMap[cq_maj9]         = "maj9";
    q2sMap[cq_min9]         = "min9";
    q2sMap[cq_maj_add11]    = "maj(11)";
    q2sMap[cq_7_add9sharp]  = "7(#9)";
    q2sMap[cq_9]            = "9";
    q2sMap[cq_aug7]         = "aug7";
    q2sMap[cq_maj11]        = "maj11";
    q2sMap[cq_min11]        = "min11";
    q2sMap[cq_maj13]        = "maj13";
    q2sMap[cq_min13]        = "min13";
#else
    q2sMap[cq_maj]          = "";
    q2sMap[cq_min]          = "m";
    q2sMap[cq_5]            = "5";
    q2sMap[cq_7]            = "7";
    q2sMap[cq_maj7]         = "maj7";
    q2sMap[cq_min7]         = "m7";
    q2sMap[cq_sus2]         = "sus2";
    q2sMap[cq_sus4]         = "sus4";
    q2sMap[cq_hdim7]        = "m7b5";
    q2sMap[cq_aug]          = "aug";
    q2sMap[cq_dim]          = "dim";
    q2sMap[cq_dim7]         = "dim7";
    q2sMap[cq_maj_add9]     = "add9";
    q2sMap[cq_min_add9]     = "m(add9)";
    q2sMap[cq_maj6]         = "6";
    q2sMap[cq_min6]         = "m6";
    q2sMap[cq_maj9]         = "maj9";
    q2sMap[cq_min9]         = "m9";
    q2sMap[cq_maj_add11]    = "add11";
    q2sMap[cq_7_add9sharp]  = "7#9";
    q2sMap[cq_9]            = "9";
    q2sMap[cq_aug7]         = "aug7";
    q2sMap[cq_maj11]        = "maj11";
    q2sMap[cq_min11]        = "m11";
    q2sMap[cq_maj13]        = "maj13";
    q2sMap[cq_min13]        = "m13";
#endif /*CFG_HARTE_SYNTAX*/

    os << q2sMap[q];

    return os;
}

static std::map<chord_quality_t, const std::vector<std::pair<size_t, int>>> pctpls = {
    {cq_maj,           {{2, 0}, {4, 0}                             }},
    {cq_min,           {{2,-1}, {4, 0}                             }},
    {cq_5,             {{4, 0}                                     }},
    {cq_7,             {{2, 0}, {4, 0}, {6,-1}                     }},
    {cq_maj7,          {{2, 0}, {4, 0}, {6, 0}                     }},
    {cq_min7,          {{2,-1}, {4, 0}, {6,-1}                     }},
    {cq_sus2,          {{1, 0}, {4, 0}                             }},
    {cq_sus4,          {{3, 0}, {4, 0}                             }},
    {cq_hdim7,         {{2,-1}, {4,-1}, {6,-1}                     }},
    {cq_aug,           {{2, 0}, {4,+1}                             }},
    {cq_dim,           {{2,-1}, {4,-1}                             }},
    {cq_dim7,          {{2,-1}, {4,-1}, {6,-2}                     }},
    {cq_maj_add9,      {{2, 0}, {4, 0}, {8, 0}                     }},
    {cq_min_add9,      {{2,-1}, {4, 0}, {8, 0}                     }},
    {cq_maj6,          {{2, 0}, {4, 0}, {5, 0}                     }},
    {cq_min6,          {{2,-1}, {4, 0}, {5, 0}                     }},
    {cq_maj9,          {{2, 0}, {4, 0}, {6, 0}, {8, 0}             }},
    {cq_min9,          {{2,-1}, {4, 0}, {6,-1}, {8, 0}             }},
    {cq_maj_add11,     {{2, 0}, {4, 0}, {10,0}                     }},
    {cq_7_add9sharp,   {{2, 0}, {4, 0}, {6,-1}, {8,+1}             }},
    {cq_9,             {{2, 0}, {4, 0}, {6,-1}, {8, 0}             }},
    {cq_aug7,          {{2, 0}, {4,+1}, {6,-1}                     }},
    {cq_maj11,         {{2, 0}, {4, 0}, {6, 0}, {8, 0},{10,0}      }},
    {cq_min11,         {{2,-1}, {4, 0}, {6,-1}, {8, 0},{10,0}      }},
    {cq_maj13,         {{2, 0}, {4, 0}, {6, 0}, {8, 0},{10,0},{12,0}}},
    {cq_min13,         {{2,-1}, {4, 0}, {6,-1}, {8, 0},{10,0},{12,0}}},
};

pcset_t make_pcset(note_t root, chord_quality_t cq)
{
    auto scale = MusicScale::getMajorScale(root);
    pcset_t pcset = { scale[0] };
    const std::vector<std::pair<size_t, int>> & pctpl = pctpls[cq];
    for (auto & E: pctpl)
        pcset.insert(scale[E.first] + E.second);
    return pcset;
}
