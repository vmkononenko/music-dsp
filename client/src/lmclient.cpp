/*
 * lmclient.cpp
 *
 * libmusic-dsp-client
 *
 * Command line interface for libmusic-dsp-client
 *
 */

#include <iomanip>
#include <iostream>
#include <string.h>
#include <sndfile.h>
#include <stdlib.h>

#include "chord_detector.h"
#include "config.h"
#include "fft.h"
#include "lmhelpers.h"
#include "window_functions.h"

using namespace std;

void usage();
void printScales();
void printFFT(double *, int, uint32_t, bool, bool);
void printTimeDomain(double *, uint32_t, bool);
void printChordInfo(amplitude_t *, uint32_t, uint32_t, bool);
void printAudioFileInfo(SF_INFO &);


int main(int argc, char* argv[])
{
    int minArgCnt = 2;              // see usage()
    bool isPolar = false;
    bool printTD = false;           // time domain
    bool tdViaInverseDFT = false;   // time domain obtained via inverse DFT
    bool printFD = false;           // frequency domain
    bool logScale = false;          // print frequency domain in logarithmic scale
    bool printAFI = false;          // audio file meta information
    bool detectChord = false;       // print detected chord
    bool printPCP = false;          // print pitch class profile

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            printFD = true;
            minArgCnt++;
        } else if (strcmp(argv[i], "-t") == 0) {
            printTD = true;
            minArgCnt++;
        } else if (strcmp(argv[i], "-i") == 0) {
            tdViaInverseDFT = true;
            minArgCnt++;
        } else if (strcmp(argv[i], "-p") == 0) {
            isPolar = true;
            minArgCnt++;
        } else if (strcmp(argv[i], "-c") == 0) {
            detectChord = true;
            minArgCnt++;
        } else if (strcmp(argv[i], "-m") == 0) {
            printAFI = true;
            minArgCnt++;
        } else if ((strcmp(argv[i], "-l") == 0)) {
            logScale = true;
            minArgCnt++;
        } else if ((strcmp(argv[i], "-s") == 0)) {
            printScales();
            return 0;
        } else if ((strcmp(argv[i], "--pcp") == 0)) {
            printPCP = true;
            minArgCnt++;
        } else if ((strcmp(argv[i], "-h") == 0)) {
            usage();
            return 0;
        } else if (argv[i][0] == '-') {
            cout << "Unrecognized option: " << argv[i] << endl;
            usage();
            return 1;
        }
    }

    if ((argc < minArgCnt) || (argv[argc - 1][0] == '-') ||
        (printFD && printTD) || (isPolar && !printFD) ||
        (printAFI && minArgCnt > 3) || (logScale && !isPolar) ||
        (tdViaInverseDFT && !printTD) || (detectChord && minArgCnt > 3))
    {
        usage();
        return 1;
    }

    char *inputFilePath = argv[argc - 1];
    SNDFILE *sf;
    SF_INFO sfinfo;
    sf_count_t itemsCnt;
    double *buf;

    /* Open input sound file */
    memset (&sfinfo, 0, sizeof (sfinfo)) ;
    if (!(sf = sf_open(inputFilePath, SFM_READ, &sfinfo))) {
        cout << "Failed to open the file" << endl;
        return 1;
    }

    itemsCnt = sfinfo.frames * sfinfo.channels;
    buf = (double*) malloc(itemsCnt * sizeof(double));

    if(!sf_read_double(sf, buf, itemsCnt)) {
        cout << "Could not read file" << endl;
        return 1;
    }

    if (printTD) {
        printTimeDomain(buf, itemsCnt, tdViaInverseDFT);
    } else if (printFD) {
        printFFT(buf, sfinfo.samplerate, itemsCnt, isPolar, logScale);
    } else if (printAFI) {
        printAudioFileInfo(sfinfo);
    } else if (detectChord) {
        printChordInfo(buf, sfinfo.samplerate, itemsCnt, false);
    } else if (printPCP) {
        printChordInfo(buf, sfinfo.samplerate, itemsCnt, true);
    }

    sf_close(sf);
    free(buf);

    return 0;
}

void printTimeDomain(amplitude_t *timeDomain, uint32_t samples, bool tdViaInverseDFT)
{
    FFT *fft = new FFT();
    vector<complex_t> x = Helpers::timeDomain2ComplexVector(timeDomain, samples, samples);

    if (tdViaInverseDFT) {
        fft->forward(x);
        fft->inverse(x);
    }
    for (uint32_t i = 0; i < samples; i++) {
        cout << i << "," << real(x[i]) << endl;
    }
}

