/*
 * synth
 * original version by ector
 * modified by tonic
 * heavily modified by !cube
 *
 * $Id: synth.c,v 1.43 2005/01/22 15:19:50 tonic Exp $
 * $Revision: 1.43 $
 */

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "minimath.h"
#include "importgl.h"
#include "synth.h"


// Globals
//int sync;
int songPos = 0;    // 1/16 notes
int songTime = 0;   // samples
int tempoTime = 0;
int tickTime = 0;

#define LFO_BASE 0
#define LFO_PHASE PI
#define LFO_RANGE 128
#define LFO_SPEED (2 * PI) / (TEMPO * 32)

#define START_CHANNEL 0
#ifdef USE_HIHAT
#define END_CHANNEL 9
#else
#define END_CHANNEL 8
#endif

#ifdef USE_PAD_CHORDS
#define DELAY_CHANNELS 4
#else
#define DELAY_CHANNELS 4
#endif
#define BASSDRUM_CHANNEL 6
#define SNARE_CHANNEL 7
#define HIHAT_CHANNEL 8
#define SHIFT_OPT (12 - BASSDRUM_CHANNEL)

#define DELAYSIZE (TEMPO * 3)
static short mixBuffer[SOUND_TOTAL_BUFFER_SIZE * 2];
static int delayBuffer[DELAYSIZE] = { 0, };
static int delayPos, cutAdd;
static int n1;
#ifdef USE_HIHAT
static int n2;
#endif

static int lastBufferRendered = SOUND_NUMBUFFERS - 1;
static HWAVEOUT waveOutHandle;
static WAVEHDR waveHdr;
static WAVEFORMATEX pcmwf;


static void soundStep()
{
    int i;
    for (i = START_CHANNEL; i < END_CHANNEL; ++i) //for (i = 0; i < NUMCHANNELS; i++)
    {
        int pat = seq[i][(songPos >> PATTERNLENGTH_SHIFT) % SONGLENGTH];   // current pattern (with looping)
        
        if (pat)
        {
            int note = patterns[pat - 1][songPos & (PATTERNLENGTH - 1)];    // current note
            
            if (note != xx)
            {
                chans[i].note = (note & 0x7F);
//                chans[i].delta = (uint32)(2678267.58f * pow(2.0f, (float)(chans[i].note + chans[i].transpose * 12 - 1 + 3) / 12.0f));
//                65536 * 65536 * 4.0879f * pow(2.0f, (float)(chans[i].note - 1 + chans[i].transpose * 12) / 12.0f) / 44100.f;
                chans[i].delta = (uint32)(398126.91177592743764172335600907f * pow(2.0f, (float)(chans[i].note + chans[i].transpose) / 12.0f));
                chans[i].delta2 = chans[i].delta + ((chans[i].detune) << 13);
                if (note & 0x80)
                {
                    chans[i].pos = 0;
                    chans[i].pos2 = chans[i].initPos2;
                }
                chans[i].envPos = 1 << 24;
                chans[i].volume = (chans[i].originalVolume << 8);

#ifdef USE_HIHAT
                if (!(note & 1) && (i == HIHAT_CHANNEL))
                    chans[i].volume = (chans[i].volume * 5) >> 3;
#endif
            }
        }
    }

    ++songPos;

    //// set sync flag on every 4th row
    //if (((songPos + 4) & 3) == 0) 
    //    sync = 1;
}


/*
static int synthrandseed = 0x2;
__forceinline int synthrand()
{
    //randseed = randseed * 0x343fd + 0x269ec3;
    //return randseed >> 16;
    synthrandseed *= 16807;
    return synthrandseed;
}
*/
#define synthrand rand


