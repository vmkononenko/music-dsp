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
 * chord_tpl.cpp
 *
 * Implementation of the ChordTpl class
 */

#include "chord_tpl.h"
#include "lmhelpers.h"
#include "lmtypes.h"
#include "music_scale.h"


using namespace std;

namespace anatomist {

std::map<chord_quality_t, std::vector<std::vector<note_presense_state_t>>> ChordTpl::chord_qlty_tpls_ = {
    {cq_major,                  {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP},
                                    {nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP},
                                    {nps_NP, nps_NP, nps_P,  nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP},
                                    {nps_NP, nps_NP, nps_NP, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}
                                }},
    {cq_minor,                  {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP},
                                    {nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}
                                }},
    {cq_7th,                    {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_NP},
                                    {nps_NP, nps_NP, nps_P,  nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_NP}
                                }},
    {cq_major_7th,              {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_P,   nps_NP, nps_NP}
                                }},
    {cq_minor_7th,              {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_NP}
                                }},
    {cq_6th,                    {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_P,  nps_NP, nps_P,  nps_P,  nps_NP,  nps_NP, nps_NP}
                                }},
    {cq_minor_6th,              {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_PF, nps_NP, nps_P,  nps_P,  nps_NP,  nps_NP, nps_NP}
                                }},
    {cq_diminished,             {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_NP,  nps_NP, nps_NP}
                                }},
    {cq_half_diminished_7th,    {
                                    {nps_P,  nps_NP, nps_NP, nps_NP, nps_NP, nps_NP, nps_NP,  nps_NP, nps_NP,
                                     nps_P,  nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_PF,  nps_NP, nps_NP}
                                }},
