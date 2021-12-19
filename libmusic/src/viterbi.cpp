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

#include <algorithm>

#include "lmhelpers.h"
#include "viterbi.h"

using namespace std;

typedef pair<uint32_t, prob_t> state_metric_t;


bool Viterbi::ValidateProbVector_(const std::vector<prob_t> &v)
{
    double p_total = 0;

    for (const auto & p : v) {
        p_total += p;
    }

    return Helpers::almostEqual(p_total, 1, (1.0 / 10000));
}

void Viterbi::ValidateInitProbs_(const vector<prob_t> &init_p)
{
    if (init_p.empty()) {
        throw invalid_argument("ValidateInitProbs_(): empty vector of initial probabilities");
    }

    if (!ValidateProbVector_(init_p)) {
        throw invalid_argument("ValidateInitProbs_(): total probability is not 1");
    }
}

void Viterbi::ValidateMatrix_(const vector<vector<double>> &obs) {
    if (obs.empty() || obs[0].empty()) {
        throw invalid_argument("ValidateMatrix_(): observation matrix is empty");
    }

    size_t states_cnt = obs[0].size();
    for (const auto & col : obs) {
        if (col.size() != states_cnt) {
            throw invalid_argument("ValidateMatrix_(): inconsistent size of columns");
        }
        if (!ValidateProbVector_(col)) {
            throw invalid_argument("ValidateMatrix_(): total column probability is not 1");
        }
    }
}

vector<uint32_t> Viterbi::GetPath(std::vector<prob_t> &init_p, prob_matrix_t &obs,
                                  prob_matrix_t &trans_p)
{
    ValidateInitProbs_(init_p);
    ValidateMatrix_(obs);
    ValidateMatrix_(trans_p);

    uint32_t obs_cnt = obs.size();
    uint32_t states_cnt = obs[0].size();

    if (init_p.size() != states_cnt) {
        throw invalid_argument("GetPath(): number of initial probabilities != number of states");
    }

    if ((trans_p.size() != states_cnt) || trans_p[0].size() != states_cnt) {
        throw invalid_argument("GetPath(): wrong transition matrix dimensions");
    }

    vector<vector<state_metric_t>> metrics(obs_cnt,
                                           vector<state_metric_t>(states_cnt, {0, -INFINITY}));
    vector<uint32_t> path(obs.size());

    for (uint32_t state = 0; state < states_cnt; state++) {
        metrics[0][state] = make_pair(0, std::log(init_p[state] * obs[0][state]));
    }

    for (uint32_t o = 1; o < obs_cnt; o++) {
        for (uint32_t i_state = 0; i_state < states_cnt; i_state++) {
            if (obs[o][i_state] > 0) {
                state_metric_t max_metric(states_cnt - 1, -INFINITY), cur_metric;
                for (uint32_t j_state = 0; j_state < states_cnt; j_state++) {
                    cur_metric = make_pair(j_state, metrics[o-1][j_state].second + std::log(trans_p[j_state][i_state]));
                    if (cur_metric.second > max_metric.second) {
                        max_metric = cur_metric;
                    }
                }
                metrics[o][i_state] = make_pair(max_metric.first, max_metric.second + std::log(obs[o][i_state]));
            }
        }
    }

    path[obs_cnt - 1] = max_element(metrics[obs_cnt - 1].begin(),
                                    metrics[obs_cnt - 1].end(),
                                    Helpers::cmpPairBySecond<uint32_t, double>)
                        - (metrics[obs_cnt - 1].begin());

    for (int32_t o = obs_cnt - 2; o >= 0; o--) {
        path[o] = metrics[o + 1][path[o + 1]].first;
    }

    return path;
}