static void sound_fillBuffer(int buffer)
{
    int i, wave, c, v, t;
    short *fillPos = mixBuffer + buffer * SOUND_BUFFER_SIZE * 2;

    for (i = 0; i < SOUND_BUFFER_SIZE; ++i) 
    {
        cutAdd = LFO_BASE + (int)(LFO_RANGE * ((float)cos(LFO_PHASE + (float)songTime * LFO_SPEED)));

        if (tempoTime == 0)
            soundStep();    // step song forward

        wave = 0;   // current sample
        v = 0;

        for (c = START_CHANNEL; c < END_CHANNEL; ++c) //for (c = 0; c < NUMCHANNELS; c++)
        {
            int sample, sample2;

            // if note cut is needed, remove comments
#ifdef USE_NOTE_CUT
            if (chans[c].note != oo)
#endif
            {
#ifdef USE_VIBRATO
                int vibrato = (int)(sin((float)songTime / chans[c].vibSpeed) * chans[c].vibRange);
#endif

                sample = (int)(chans[c].pos >> 18);

#ifndef USE_PAD_CHORDS
                if (chans[c].type & 1)
                {
#endif
                    sample2 = (int)(chans[c].pos2 >> 18);
                    sample = sample - sample2;
#ifndef USE_PAD_CHORDS
                }
#endif

                t = chans[c].pos >> 26;
#ifdef USE_VIBRATO
                chans[c].pos += chans[c].delta + vibrato;         // go forward saw wave
                chans[c].pos2 += chans[c].delta2 + vibrato;       // go forward saw wave
#else
                chans[c].pos += chans[c].delta;         // go forward saw wave
                chans[c].pos2 += chans[c].delta2;       // go forward saw wave
#endif

                if (t != (chans[c].pos >> 26))
                {
#ifdef USE_HIHAT
                    if (c == SNARE_CHANNEL)
                        n1 = synthrand() & 0x3fff;
                    else if (c == HIHAT_CHANNEL)
                        n2 = synthrand() & 0x3fff;
#else
                    if (c == SNARE_CHANNEL)
                        n1 = synthrand() & 0x3fff;
#endif
                }

                chans[c].envPos -= chans[c].envPos >> chans[c].envShift;
            }
#ifdef USE_NOTE_CUT
            else sample = 0;
#endif

            if (c == DELAY_CHANNELS)
            {
                //-- DELAY -------------------------------------------
                // do delay effect here so that it will not affect drums
                v = (delayBuffer[delayPos] * 5) >> 3;
                delayBuffer[delayPos] = v + wave;
                ++delayPos;
                if (delayPos >= DELAYSIZE)
                    delayPos = 0;
                //-- END DELAY -------------------------------------------
            }

            if (c == BASSDRUM_CHANNEL || c == SNARE_CHANNEL)
            {
                chans[c].delta -= chans[c].delta >> (c + SHIFT_OPT);
                chans[c].delta2 = chans[c].delta;
                if (c == SNARE_CHANNEL) sample += n1;
            }
#ifdef USE_HIHAT
            else if (c == HIHAT_CHANNEL)
            {
                sample = n2;
            }
#endif

            // volume
            sample = (sample * (chans[c].volume >> 8)) >> 8;

            if (tickTime == 0)
            {
                chans[c].volume -= chans[c].volumeFade;
                if (chans[c].volume < 0) chans[c].volume = 0;
            }

            {
                int cut = chans[c].cutoff + (chans[c].envPos >> chans[c].envShift2);
                if (chans[c].lfo)
                    cut += cutAdd;

                if (cut < 10)
                    cut = 10;

                chans[c].filterDelta = (chans[c].filterDelta * chans[c].resonance) >> 8;
                chans[c].filterDelta += ((sample - chans[c].filterPos) * cut) >> 11;
                chans[c].filterPos += chans[c].filterDelta;
            }

            wave += chans[c].filterPos;

            //chans[c].envPos *= 0.999f;       // envelope fade out
        }

        {
#ifdef SURROUND

            // surround sound
            int sample1 = (wave - v) >> 1;
            int sample2 = (wave + v) >> 1;
#ifdef USE_CLIPPING
            if (sample1 < -32768) sample1 = -32768;
            if (sample1 > 32767) sample1 = 32767;
            if (sample2 < -32768) sample2 = -32768;
            if (sample2 > 32767) sample2 = 32767;
#endif
            *fillPos++ = sample1;
            *fillPos++ = sample2;

#else

            int sample = (wave + v) >> 1;
#ifdef USE_CLIPPING
            if (sample < -32768) sample = -32768;
            if (sample > 32767) sample = 32767;
#endif

            *fillPos++ = sample;
            *fillPos++ = sample;
#endif
        }

        ++songTime; //fram en sample
        if (++tempoTime > TEMPO)
            tempoTime = 0;
        if (++tickTime > TICK)
            tickTime = 0;
    }
}


