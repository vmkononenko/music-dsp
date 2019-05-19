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

#ifdef VITERBI_TEST_FRIENDS
#undef VITERBI_TEST_FRIENDS
#endif
#define VITERBI_TEST_FRIENDS                \
    friend class ViterbiTestHelper;         \

#include <viterbi.h>

class ViterbiTestHelper {
public:
    static bool ValidateInitProbs_(std::vector<double> &);
};

class TestInitProbsEmpty {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestInitProbsBadSum {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestObsEmpty {
private:
    void __test();

public:
    void operator()() { __test(); };
};

