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