void updateSound()
{
//#define DEBUG_OUTPUT_BUFFERSTATS

#ifdef DEBUG_OUTPUT_BUFFERSTATS
    int buffersRendered = 0;
#endif // DEBUG_OUTPUT_BUFFERSTATS

    int cursorPos, cursorBuffer, prevBuffer;

    MMTIME mmt;
    mmt.wType = TIME_BYTES;
    waveOutGetPosition(waveOutHandle, &mmt, sizeof(MMTIME));

    //cursorPos = mmt.u.cb / 4;
    //cursorPos %= SOUND_TOTAL_BUFFER_SIZE;
    cursorPos = mmt.u.cb >> 2;
    cursorPos &= SOUND_TOTAL_BUFFER_SIZE_MASK;

    //cursorBuffer = cursorPos / SOUND_BUFFER_SIZE;
    //prevBuffer   = cursorBuffer - 1;
    //if (prevBuffer < 0)
    //    prevBuffer = SOUND_NUMBUFFERS - 1;
    cursorBuffer = cursorPos >> SOUND_BUFFER_SIZE_BITS;
    prevBuffer = (cursorBuffer + SOUND_NUMBUFFERS - 1) & SOUND_NUMBUFFERS_MASK;

    /*
    if (lastBufferRendered != prevBuffer)
    {
        sound_fillBuffer(prevBuffer);
        lastBufferRendered = prevBuffer;
    }
    */
    while (lastBufferRendered != prevBuffer)
    {
        lastBufferRendered = (lastBufferRendered + 1) & SOUND_NUMBUFFERS_MASK;
        sound_fillBuffer(lastBufferRendered);
#ifdef DEBUG_OUTPUT_BUFFERSTATS
        ++buffersRendered;
#endif // DEBUG_OUTPUT_BUFFERSTATS
    }

#ifdef DEBUG_OUTPUT_BUFFERSTATS
    {
        char str[1024];
        sprintf(str, "%d %d %d %d\n", mmt.u.cb / 4 / SOUND_BUFFER_SIZE, cursorBuffer, lastBufferRendered, buffersRendered);
        OutputDebugString(str);
    }
#endif // DEBUG_OUTPUT_BUFFERSTATS
}


void initSoundSystem()
{
    int i;

    pcmwf.wFormatTag = WAVE_FORMAT_PCM;
    pcmwf.nChannels = 2;
    pcmwf.wBitsPerSample = 16;
    pcmwf.nBlockAlign = pcmwf.nChannels * pcmwf.wBitsPerSample / 8;
    pcmwf.nSamplesPerSec = 44100;
    pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;

    waveOutOpen(&waveOutHandle, WAVE_MAPPER, &pcmwf, 0, 0, 0);

    // CREATE AND START LOOPING WAVEOUT BLOCK
    // stereo (16 bits)

    // array is already initialized to 0
    //ZeroMemory(delayBuffer, DELAYSIZE * sizeof(int));

    waveHdr.lpData = (LPSTR)mixBuffer;
    waveHdr.dwBufferLength = SOUND_TOTAL_BUFFER_SIZE * 2 * 2;
    waveHdr.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
    waveHdr.dwLoops = -1;
    waveOutPrepareHeader(waveOutHandle, &waveHdr, sizeof(WAVEHDR));

    // fill sound-buffers

    for (i = 0; i < SOUND_NUMBUFFERS; ++i)
        sound_fillBuffer(i);

    n1 = 0;
#ifdef USE_HIHAT
    n2 = 0;
#endif
//    songPos = 18 * PATTERNLENGTH;

    /*
    // testing..
    for (i = 0; i < 10; ++i)
    {
        char str[1024];
        sprintf(str, "%d\n", patterns[i][0]);
        OutputDebugString(str);
    }
    */
}


void startSound()
{
    waveOutWrite(waveOutHandle, &waveHdr, sizeof(WAVEHDR));
}


#define PAD_RESO 100
#define PAD_VOLUME 180
#define PAD_FADEOUT 220

