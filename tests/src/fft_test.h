#ifdef FFT_TEST_FRIENDS
#undef FFT_TEST_FRIENDS
#endif
#define FFT_TEST_FRIENDS                \
    friend class FftTestHelper;         \
    friend class TestAvg;               \
    friend class TestBitReverse;        \
    friend class TestSortBitReversal;   \

#include <fft.h>

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
