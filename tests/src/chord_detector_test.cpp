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
#include <sndfile.h>

#include "config.h"
#include "cute.h"
#include "window_functions.h"

#include "chord_detector_test.h"

#define TEST_FILES_DIR_ENV_VAR      "LM_TEST_FILES_DIR"
#define SEPARATE_CHORDS_DIR         "/chords_separate/"


using namespace anatomist;

uint32_t Common::openSoundFile(amplitude_t **chanBuf, const char *filePath,
                               uint32_t *sampleRate)
{
    SNDFILE *sf;
    SF_INFO sfinfo;
    amplitude_t *buf;
    sf_count_t itemsTotal;

    /* Open input sound file */
    memset (&sfinfo, 0, sizeof (sfinfo)) ;
    sf = sf_open(filePath, SFM_READ, &sfinfo);

    ASSERTM("Could not open input file", (sf != nullptr));

    itemsTotal = (uint32_t)(sfinfo.frames * sfinfo.channels);
    buf = (double*) malloc(itemsTotal * sizeof(double));

    sf_read_double(sf, buf, itemsTotal);

    *sampleRate = sfinfo.samplerate;

    *chanBuf = (amplitude_t *) malloc(sfinfo.frames * sizeof(amplitude_t));

    for (uint32_t i = 0; i < sfinfo.frames; i++) {
        (*chanBuf)[i] = buf[i * sfinfo.channels];
    }

    free(buf);

    return sfinfo.frames;
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

    std::vector<segment_t> segments;
    cd->getSegments(segments, timeDomain, samplesCnt, sampleRate);

    for (auto s : segments) {
        ASSERTM(fileName, (s.silence || s.chord == expectedChord));
    }

    free(timeDomain);
    delete cd;
}

void TestChord_C::__test()
{
    const char *testFile;
    chord_t expected(note_C, cq_maj);

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
    chord_t expected(note_D, cq_maj);

    testFile = "D_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}

void TestChord_Dm::__test()
{
    const char *testFile;
    chord_t expected(note_D, cq_min);

    testFile = "Dm_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);

    testFile = "Dm_zemfira_webgirl.wav";
    Common::testChord(testFile, expected);
}

void TestChord_Em::__test()
{
    const char *testFile;
    chord_t expected(note_E, cq_min);

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
    chord_t expected(note_F, cq_maj);

    testFile = "F_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}

void TestChord_G::__test()
{
    const char *testFile;
    chord_t expected(note_G, cq_maj);

    testFile = "G_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);
}

void TestChord_Am::__test()
{
    const char *testFile;
    chord_t expected(note_A, cq_min);

    testFile = "Am_liapis_trubetskoi_v_platie_belom.wav";
    Common::testChord(testFile, expected);

    testFile = "Am_the_animals_house_of_the_rising_sun.wav";
    Common::testChord(testFile, expected);
}
