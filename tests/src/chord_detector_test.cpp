#include <cstring>
#include <sndfile.h>

#include "config.h"
#include "cute.h"
#include "window_functions.h"

/* include this header, otherwise includes will go in the following order:
 * chord_detector_test.h -> chord_detector.h -> pitch_calculator.h. Then
 * pitch calculator test friends are defined in pitch_calculator.h before
 * pitch_calculator_test.h is processed by linker*/
#include "pitch_calculator_test.h"
#include "chord_detector_test.h"

#define TEST_FILES_DIR_ENV_VAR      "LM_TEST_FILES_DIR"
#define SEPARATE_CHORDS_DIR         "/chords_separate/"

uint32_t Common::openSoundFile(amplitude_t **buf, const char *filePath,
                               uint32_t *sampleRate)
{
    SNDFILE *sf;
    SF_INFO sfinfo;
    sf_count_t itemsCnt;

    /* Open input sound file */
    memset (&sfinfo, 0, sizeof (sfinfo)) ;
    sf = sf_open(filePath, SFM_READ, &sfinfo);

    ASSERTM("Could not open input file", (sf != nullptr));

    itemsCnt = std::min(CFG_WINDOW_SIZE, (uint32_t)(sfinfo.frames * sfinfo.channels));
    *buf = (double*) malloc(itemsCnt * sizeof(double));

    *sampleRate = sfinfo.samplerate;

    return sf_read_double(sf, *buf, itemsCnt);
}

void Common::testChord(const char *fileName, chord_t &expectedChord)
{
    std::string testFile;
    ChordDetector *cd = new ChordDetector();
    amplitude_t *timeDomain = nullptr;
    uint32_t samplesCnt, sampleRate = 0;
    const char* testFilesDir = std::getenv(TEST_FILES_DIR_ENV_VAR);

    ASSERTM("Test files directory (LM_TEST_FILES_DIR) is not specified",
            (testFilesDir != nullptr));

    testFile = std::string(testFilesDir) + std::string(SEPARATE_CHORDS_DIR) +
               std::string(fileName);

    samplesCnt = Common::openSoundFile(&timeDomain, testFile.c_str(), &sampleRate);
    ASSERTM("Could not read file", (samplesCnt != 0));

    WindowFunctions::applyDefault(timeDomain, samplesCnt);
    chord_t chord = cd->getChord(timeDomain, samplesCnt, sampleRate);

    ASSERT_EQUALM(fileName, expectedChord, chord);

    free(timeDomain);
    delete cd;
}

void TestChord_C::__test()
{
    const char *testFile;
    chord_t expected(note_C, cq_Major);

    testFile = "C_chord_construction_guitarlessonworld.wav";
    Common::testChord(testFile, expected);

    testFile = "C_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);

    testFile = "C_robbie_williams_sexed_up.wav";
    Common::testChord(testFile, expected);

    testFile = "C_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}

void TestChord_D::__test()
{
    const char *testFile;
    chord_t expected(note_D, cq_Major);

    testFile = "D_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}

void TestChord_Dm::__test()
{
    const char *testFile;
    chord_t expected(note_D, cq_Minor);

    testFile = "Dm_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);

    testFile = "Dm_zemfira_webgirl.wav";
    Common::testChord(testFile, expected);
}

void TestChord_Em::__test()
{
    const char *testFile;
    chord_t expected(note_E, cq_Minor);

    testFile = "Em_nikolskii_moi_drug_hudozhnik_i_poet_1.wav";
    Common::testChord(testFile, expected);

    testFile = "Em_nikolskii_moi_drug_hudozhnik_i_poet_2.wav";
    Common::testChord(testFile, expected);

    testFile = "Em_robbie_williams_sexed_up.wav";
    Common::testChord(testFile, expected);
}

void TestChord_F::__test()
{
    const char *testFile;
    chord_t expected(note_F, cq_Major);

    testFile = "F_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}

void TestChord_G::__test()
{
    const char *testFile;
    chord_t expected(note_G, cq_Major);

    testFile = "G_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);
}

void TestChord_Am::__test()
{
    const char *testFile;
    chord_t expected(note_A, cq_Minor);

    testFile = "Am_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);

    testFile = "Am_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}