#if 0
    {cq_diminished_7th,         {nps_P, nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_PFF, nps_NP, nps_NP}},
    {cq_half_diminished_7th,    {nps_P, nps_NP, nps_PF, nps_NP, nps_PF, nps_NP, nps_PF,  nps_NP, nps_NP}},
    {cq_9th,                    {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_P }},
    {cq_major_9th,              {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_P,   nps_NP, nps_P }},
    {cq_added_9th,              {nps_P, nps_NP, nps_P,  nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_P }},
    {cq_minor_9th,              {nps_P, nps_NP, nps_PF, nps_NP, nps_P,  nps_NP, nps_PF,  nps_NP, nps_P }},
    {cq_suspended_4th,          {nps_P, nps_NP, nps_NP, nps_P,  nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_suspended_2nd,          {nps_P, nps_P,  nps_NP, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
    {cq_5th,                    {nps_P, nps_NP, nps_NP, nps_NP, nps_P,  nps_NP, nps_NP,  nps_NP, nps_NP}},
#endif
};


ChordTpl::ChordTpl(note_t root_note, chord_quality_t cq, uint8_t slash_subtype) :
        root_note_(root_note), chord_quality_(cq)
{
    if ((root_note < note_Min) || (root_note > note_Max)) {
        if ((root_note == note_Unknown) && (cq == cq_unknown)) {
            InitN_();
            return;
        } else {
            throw std::invalid_argument("ChordTpl(): Invalid note");
        }
    }
    if ((cq < cq_Min) || (cq > cq_Max) ||
        (chord_qlty_tpls_.find(cq) == chord_qlty_tpls_.end()))
    {
        throw std::invalid_argument("ChordTpl(): Invalid chord quality");
    }
    if (slash_subtype >= chord_qlty_tpls_[cq].size()) {
        throw std::invalid_argument("ChordTpl(): Invalid slash subtype");
    }

    InitTpl_(root_note, cq, slash_subtype);
}

ChordTpl::ChordTpl(note_t root_note, chord_quality_t cq, std::vector<prob_t> &tpl) :
        root_note_(root_note), chord_quality_(cq), tpl_(tpl)
{
    if ((root_note < note_Min) || (root_note > note_Max)) {
        throw std::invalid_argument("ChordTpl(): Invalid note");
    }

    if ((cq < cq_Min) || (cq > cq_Max)) {
        throw std::invalid_argument("ChordTpl(): Invalid chord quality");
    }
}

void ChordTpl::InitTpl_(note_t root_note, chord_quality_t cq, uint8_t ss)
{
    vector<note_t> scale = MusicScale::getMajorScale(root_note);
    vector<note_presense_state_t> *qt = &chord_qlty_tpls_[cq][ss];

    tpl_.resize(notes_Total * 2, 0);

    for (uint8_t i = 0; i < qt->size(); i++) {
        note_t note = note_Unknown;
        note_presense_state_t presenseState = (*qt)[i];
        uint8_t treble_offset = notes_Total;
        uint8_t scale_idx = i % (qt->size() / 2);

        switch(presenseState) {
            case nps_present:
                note = scale[scale_idx];
                break;
            case nps_present_flat:
            case nps_present_flat_flat:
            case nps_present_sharp:
            {
                int8_t interval;

                if (presenseState == nps_present_flat) {
                    interval = -1;
                } else if (presenseState == nps_present_flat_flat) {
                    interval = -2;
                } else {
                    interval = 1;
                }

                note = scale[scale_idx] + interval;

                break;
            }
            default:
                note = note_Unknown;
                break;
        }

        if (note != note_Unknown) {
            // the first half of a template is bass and the second is treble
            if (i < qt->size() / 2) {
                bass_note_ = note;
                treble_offset = 0;
            }
            tpl_[note - note_Min + treble_offset] = 1;
        }
    }

    for (uint8_t i = 0; i < tpl_.size() / 2; i++) {
        if (tpl_[i] != 1 && tpl_[i + notes_Total] == 1) {
            tpl_[i] = 0.5;
        }
    }

    PostInit_(1.0f);
}

void ChordTpl::InitN_()
{
    typeof(tpl_) treble(notes_Total, 1);
    tpl_.resize(notes_Total, 0.5);
    tpl_.insert(tpl_.end(), treble.begin(), treble.end());

    PostInit_(1.1f);
}

void ChordTpl::PostInit_(float boost)
{
    float stand = 0;

    for (const auto & val : tpl_) {
        stand += pow(abs(val), 2.0) / tpl_.size();
    }

    stand = powf(stand, 1.0f / 2.0) / boost;

    for (auto & val : tpl_) {
        val /= stand;
    }
}

tpl_score_t ChordTpl::GetScore(pcp_t *pcp)
{
    if (pcp->size() == tpl_.size()) {
        return pcp->sumProduct(tpl_);
    } else if (pcp->size() == tpl_.size() / 2) { /* no separation between bass and treble */
        vector<typeof(tpl_[0])> treble(tpl_.begin() + notes_Total, tpl_.end());
        return pcp->euclideanDistance<typeof(tpl_[0])>(treble);
    } else {
        throw invalid_argument("Incompatible PCP size");
    }
}

note_t ChordTpl::RootNote()
{
    return root_note_;
}

note_t ChordTpl::BassNote()
{
    return bass_note_;
}

chord_quality_t ChordTpl::Quality()
{
    return chord_quality_;
}

size_t ChordTpl::SlashSubtypesCnt(chord_quality_t q)
{
    if ((q < cq_Min) || (q > cq_Max) ||
        (chord_qlty_tpls_.find(q) == chord_qlty_tpls_.end()))
    {
        throw std::invalid_argument("ChordTpl::SlashSubtypesCnt(): Invalid chord quality");
    }

    return chord_qlty_tpls_[q].size();
}

ostream& operator<<(std::ostream& os, const ChordTpl& tpl)
{
    os << Chord(tpl.root_note_, tpl.chord_quality_, tpl.bass_note_);
    for (auto &v : tpl.tpl_) {
        os << "," << v;
    }

    return os;
}

}
