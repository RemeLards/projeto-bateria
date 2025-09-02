#include "audio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// #include "sound/Bass_Drum_1_channel1.h"
#include "sound/Bass_Drum_2_channel1.c"
#include "sound/Clap_channel1.c"
#include "sound/Maracas_channel1.c"
#include "sound/High_Bongo_channel1.c"
#include "em_device.h"
#include "gpio.h"
#include "clock_efm32gg2.h"
#include "pwm.h"

#define IGNORE_RYTHM_COUNTER 0
#define USE_SINE_TEST 0

typedef enum
{
    CLAP_TYPE = 0,
    BASSDRUM2_TYPE,
    MARACAS_TYPE,
    HIGH_BONGO_TYPE,
}SONG_TYPES;

typedef enum
{
    ROCK = 0,
    MPB,
    SAMBA,
}RYTHM_TYPES;

#define CLAP_BPM_MUL 1
#define BASSDRUM2_BPM_MUL 2
#define MARACAS_BPM_MUL 1
#define HIGH_BONGO_BPM_MUL 1


#define SAMPLE_RATE 44100


RYTHM rythm;

void rythm_init()
{
    rythm.rythm_counter = 0;
    rythm.rythm_limit = 0;
    rythm.rythm_type = NULL;
    rythm.rythm_bpm_mult = 1;

    rythm.sample.sample_counter = 0;
    rythm.sample.sample_limit = 0;
    rythm.sample.sample_type = NULL;
    rythm.sample.sample_bpm_mul = 1;
}


#define ROCK_LIMIT 2
uint8_t rock[3] = {BASSDRUM2_TYPE,BASSDRUM2_TYPE,CLAP_TYPE};

#define MPB_LIMIT 2
uint8_t mpb[3] = {CLAP_TYPE,CLAP_TYPE,BASSDRUM2_TYPE};

#define SAMBA_LIMIT 9
uint8_t samba[10] = {MARACAS_TYPE, MARACAS_TYPE, MARACAS_TYPE, MARACAS_TYPE, MARACAS_TYPE,
                  MARACAS_TYPE, MARACAS_TYPE, HIGH_BONGO_TYPE, HIGH_BONGO_TYPE, HIGH_BONGO_TYPE};

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


void change_rythm(uint8_t rythm_choosen)
{
    if ( rythm_choosen == ROCK)
    {
        rythm.rythm_type = rock;
        rythm.rythm_limit = ROCK_LIMIT;
        rythm.rythm_bpm_mult = 1;
    }
    else if (rythm_choosen == MPB)
    {
        rythm.rythm_type = mpb;
        rythm.rythm_limit = MPB_LIMIT;
        rythm.rythm_bpm_mult = 1;
    }
    else if (rythm_choosen == SAMBA)
    {
        rythm.rythm_type = samba;
        rythm.rythm_limit = SAMBA_LIMIT;
        rythm.rythm_bpm_mult = 1;
    }
    rythm.rythm_counter = 0;
    rythm.sample.sample_counter = 0;
    change_sample(rythm.rythm_type[rythm.rythm_counter++]);
}

void play_rythm(void)
{
    if (rythm.rythm_type)
    {
        if(!play_sample())
        {
            if (rythm.rythm_counter > rythm.rythm_limit) rythm.rythm_counter = 0;
            change_sample(rythm.rythm_type[rythm.rythm_counter++]);
        }
    }    
}

void change_sample(int sample)
{
    if ( sample == CLAP_TYPE )
    {
        #if USE_SINE_TEST
        rythm.sample.sample_type = (uint8_t*)sine_table;
        rythm.sample.sample_limit = 99;
        #else
        rythm.sample.sample_type = (uint8_t*) CLAP;
        rythm.sample.sample_limit = CLAP_SIZE;
        rythm.sample.sample_bpm_mul = CLAP_BPM_MUL;
        #endif
    }
    else if ( sample == BASSDRUM2_TYPE )
    {
        rythm.sample.sample_type = (uint8_t*) BASSDRUM2;
        rythm.sample.sample_limit = BASSDRUM2_SIZE;
        rythm.sample.sample_bpm_mul = BASSDRUM2_BPM_MUL;
    }
    else if ( sample == MARACAS_TYPE )
    {
        rythm.sample.sample_type = (uint8_t*) MARACAS;
        rythm.sample.sample_limit = MARACAS_SIZE;
        rythm.sample.sample_bpm_mul = MARACAS_BPM_MUL;
    }
    else if ( sample == HIGH_BONGO_TYPE )
    {
        rythm.sample.sample_type = (uint8_t*) HIGHBONGO;
        rythm.sample.sample_limit = HIGHBONGO_SIZE;
        rythm.sample.sample_bpm_mul = HIGH_BONGO_BPM_MUL;
    }

    rythm.sample.sample_counter=0;
}

uint8_t play_sample(void)
{
    if (rythm.sample.sample_counter > rythm.sample.sample_limit)
    {
        rythm.sample.sample_counter = 0; //Resets the song
        return 0;
    }
    
    if(!stop_music)
    {
        if (rythm.sample.sample_type)
        {

            uint8_t pwm_val = rythm.sample.sample_type[rythm.sample.sample_counter];
            rythm.sample.sample_counter += (1 * rythm.sample.sample_bpm_mul * rythm.rythm_bpm_mult);


            PWM_Write(TIMER3,2,pwm_val);
        }
    }
    return 1;
}

void toggle_song()
{
    if (stop_music) resume_song();
    else stop_song();
}

void stop_song()
{
    stop_music = 1;
    PWM_Write(TIMER3,2,0); //stop any sound
}

void resume_song()
{
    stop_music = 0;
}

uint16_t estimate_initial_bpm()
{
    if (!rythm.rythm_type) return 0;

    int beats = (int)rythm.rythm_limit + 1; 
    if (beats <= 0) return 0;

    double sample_size = 0.0;

    for (int i = 0; i < beats; ++i)
    {
        switch (rythm.rythm_type[i])
        {
            case CLAP_TYPE:
                sample_size += (double)CLAP_SIZE / (double)CLAP_BPM_MUL;
                break;

            case BASSDRUM2_TYPE:
                sample_size += (double)BASSDRUM2_SIZE / (double)BASSDRUM2_BPM_MUL;
                break;

            case MARACAS_TYPE:
                sample_size += (double)MARACAS_SIZE / (double)MARACAS_BPM_MUL;
                break;

            case HIGH_BONGO_TYPE:
                sample_size += (double)HIGHBONGO_SIZE / (double)HIGH_BONGO_BPM_MUL;
                break;
        }
    }

    double duration_seconds = sample_size / (double)SAMPLE_RATE; // tempo total em s

    double bpm = ((double)beats / duration_seconds) * 60.0;

    rythm.original_bpm = (uint16_t) ((bpm + 0.5)) ; // arredonda ao inteiro mais próximo
    return rythm.original_bpm;
}

int16_t multiply_bpm(uint8_t value)
{
    rythm.rythm_bpm_mult = value;
    return rythm.original_bpm*rythm.rythm_bpm_mult;
}