double max_amplitude(std::vector<complex_t> x) {
    double max = real(x[0]);
    for (complex_t point : x) {
        amplitude_t mag = real(point);
        if (mag > max) {
            max = mag;
        }
    }
    return max;
}

void printFFT(amplitude_t *timeDomain, int sampleRate, uint32_t samples,
              bool isPolar, bool logScale)
{
    FFT *fft = new FFT();
    vector<complex_t> x;
    uint32_t fftSize = Helpers::nextPowerOf2(samples);

    WindowFunctions::applyHamming(timeDomain, samples);
    x = Helpers::timeDomain2ComplexVector(timeDomain, samples, fftSize);

    fft->forward(x);
    if (isPolar) {
        fft->toPolar(x);
    }

    if (isPolar) {
        double magMax = max_amplitude(x);
        cout << magMax;
        for (uint32_t i = 0; i < samples / 2; i++) {
            amplitude_t freq = i * sampleRate / x.size();
            double mag = logScale ? 10 * log10(real(x[i]) / magMax) : real(x[i]);
            cout << freq << "," << mag << endl;
        }
    } else {
        cout << "Not implemented" << endl;
    }

    delete fft;
}

void printChordInfo(amplitude_t *timeDomain, uint32_t sampleRate,
		            uint32_t samples, bool printPCP)
{
    ChordDetector *cd = new ChordDetector();

    //WindowFunctions::applyHamming(timeDomain, samples);
    //WindowFunctions::applyBlackman(timeDomain, samples);
    if (samples > CFG_WINDOW_SIZE) {
        samples = CFG_WINDOW_SIZE;
    }
    WindowFunctions::applyDefault(timeDomain, CFG_WINDOW_SIZE);

    if (printPCP) {
        PitchClsProfile pcp = cd->getPCP(timeDomain, samples, sampleRate);
        cout << pcp << endl;
    } else {
        chord_t chord = cd->getChord(timeDomain, samples, sampleRate);
        cout << chord << endl;
    }

    delete cd;
}

void printAudioFileInfo(SF_INFO &sfinfo)
{
    cout << setw(20) << "Frames (samples):" << setw(10) << sfinfo.frames << "\n"
         << setw(20) << "Sample rate:" << setw(10) << sfinfo.samplerate << "\n"
         << setw(20) << "Format:" << setw(10) << sfinfo.format << "\n"
         << setw(20) << "Sections:" << setw(10) << sfinfo.sections << "\n"
         << setw(20) << "Seekable:" << setw(10) << sfinfo.seekable << "\n";
}

void __printScale(note_t note, vector<note_t> scale)
{
    cout << setw(2) << note << ": " << setw(3) << right << Helpers::noteToString(note)
         << setw(5) << left << right << "[  ";
    for (uint8_t i = 0; i < scale.size(); i++) {
        cout << setw(2) << left << Helpers::noteToString(scale[i]) << "  ";
    }
    cout << "]" << endl;
}

void printScales()
{
    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;
        __printScale(note, MusicScale::getMajorScale(note));
    }
}

void usage()
{
    cout << "Usage:\n"
         << "\tlmclient <-t [i] | -f [-p] [-l] | -c> <filename>\n"
         << "\tlmclient -i <filename>\n"
         << endl;

    cout << "\nOptions:\n"
         << "\t-t\tprint time domain data\n"
         << "\t-i\tprint time domain obtained by inverse transform to frequency domain\n"
         << "\t-f\tprint frequency domain data\n"
         << "\t-p\tused with -f. Output frequency domain in polar format.\n"
            "\t\tCan be used with -l to display data in logarithmic scale\n"
         << "\t-l\tdisplay frequency domain in logarithmic scale. Is used with -p\n"
         << "\t-m\tdisplay input audio file information and exit\n"
         << "\t-c\tprint recognized chord information\n"
         << "\t-s\tprint major scales for all notes"
         << endl;

    cout << "\nExamples:\n"
         << "\tPrint time domain:\n"
         << "\t\tlmclient -t /path/to/file\n\n"
         << "\tPrint time domain obtained by inverse transform to frequency domain:\n"
         << "\t\tlmclient -t -i /path/to/file\n\n";
}