#ifdef USE_PAD_CHORDS
#define CHORD_RESO 240
#define CHORD_VOLUME 60
#define CHORD_FADEOUT 80
#define CHORD_TRANSPOSE (3 * 12)
#define CHORD_TYPE 1
#define CHORD_LFO 1
#define CHORD_ENVSHIFT 24
#define CHORD_ENVSHIFT2 17
#define CHORD_DETUNE 4
#define CHORD_INITPOS2 0
#define CHORD_VIBSPEED 1600
#define CHORD_VIBRANGE 400000
#else
#define CHORD_RESO 240
#define CHORD_VOLUME 160
#define CHORD_FADEOUT (6 << 8)
#define CHORD_TRANSPOSE (3 * 12)
#define CHORD_TYPE 0
#define CHORD_LFO 1
#define CHORD_ENVSHIFT 13
#define CHORD_ENVSHIFT2 16
#define CHORD_DETUNE 10
#define CHORD_INITPOS2 (12288 << 18)
#define CHORD_VIBSPEED 1600
#define CHORD_VIBRANGE 400000
#endif

MelodyChannel chans[NUMCHANNELS] =
{
    /* PLUCK */
    {
        /*cutoff*/      0,
        /*reso*/        120, 
        /*transpose*/   (2 * 12), 
        /*lfo*/         0, 
        /*type*/        1, 
        /*envshift*/    14, 
        /*envshift2*/   13, 
        /*detune*/      6,
        /*volume*/      180, 
        0, 
        /*fadeout*/     (10 << 8), 
#ifdef USE_VIBRATO
        /*vibSpeed*/    1600,
        /*vibRange*/    150000,
#endif
        /*initPos2*/    (8192 << 18)
    },

    /* CHORD1 */
    {
        /*cutoff*/      0,
        /*reso*/        CHORD_RESO, 
        /*transpose*/   CHORD_TRANSPOSE, 
        /*lfo*/         CHORD_LFO, 
        /*type*/        CHORD_TYPE, 
        /*envshift*/    CHORD_ENVSHIFT, 
        /*envshift2*/   CHORD_ENVSHIFT2, 
        /*detune*/      CHORD_DETUNE,
        /*volume*/      CHORD_VOLUME, 
        0, 
        /*fadeout*/     CHORD_FADEOUT, 
#ifdef USE_VIBRATO
        /*vibSpeed*/    CHORD_VIBSPEED,
        /*vibRange*/    CHORD_VIBRANGE,
#endif
        /*initPos2*/    CHORD_INITPOS2
    },

    /* CHORD2 */
    {
        /*cutoff*/      0,
        /*reso*/        CHORD_RESO, 
        /*transpose*/   CHORD_TRANSPOSE, 
        /*lfo*/         CHORD_LFO, 
        /*type*/        CHORD_TYPE, 
        /*envshift*/    CHORD_ENVSHIFT, 
        /*envshift2*/   CHORD_ENVSHIFT2, 
        /*detune*/      CHORD_DETUNE,
        /*volume*/      CHORD_VOLUME, 
        0, 
        /*fadeout*/     CHORD_FADEOUT, 
#ifdef USE_VIBRATO
        /*vibSpeed*/    CHORD_VIBSPEED,
        /*vibRange*/    CHORD_VIBRANGE,
#endif
        /*initPos2*/    CHORD_INITPOS2
    },

    /* CHORD3 */
    {
        /*cutoff*/      0,
        /*reso*/        CHORD_RESO, 
        /*transpose*/   CHORD_TRANSPOSE, 
        /*lfo*/         CHORD_LFO, 
        /*type*/        CHORD_TYPE, 
        /*envshift*/    CHORD_ENVSHIFT, 
        /*envshift2*/   CHORD_ENVSHIFT2, 
        /*detune*/      CHORD_DETUNE,
        /*volume*/      CHORD_VOLUME, 
        0, 
        /*fadeout*/     CHORD_FADEOUT, 
#ifdef USE_VIBRATO
        /*vibSpeed*/    CHORD_VIBSPEED,
        /*vibRange*/    CHORD_VIBRANGE,
#endif
        /*initPos2*/    CHORD_INITPOS2
    },

    /* PAD OCTAVE 1 */
    {
        /*cutoff*/      0, 
        /*reso*/        PAD_RESO,
#ifdef USE_PAD_CHORDS
        /*transpose*/   (-1 * 12),
#else
        /*transpose*/   0,
#endif
        /*lfo*/         1, 
        /*type*/        1,
        /*envshift*/    24,
        /*envshift2*/   17, 
        /*detune*/      6,
        /*volume*/      PAD_VOLUME,
        0,
        /*fadeout*/     PAD_FADEOUT,
#ifdef USE_VIBRATO
        /*vibSpeed*/    1600,
        /*vibRange*/    150000,
#endif
        /*initPos2*/    (8192 << 18)
    },

    /* PAD OCTAVE 2 */
    {
        /*cutoff*/      0, 
        /*reso*/        PAD_RESO,
#ifdef USE_PAD_CHORDS
        /*transpose*/   0,
#else
        /*transpose*/   (1 * 12),
#endif
        /*lfo*/         1, 
        /*type*/        1,
        /*envshift*/    24,
        /*envshift2*/   17, 
        /*detune*/      6,
        /*volume*/      PAD_VOLUME,
        0,
        /*fadeout*/     PAD_FADEOUT,
#ifdef USE_VIBRATO
        /*vibSpeed*/    1600,
        /*vibRange*/    150000,
#endif
        /*initPos2*/    (8192 << 18)
    },

    /* BASSDRUM */
    {
        /*cutoff*/      0, 
        /*reso*/        0,
        /*transpose*/   (6 * 12),
        /*lfo*/         0, 
        /*type*/        1,
        /*envshift*/    24,
        /*envshift2*/   16,
        /*detune*/      0,
        /*volume*/      200,
        0,
        /*fadeout*/     (17 << 8),
#ifdef USE_VIBRATO
        /*vibSpeed*/    0,
        /*vibRange*/    0,
#endif
        /*initPos2*/    (8192 << 18)
    },

    /* SNARE */
    {
        /*cutoff*/      0, 
        /*reso*/        230,
        /*transpose*/   (5 * 12),
        /*lfo*/         0, 
        /*type*/        1,
        /*envshift*/    24,
        /*envshift2*/   16,
        /*detune*/      0,
        /*volume*/      150,
        0,
        /*fadeout*/     (10 << 8),
#ifdef USE_VIBRATO
        /*vibSpeed*/    0,
        /*vibRange*/    0,
#endif
        /*initPos2*/    (8192 << 18)
    },

#ifdef USE_HIHAT
    /* HIHAT */
    {
        /*cutoff*/      0, 
        /*reso*/        0,
        /*transpose*/   (7 * 12),
        /*lfo*/         0, 
        /*type*/        1,
        /*envshift*/    24,
        /*envshift2*/   15,
        /*detune*/      0,
        /*volume*/      165,
        0,
        /*fadeout*/     (60 << 8),
#ifdef USE_VIBRATO
        /*vibSpeed*/    0,
        /*vibRange*/    0,
#endif
        /*initPos2*/    0
    },
#endif
};


