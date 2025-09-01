#ifndef _AUDIO_HEADER_
#define _AUDIO_HEADER_
#include <stdint.h>

void play_sample(void);
void change_sample(int sample);
void stop_song(void);
void resume_song(void);
void toggle_song(void);
int estimate_bpm();
void rythm_init();


typedef struct sample
{
    uint8_t* sample_type;
    uint16_t   sample_counter;
    uint16_t   sample_limit;
}SAMPLE;

typedef struct rythm
{
    uint8_t* rythm_type;
    uint16_t   rythm_counter;
    uint16_t  rythm_limit;
    SAMPLE sample;
}RYTHM;



#endif