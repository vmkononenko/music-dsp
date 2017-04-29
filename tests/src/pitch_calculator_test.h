#pragma once

#define PITCH_CALCULATOR_TEST_FRIENDS \
    friend class TestIsPitch; \
    friend class TestA4PitchIdx; \

#include <pitch_calculator.h>

class TestIsPitch {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestGetPitch {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestPitchToNote {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestA4PitchIdx {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestNoteToPitch {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestGetPitchByInterval {
private:
    void __test();

public:
    void operator()() { __test(); };
};
