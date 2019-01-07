/*
 * lmclient.cpp
 *
 * libmusic-dsp-client
 *
 * Command line interface for libmusic-dsp-client
 *
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <sndfile.h>
#include <stdlib.h>

#include "beat_detector.h"
#include "chord_detector.h"
#include "config.h"
#include "envelope.h"
#include "fft.h"
#include "lmhelpers.h"
#include "window_functions.h"

using namespace anatomist;
using namespace std;

void usage();
void printScales();
void printSigEnvelope(amplitude_t *, uint32_t);
void printFFT(double *, int, uint32_t, bool, bool);
void printTimeDomain(double *, uint32_t, uint32_t, bool, bool);
void printChordInfo(amplitude_t *, SF_INFO &, uint32_t, uint32_t, string, bool, int, bool);
void printAudioFileInfo(SF_INFO &);
void printBPM(amplitude_t *, uint32_t, uint32_t);


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
    bool printEnvelope = false;     // print signal envelope
    bool detectBeat = false;        // print beats per minute
    bool legacy = false;            // legacy version of the feature
    int  n = 0;                     // a number of FFT windows to analyze
    string refChord;                // reference chord to evaluate against
    int winSize = 0;                // default window size is set by the lib

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            printFD = true;
            minArgCnt++;
        } else if (strcmp(argv[i], "-t") == 0) {
            printTD = true;
            minArgCnt++;
        } else if ((strcmp(argv[i], "-e") == 0)) {
            printEnvelope = true;
            minArgCnt++;
        } else if ((strcmp(argv[i], "-b") == 0)) {
            detectBeat = true;
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
        } else if (strcmp(argv[i], "-w") == 0) {
            i++;
            if (i >= argc) { usage(); return 1; }
            winSize = atoi(argv[i]);
            if (winSize <= 0) {usage(); return 1; }
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
        } else if ((strcmp(argv[i], "-n") == 0)) {
            i++;
            if (i >= argc) { usage(); return 1; }
            n = atoi(argv[i]);
            minArgCnt++;
        } else if ((strcmp(argv[i], "-r") == 0)) {
            i++;
            if (i >= argc) { usage(); return 1; }
            refChord = string(argv[i]);
            minArgCnt++;
        } else if ((strcmp(argv[i], "--legacy") == 0)) {
            legacy = true;
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
        (tdViaInverseDFT && !printTD) || (detectChord && minArgCnt > 6) ||
        (winSize > 0 && !detectChord && !printPCP) ||
        (detectChord && legacy && (winSize || n > 0)) ||
        (printEnvelope && minArgCnt > 3) ||
        (detectBeat && !printTD && minArgCnt > 3) ||
        (detectBeat && printTD && minArgCnt > 4) || (legacy && minArgCnt == 3))
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
        printTimeDomain(buf, itemsCnt, sfinfo.samplerate, tdViaInverseDFT, detectBeat);
    } else if (printFD) {
        printFFT(buf, sfinfo.samplerate, itemsCnt, isPolar, logScale);
    } else if (printAFI) {
        printAudioFileInfo(sfinfo);
    } else if (detectChord || printPCP) {
        printChordInfo(buf, sfinfo, itemsCnt, n, refChord, printPCP, winSize, legacy);
    } else if (printEnvelope) {
        printSigEnvelope(buf, itemsCnt);
    } else if (detectBeat && !printTD) {
        printBPM(buf, itemsCnt, sfinfo.samplerate);
    }

    sf_close(sf);
    free(buf);

    return 0;
}

void printTimeDomain(amplitude_t *td, uint32_t samples, uint32_t samplerate,
                     bool td_via_inverse_dft, bool detect_beat)
{
#define PLOT_SAMPLES_MAX    1000000U
    if (detect_beat) {
        BeatDetector *bd = new BeatDetector(td, samples, samplerate);
        uint32_t beatOffset = bd->getOffset();
        uint16_t beatInterval = bd->getIdxInterval();
        uint32_t plotSamples = min(PLOT_SAMPLES_MAX, samples);
        amplitude_t ampMax = *max_element(td, td + plotSamples);
        amplitude_t ampMin = *min_element(td, td + plotSamples);
        uint32_t beats = 0;

        for (uint32_t i = 0; i < plotSamples; i++) {
            amplitude_t beatAmp = ampMin;
            if (i == beatOffset + beats * beatInterval) {
                beatAmp = ampMax;
                beats++;
            }
            cout << i << "," << td[i] << "," << beatAmp << endl;
        }

        return;
    }

    anatomist::FFT *fft = new anatomist::FFT(td, samples, samplerate, false);
    vector<complex_t> *x = fft->GetFreqDomain().r();

    if (td_via_inverse_dft) {
        fft->Inverse();
    }

    for (uint32_t i = 0; i < samples; i++) {
        cout << i << "," << real((*x)[i]) << endl;
    }

    delete fft;
}

void printSigEnvelope(amplitude_t *timeDomain, uint32_t samples) {
    Envelope *e = new Envelope(timeDomain, samples);

    cout << *e;

    delete e;
}

void printBPM(amplitude_t *timeDomain, uint32_t samples, uint32_t sampleRate) {
    BeatDetector *bd = new BeatDetector(timeDomain, samples, sampleRate);

    cout << bd->getBPM() << endl;

    delete bd;
}

double max_amplitude(amplitude_t *p, uint32_t len) {
    double max = p[0];
    for (uint32_t i = 0; i < len; i++) {
        if (p[i] > max) {
            max = p[i];
        }
    }
    return max;
}

void printFFT(amplitude_t *td, int samplerate, uint32_t samples,
              bool polar, bool logScale)
{
    if (!polar) {
        throw runtime_error("Not implemented");
    }

    anatomist::FFT *fft = new anatomist::FFT(td, samples, samplerate, polar);
    amplitude_t *p = fft->GetFreqDomain().p;
    double mag_max = max_amplitude(p, fft->GetFreqDomainLen());

    cout << mag_max;

    for (uint32_t i = 0; i < fft->GetFreqDomainLen() / 2; i++) {
        amplitude_t freq = i * samplerate / fft->GetSize();
        double mag = logScale ? 10 * log10(p[i] / mag_max) : p[i];
        cout << freq << "," << mag << endl;
    }

    delete fft;
}

void __printChordEvalScore(uint32_t total, uint32_t fails)
{
    float score = (total - fails) * 100.f / total;
    cout << fixed << setprecision(2) << score << " %" << endl;
}

void __printChordInfoLegacy(amplitude_t *timeDomain, SF_INFO &sfinfo,
                            uint32_t itemsCnt, uint32_t n, string refChord,
                            bool printPCP, int winSize)
{
    ChordDetector *cd = new ChordDetector();
    uint32_t iterMax;
    uint32_t iter = 1;
    uint32_t fails = 0;

    if (winSize <= 0) {
        winSize = CFG_WINDOW_SIZE;
    }

    iterMax = itemsCnt / sfinfo.channels / winSize;
    if (itemsCnt / sfinfo.channels % winSize) {
        iterMax += 1;
    }

    iterMax = (n == 0) ? iterMax : std::min(iterMax, n);

    while (iter <= iterMax) {
        uint32_t len = (iter * winSize <= itemsCnt / sfinfo.channels) ? winSize :
                       itemsCnt / sfinfo.channels - (iter - 1) * winSize;
        amplitude_t *chanTD = (amplitude_t *) malloc(len * sizeof(amplitude_t));

        /* extract time domain for the first channel */
        for (uint32_t i = 0; i < len; i++) {
            uint32_t offset = (iter - 1) * winSize;
            chanTD[i] = timeDomain[offset + i * sfinfo.channels];
        }

        if (printPCP) {
            pcp_t *pcp = cd->GetPCP(chanTD, len, sfinfo.samplerate);
            cout << *pcp << endl;
            delete pcp;
        } else {
            chord_t chord = cd->getChord(chanTD, len, sfinfo.samplerate);
            if (refChord.empty()) {
                cout << setw(3) << iter << ": " << chord << endl;
            } else {
                if (refChord.compare(chord.toString())) {
                    fails++;
                }
            }
        }

        free(chanTD);
        iter++;
    }

    if (!refChord.empty()) {
        __printChordEvalScore(iterMax, fails);
    }

    delete cd;
}