#define L1 1
#define L2 2
#define L3 3
#define L4 4
#define L5 5
#define L6 6
#define L7 7
#define L8 8
#define L9 9

#ifdef USE_PAD_CHORDS
#define C_G4 10
#define C_As4 11
#define C_F5 12
#define C_A4 13
#define C_G5 14
#define C_A5 15
#define C_D5 16
#define C_Ds5 17

#define B1 18
#define B2 19
#define S1 20
#define S2 21
#define H 22
#else
#define P1 10
#define P2 11

#define C_G4 12
#define C_As4 13
#define C_F5 14
#define C_A4 15
#define C_G5 16
#define C_A5 17
#define C_D5 18
#define C_Ds5 19

#define B1 20
#define B2 21
#define S1 22
#define S2 23
#define H 24
#endif


// song orders with pattern for each channel 
// time goes horizontally, 0 = no pattern
char seq[NUMCHANNELS][SONGLENGTH] =
{
    { L1, 0,L2, 0,L2, 0,L3, 0,L4, 0,L5, 0,L2, 0,L3, 0,L4, 0,L5, 0,L2, 0,L3, 0,L4, 0,L5, 0,L6, 0,L7, 0,L8, 0,L9, 0,L1, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,C_G4, 0,C_G4, 0,C_G4, 0,C_G4, 0,C_As4, 0,C_As4, 0,C_As4, 0,C_As4, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,C_As4, 0,C_A4, 0,C_As4, 0,C_As4, 0,C_D5, 0,C_D5, 0,C_Ds5, 0,C_Ds5, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,C_F5, 0,C_F5, 0,C_G5, 0,C_A5, 0,C_G5, 0,C_F5, 0,C_G5, 0,C_F5, 0, 0, 0, 0, 0, 0 },
#ifdef USE_PAD_CHORDS
    {  0, 0, 0, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0, 0 },
    {  0, 0, 0, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0,C_Ds5, 0,C_Ds5, 0,C_G5, 0,C_G5, 0, 0 },
#else
    {  0, 0, 0, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0, 0 },
    {  0, 0, 0, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0,P2, 0,P2, 0,P1, 0,P1, 0, 0 },
#endif
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,B1,B1,B2,B1,B1,B1,B2,B1,B1,B1,B2,B1,B1,B1,B2,B1,B1,B1,B2,B1,B1,B1,B2,B1, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,S1,S1,S1,S2,S1,S1,S1,S2,S1,S1,S1,S2,S1,S1,S1,S2,S1,S1,S1,S2,S1,S1,S1,S2, 0, 0, 0, 0, 0 },
#ifdef USE_HIHAT
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, 0, 0, 0, 0, 0 },
#endif
};

