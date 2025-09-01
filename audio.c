#include "audio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// #include "sound/Bass_Drum_1_channel1.h"
#include "sound/Bass_Drum_2_channel1.c"
#include "sound/Claves_channel1.c"
#include "em_device.h"
#include "gpio.h"
#include "clock_efm32gg2.h"
#include "pwm.h"

#define IGNORE_RYTHM_COUNTER 0
#define USE_SINE_TEST 0

#define CLAVES_TYPE 0
#define BASSDRUM2_TYPE 1

#define BLOCK_SIZE 256
#define SAMPLE_RATE 44100
#define THRESHOLD_FACTOR 1.5f


RYTHM rythm;

void rythm_init()
{
    rythm.rythm_counter=0;
    rythm.rythm_limit=0;
    rythm.rythm_type=NULL;

    rythm.sample.sample_counter=0;
    rythm.sample.sample_limit=0;
    rythm.sample.sample_type=NULL;
}


uint8_t rock[3] = {BASSDRUM2_TYPE,BASSDRUM2_TYPE,CLAVES_TYPE};
uint8_t pop[3] = {CLAVES_TYPE,CLAVES_TYPE,BASSDRUM2_TYPE};

static uint8_t* choosen_sample = NULL;
static uint8_t* choosen_rythm = NULL;
static int song_counter = 0;
static int song_limit = 0;
static uint8_t stop_music = 0; 

static const uint8_t sine_table[100] = {
    63, 67, 71, 75, 79, 83, 86, 90, 94, 97,
    100, 103, 106, 109, 112, 114, 117, 119, 120, 122,
    123, 125, 125, 126, 126, 127, 126, 126, 125, 125,
    123, 122, 120, 119, 117, 114, 112, 109, 106, 103,
    100, 97, 94, 90, 86, 83, 79, 75, 71, 67,
    63, 59, 55, 51, 47, 43, 40, 36, 32, 29,
    26, 23, 20, 17, 14, 12, 9, 7, 6, 4,
    3, 1, 1, 0, 0, 0, 0, 0, 1, 1,
    3, 4, 6, 7, 9, 12, 14, 17, 20, 23,
    26, 29, 32, 36, 40, 43, 47, 51, 55, 59};


void change_rythm(uint8_t rythm)
{
    if ( rythm == 0)
    {
        choosen_rythm = rock;
    }
    else if (rythm == 1)
    {
        choosen_sample = pop;
    }
    song_counter=0;
}

void change_sample(int sample)
{
    if ( sample == 0)
    {
        #if USE_SINE_TEST
        rythm.sample.sample_type = (uint8_t*)sine_table;
        rythm.sample.sample_limit = 99;
        #else
        rythm.sample.sample_type = CLAVES;
        rythm.sample.sample_limit = CLAVES_SIZE;
        #endif
    }
    else if (sample == 1)
    {
        rythm.sample.sample_type = BASSDRUM2;
        rythm.sample.sample_limit = BASSDRUM2_SIZE;
    }
    rythm.sample.sample_counter=0;
}

void play_sample()
{
    if (rythm.sample.sample_counter > rythm.sample.sample_limit) rythm.sample.sample_counter = 0; //Resets the song
    
    if(!stop_music)
    {
        if (rythm.sample.sample_type)
        {

            uint8_t pwm_val = rythm.sample.sample_type[rythm.sample.sample_counter++];

            PWM_Write(TIMER3,2,pwm_val);
        }
    }
}

void toggle_song()
{
    if (stop_music) resume_song();
    else stop_song();
}

void stop_song()
{
    stop_music = 1;
}

void resume_song()
{
    stop_music = 0;
}

int estimate_bpm()
{
    if (!rythm.sample.sample_type || rythm.sample.sample_limit == 0) return 0;

    int beats = 0;
    int block_energy_prev = 0;
    int threshold = 60; // ajuste conforme amplitude

    for (int i = 0; i < rythm.sample.sample_limit; i += BLOCK_SIZE) {
        int max_val = 0;
        for (int j = 0; j < BLOCK_SIZE && (i+j) < rythm.sample.sample_limit; j++) {
            if (rythm.sample.sample_type[i+j] > max_val) max_val = rythm.sample.sample_type[i+j];
        }

        if (max_val > threshold && max_val > block_energy_prev) {
            beats++;
        }
        block_energy_prev = max_val;
    }

    float seconds = (float)rythm.sample.sample_limit / SAMPLE_RATE;
    float bpm = (beats / seconds) * 60.0f;
    return (int)bpm;
}