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

#include <cstring>

#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "chord_detector_test.h"
#include "fft_test.h"
#include "helpers_test.h"
#include "pitch_calculator_test.h"
#include "viterbi_test.h"


using namespace anatomist;
using namespace std;

void runTests(int argc, char const *argv[], const cute::suite &suite, const char *name)
{
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(suite, name);
}

cute::suite fftTestSuite()
{
    cute::suite s;

    s.push_back(TestBitReverse());
    s.push_back(TestSortBitReversal());
    s.push_back(TestForwardTransform01());
    s.push_back(TestForwardTransform02());
    s.push_back(TestForwardTransform03());
    s.push_back(TestAvg());

    return s;
}

cute::suite helpersTestSuite()
{
    cute::suite s;

    s.push_back(TestNextPowerOf2());
    s.push_back(TestStdRound());

    return s;
}

cute::suite pitchCalculatorTestSuite()
{
    cute::suite s;

    s.push_back(TestIsPitch());
    s.push_back(TestA4PitchIdx());
    s.push_back(TestNoteToPitch());
    s.push_back(TestPitchToNote());
    s.push_back(TestGetPitchByInterval());

    return s;
}

cute::suite chordDetectorTestSuite()
{
    cute::suite s;

    s.push_back(TestChord_C());
    s.push_back(TestChord_D());
    s.push_back(TestChord_Dm());
    s.push_back(TestChord_Em());
    s.push_back(TestChord_F());
    s.push_back(TestChord_G());
    s.push_back(TestChord_Am());

    return s;
}

cute::suite viterbiTestSuite()
{
    cute::suite s;

    s.push_back(TestInitProbsEmpty());
    s.push_back(TestInitProbsBadSum());
    s.push_back(TestObsEmpty());

    return s;
}

void usage()
{
	cout << "Usage:\r\tlmtests --<all|fft|helpers|chords|viterbi>" << endl;
}

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		usage();
		return -1;
	}

	cute::suite suite;
	const char *name = "";

	if (strcmp(argv[1], "--all") == 0) {
		cout << "all" << endl;
	} else if (strcmp(argv[1], "--fft") == 0) {
		suite = fftTestSuite();
		name = "FFT Tests";
	} else if (strcmp(argv[1], "--helpers") == 0) {
		suite = helpersTestSuite();
		name = "Helpers Tests";
	} else if (strcmp(argv[1], "--pitch") == 0) {
	    suite = pitchCalculatorTestSuite();
	    name = "Pitch Detector Test Suite";
	} else if (strcmp(argv[1], "--chords") == 0) {
	    suite = chordDetectorTestSuite();
	    name = "Chord Detector Test Suite";
	} else if (strcmp(argv[1], "--viterbi") == 0) {
	    suite = viterbiTestSuite();
	    name = "Viterbi Test Suite";
	} else {
		usage();
		return -1;
	}

	/** @TODO 1 has to be passed as argc to runTests() otherwise CUTE
	 *        will produce an empty output */
	runTests(1, argv, suite, name);

	return 0;
}