void printChordInfo(amplitude_t *timeDomain, SF_INFO &sfinfo, uint32_t itemsCnt,
                    uint32_t n, string refChord, bool printPCP, int winSize,
                    bool legacy)
{
    if (legacy) {
        return __printChordInfoLegacy(timeDomain, sfinfo, itemsCnt, n, refChord,
                                      printPCP, winSize);
    }

    ChordDetector *cd = new ChordDetector();
    amplitude_t *channelTD = (amplitude_t *) malloc(sfinfo.frames * sizeof(amplitude_t));
    std::vector<segment_t> segments;
    uint32_t fails = 0;

    for (uint32_t i = 0; i < sfinfo.frames; i++) {
        channelTD[i] = timeDomain[i * sfinfo.channels];
    }

    if (!printPCP) {
        cd->getSegments(segments, channelTD, sfinfo.frames, sfinfo.samplerate);
        for (uint32_t i = 0; i < segments.size(); i++) {
            segment_t *s = &segments[i];
            if (refChord.empty()) {
                auto idxToSec = [&](int idx) {
                    return Helpers::stdRound<float>(idx / (float) sfinfo.samplerate, 2);
                };
                cout << setw(3) << i << ": " << setw(3) << (s->silence ? "S" : s->chord.toString())
                     << " [" << idxToSec(s->startIdx) << ", " << idxToSec(s->endIdx) << "]" << endl;
            } else {
                if (!s->silence && refChord.compare(s->chord.toString())) {
                    fails += (s->endIdx - s->startIdx + 1);
                }
            }
        }

        if (!refChord.empty()) {
            __printChordEvalScore(sfinfo.frames, fails);
        }
    } else {
        chromagram_t chromagram = cd->GetChromagram(channelTD, sfinfo.frames, sfinfo.samplerate);
        uint32_t print_cnt = (n == 0) ? chromagram.size() : std::min(static_cast<uint32_t>(chromagram.size()), n);

        for (uint32_t i = 0; i < print_cnt; i++) {
            cout << chromagram[i] << endl;
        }

        cout << chromagram.size() << endl;
    }

    free(channelTD);
    delete cd;
}

