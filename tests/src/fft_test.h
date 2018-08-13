#ifdef FFT_TEST_FRIENDS
#undef FFT_TEST_FRIENDS
#endif
#define FFT_TEST_FRIENDS \
    friend class TestAvg; \
    friend class TestBitReverse; \
    friend class TestForwardTransform; \
    friend class TestInverseTransform; \
    friend class TestSortBitReversal; \

#include <fft.h>

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

class TestForwardTransform {
private:
    void __test();

public:
    void operator()() { __test(); };
};

class TestInverseTransform {
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
