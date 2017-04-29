#include "cute.h"

#include "pitch_calculator_test.h"

void TestIsPitch::__test()
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    for (uint16_t i = 0; i < SEMITONES_TOTAL; i++) {
        ASSERT_EQUAL(true, pc.__isPitch(pc.__mPitches[i]));
    }

    ASSERT_EQUAL(true, pc.__isPitch(440));
    ASSERT_EQUAL(true, pc.__isPitch(130.8128));
    ASSERT_EQUAL(true, pc.__isPitch(277.1826));
    ASSERT_EQUAL(true, pc.__isPitch(587.3295));
    ASSERT_EQUAL(true, pc.__isPitch(622.2540));
    ASSERT_EQUAL(true, pc.__isPitch(82.4069));
    ASSERT_EQUAL(true, pc.__isPitch(1396.9129));
    ASSERT_EQUAL(true, pc.__isPitch(92.4986));
    ASSERT_EQUAL(true, pc.__isPitch(97.9989));
    ASSERT_EQUAL(true, pc.__isPitch(415.3047));
    ASSERT_EQUAL(true, pc.__isPitch(1760));
    ASSERT_EQUAL(true, pc.__isPitch(987.7666));
    ASSERT_EQUAL(false, pc.__isPitch(100500));
}

void TestA4PitchIdx::__test()
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    ASSERT_EQUAL(FREQ_A4, pc.__mPitches[pc.__mPitchIdxA4]);
}

void TestNoteToPitch::__test()
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    ASSERT_EQUAL(440, pc.noteToPitch(note_A, OCTAVE_4));
    ASSERT_EQUAL(130.8128, pc.noteToPitch(note_C, OCTAVE_3));
    ASSERT_EQUAL(277.1826, pc.noteToPitch(note_C_sharp, OCTAVE_4));
    ASSERT_EQUAL(587.3295, pc.noteToPitch(note_D, OCTAVE_5));
    ASSERT_EQUAL(622.2540, pc.noteToPitch(note_D_sharp, OCTAVE_5));
    ASSERT_EQUAL(82.4069, pc.noteToPitch(note_E, OCTAVE_2));
    ASSERT_EQUAL(1396.9129, pc.noteToPitch(note_F, OCTAVE_6));
    ASSERT_EQUAL(92.4986, pc.noteToPitch(note_F_sharp, OCTAVE_2));
    ASSERT_EQUAL(97.9989, pc.noteToPitch(note_G, OCTAVE_2));
    ASSERT_EQUAL(415.3047, pc.noteToPitch(note_G_sharp, OCTAVE_4));
    ASSERT_EQUAL(1760, pc.noteToPitch(note_A, OCTAVE_6));
    ASSERT_EQUAL(466.1638, pc.noteToPitch(note_A_sharp, OCTAVE_4));
    ASSERT_EQUAL(987.7666, pc.noteToPitch(note_B, OCTAVE_5));
}

void TestPitchToNote::__test()
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    ASSERT_EQUAL(note_C, pc.pitchToNote(130.8128));
    ASSERT_EQUAL(note_C_sharp, pc.pitchToNote(277.1826));
    ASSERT_EQUAL(note_D, pc.pitchToNote(587.3295));
    ASSERT_EQUAL(note_D_sharp, pc.pitchToNote(622.2540));
    ASSERT_EQUAL(note_E, pc.pitchToNote(82.4069));
    ASSERT_EQUAL(note_F, pc.pitchToNote(1396.9129));
    ASSERT_EQUAL(note_F_sharp, pc.pitchToNote(92.4986));
    ASSERT_EQUAL(note_G, pc.pitchToNote(97.9989));
    ASSERT_EQUAL(note_G_sharp, pc.pitchToNote(415.3047));
    ASSERT_EQUAL(note_A, pc.pitchToNote(1760));
    ASSERT_EQUAL(note_A_sharp, pc.pitchToNote(466.1638));
    ASSERT_EQUAL(note_B, pc.pitchToNote(987.7666));
}

void TestGetPitchByInterval::__test()
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    ASSERT_EQUAL(155.5635, pc.getPitchByInterval(130.8128, 3));
}
