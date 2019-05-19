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

#include "cute.h"

#include "viterbi_test.h"


using namespace std;

bool ViterbiTestHelper::ValidateInitProbs_(vector<double> &init_p) {
    bool e_thrown = false;

    try {
        Viterbi::ValidateInitProbs_(init_p);
    } catch (const invalid_argument & e) {
        // good, this is expected
        e_thrown = true;
    }

    return e_thrown;
}

void TestInitProbsEmpty::__test()
{
    vector<double> init_p;

    ASSERT_EQUALM("Empty initial probabilities matrix", true,
                  ViterbiTestHelper::ValidateInitProbs_(init_p));
}

void TestInitProbsBadSum::__test()
{
    vector<double> init_p { 0.9, 0.2 };

    ASSERT_EQUALM("Wrong initial probabilities sum", true,
                  ViterbiTestHelper::ValidateInitProbs_(init_p));
}

void TestObsEmpty::__test()
{

}
