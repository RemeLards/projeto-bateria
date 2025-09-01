#include "audio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// #include "sound/Bass_Drum_1_channel1.h"
#include "sound/Bass_Drum_2_channel1.h"
#include "sound/Bass_Drum_1_channel1.c"
#include "em_device.h"
#include "gpio.h"
#include "clock_efm32gg2.h"
#include "pwm.h"


static uint16_t* choosen_song = NULL;
static int song_counter = 0;
static int song_limit = 0;
static uint8_t stop_music = 0; 

void change_audio(int genre)
{
    if ( genre == 0)
    {
        // choosen_song = BASSDRUM1;
        // song_limit = BASSDRUM1_SIZE;
        choosen_song = NULL;
    }
    else if (genre == 1)
    {
        choosen_song = BASSDRUM2;
        song_limit = BASSDRUM2_SIZE;
    }
    song_counter=0;
}

void play_audio()
{
    if (choosen_song && !stop_music)
    {
        if (song_counter > song_limit) song_counter = 0; //Resets the song
        PWM_Write(TIMER3,2,choosen_song[song_counter++]);
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