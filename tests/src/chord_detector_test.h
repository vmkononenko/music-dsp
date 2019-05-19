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

#pragma once

#include "chord_detector.h"


class Common {

public:
    static uint32_t openSoundFile(amplitude_t **buf, const char *filePath,
                                  uint32_t *sampleRate);

    static void testChord(const char *fileName, chord_t &expectedChord);
};

class TestChord_C {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestChord_D {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestChord_Dm {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestChord_Em {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestChord_F {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestChord_G {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestChord_Am {
private:
    void __test();

public:
    void operator()() { __test(); };
};
