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
