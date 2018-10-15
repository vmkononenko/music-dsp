#include <algorithm>

#include "lmhelpers.h"
#include "viterbi.h"

using namespace std;

typedef pair<uint32_t, double> state_metric_t;

vector<uint32_t> Viterbi::GetPath(obs_matrix_t &obs, vector<double> init_p,
        vector<vector<double>> trans_p)
{
    if (obs.empty() || obs[0].empty()) {
        throw invalid_argument("GetPath(): observation matrix is empty");
    }

    if (trans_p.empty() || trans_p[0].empty()) {
        throw invalid_argument("GetPath(): transition matrix is empty");
    }

    uint32_t obs_cnt = obs.size();
    uint32_t states_cnt = obs[0].size();

    if (init_p.size() != states_cnt) {
        throw invalid_argument("GetPath(): number of initial probabilities != number of states");
    }

    if ((trans_p.size() != states_cnt) || trans_p[0].size() != states_cnt) {
        throw invalid_argument("GetPath(): wrong transition matrix dimensions");
    }

    vector<vector<state_metric_t>> metrics(obs_cnt,  vector<state_metric_t>(states_cnt));
    vector<uint32_t> path(obs.size());

    for (uint32_t state = 0; state < states_cnt; state++) {
        metrics[0][state] = make_pair(0, init_p[state] * obs[0][state]);
    }

    for (uint32_t o = 1; o < obs_cnt; o++) {
        for (uint32_t i_state = 0; i_state < states_cnt; i_state++) {
            state_metric_t max_metric(states_cnt - 1, 0), cur_metric;
            for (uint32_t j_state = 0; j_state < states_cnt; j_state++) {
                cur_metric = make_pair(j_state, metrics[o-1][j_state].second * trans_p[j_state][i_state]);
                if (cur_metric.second > max_metric.second) {
                    max_metric = cur_metric;
                }
            }
            metrics[o][i_state] = make_pair(max_metric.first, max_metric.second * obs[o][i_state]);
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