// patterns (x = no note)

#define BASSDRUM C1 | HS
#define SNARE D2 | HS
#define HIHAT1 Cs1 | HS
#define HIHAT2 C1 | HS

char patterns[NUMPATTERNS][PATTERNLENGTH] = 
{
    // sekv
    { G4 | HS, xx, xx, xx, As4, xx, xx, xx, A4 , xx, xx, xx, xx, xx, xx, xx },
    { G4     , xx, xx, xx, As4, xx, xx, xx, A4 , xx, xx, xx, xx, xx, xx, xx },
    { A4     , xx, xx, xx, C5 , xx, xx, xx, As4, xx, xx, xx, xx, xx, xx, xx },
    { C5     , xx, xx, xx, G5 , xx, xx, xx, F5 , xx, xx, xx, xx, xx, xx, xx },
    { A5     , xx, xx, xx, As5, xx, xx, xx, G5 , xx, xx, xx, xx, xx, xx, xx },
    { A5     , xx, xx, xx, C6 , xx, xx, xx, As5, xx, xx, xx, xx, xx, xx, xx },
    { C6     , xx, xx, xx, Ds6, xx, xx, xx, D6 , xx, xx, xx, xx, xx, xx, xx },
    { G6     , xx, xx, xx, F6 , xx, xx, xx, As5, xx, xx, xx, xx, xx, xx, xx },
    { Ds6    , xx, xx, xx, D6 , xx, xx, xx, G5 , xx, xx, xx, xx, xx, xx, xx },

    // chords
#ifdef USE_PAD_CHORDS
    { G4 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { As4 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { F5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { A4 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { G5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { A5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { D5 | HS , xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { Ds5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
#else
    // bass pad
    { G1 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { Ds1 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },

    { xx, xx, xx, xx, xx, xx, G4 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, As4 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, F5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, A4 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, G5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, A5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, D5 | HS , xx, xx, xx, xx, xx, xx, xx, xx, xx },
    { xx, xx, xx, xx, xx, xx, Ds5 | HS, xx, xx, xx, xx, xx, xx, xx, xx, xx },
#endif

    // drums
    { BASSDRUM, xx, xx, xx, xx, xx, xx, xx, xx, xx, BASSDRUM, xx, xx, BASSDRUM, xx, xx },
    { BASSDRUM, xx, xx, xx, xx, xx, xx, xx, BASSDRUM, xx, BASSDRUM, xx, xx, BASSDRUM, xx, xx },
    { xx, xx, xx, xx, SNARE, xx, xx, xx, xx, xx, xx, xx, SNARE, xx, xx, xx },
    { xx, xx, xx, xx, SNARE, xx, xx, xx, xx, xx, xx, xx, SNARE, xx, xx, SNARE },
#ifdef USE_HIHAT
    { HIHAT1, HIHAT2, HIHAT1, HIHAT2, HIHAT1, HIHAT2, HIHAT1, HIHAT2, HIHAT1, HIHAT2, HIHAT1, HIHAT2, HIHAT1, HIHAT2, HIHAT1, HIHAT2 },
#endif
    //{ xx,xx, xx, xx, xx,xx, xx, xx, xx, xx, xx, xx, xx,xx, xx, xx, xx,xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx,xx, xx, xx },
};
