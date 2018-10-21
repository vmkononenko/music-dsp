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

