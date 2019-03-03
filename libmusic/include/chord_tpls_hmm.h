#pragma once

#include "lmtypes.h"

#define CHORD_TPLS_HMM_TRAINED 0

std::map<note_t, std::map<chord_quality_t, std::vector<prob_t>>> hmm_training_res;
