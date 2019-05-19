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

#ifdef FFT_TEST_FRIENDS
#undef FFT_TEST_FRIENDS
#endif
#define FFT_TEST_FRIENDS                \
    friend class FftTestHelper;         \
    friend class TestAvg;               \
    friend class TestBitReverse;        \
    friend class TestSortBitReversal;   \

#include <fft.h>

namespace anatomist {

class FftTestHelper {
public:
    static void TestTransform(std::vector<complex_t> &, std::vector<complex_t> &);
};

class TestBitReverse {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestSortBitReversal {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestForwardTransform01 {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestForwardTransform02 {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestForwardTransform03 {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestAvg {
private:
    void __test();

public:
    void operator()() { __test(); };
};

}