void printAudioFileInfo(SF_INFO &sfinfo)
{
    cout << setw(20) << "Frames:"       << setw(10) << sfinfo.frames << "\n"
         << setw(20) << "Channels:"     << setw(10) << sfinfo.channels << "\n"
         << setw(20) << "Sample rate:"  << setw(10) << sfinfo.samplerate << "\n"
         << setw(20) << "Format:"       << setw(10) << sfinfo.format << "\n"
         << setw(20) << "Sections:"     << setw(10) << sfinfo.sections << "\n"
         << setw(20) << "Seekable:"     << setw(10) << sfinfo.seekable << "\n";
}

void __printScale(note_t note, vector<note_t> scale)
{
    cout << setw(3) << right << note << setw(5) << left << right << "[  ";
    for (uint8_t i = 0; i < scale.size(); i++) {
        cout << setw(2) << left << scale[i] << "  ";
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
         << "\tlmclient <-t [i] | -f [-p] [-l]> <filename>\n"
         << "\tlmclient <-c [-r <reference_chord>] | --pcp> [-n <iterations>] <filename>\n"
         << endl;

    cout << "\nOptions:\n"
         << "\t-t\tprint time domain data\n"
         << "\t-e\tprint signal envelope\n"
         << "\t-i\tprint time domain obtained by inverse transform to frequency domain\n"
         << "\t-f\tprint frequency domain data\n"
         << "\t-p\tused with -f. Output frequency domain in polar format.\n"
            "\t\tCan be used with -l to display data in logarithmic scale\n"
         << "\t-l\tdisplay frequency domain in logarithmic scale. Is used with -p\n"
         << "\t-m\tdisplay input audio file information and exit\n"
         << "\t-s\tprint major scales for all notes\n"
         << "\t-c\tprint recognized chord information\n"
         << "\t-r <rc>\tcalculate precision score. The file has to contain single chord recording.\n"
         << "\t\tUsed with -c\n"
         << "\t--pcp\tprint Pitch Class Profile\n"
         << "\t-w\twindow size in samples - length of blocks to pass for analysis.\n"
         << "\t\tUsed with -c and --pcp\n"
         << "\t-n <iterations>\tnumber of FFT windows to analyse. Used with -c or --pcp\n"
         << "\t-b\tdetect BPM of the input audio\n"
         << "\t\tIn combination with -t prints peaks at the beat indices along with time domain.\n"
         << "\t--legacy\tuse legacy version of the feature. Can't be used a standalone option."
         << endl;

    cout << "\nExamples:\n"
         << "\tPrint time domain:\n"
         << "\t\tlmclient -t /path/to/file\n\n"
         << "\tPrint time domain obtained by inverse transform to frequency domain:\n"
         << "\t\tlmclient -t -i /path/to/file\n\n"
         << "\tGet chord precision score fo a file containing single chord recoring:\n"
         << "\t\tlmclient -c -r Dm /path/to/file/with/Dm\n\n"
         << "\tPrint detected beat along with time domain:\n"
         << "\t\tlmclient -b -t /path/to/file\n\n";
}
