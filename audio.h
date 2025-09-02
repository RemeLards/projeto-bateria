#ifndef _AUDIO_HEADER_
#define _AUDIO_HEADER_
#include <stdint.h>

uint8_t play_sample(void);
void change_sample(int sample);
void stop_song(void);
void resume_song(void);
void toggle_song(void);
int estimate_bpm(void);
void rythm_init(void);
void change_rythm(uint8_t rythm_choosen);
void play_rythm(void);


typedef struct sample
{
    uint8_t*   sample_type;
    uint32_t   sample_counter;
    uint32_t   sample_limit;
    uint8_t    sample_bpm_mul;   
}SAMPLE; 

typedef struct rythm
{
    uint8_t*   rythm_type;
    uint32_t   rythm_counter;
    uint32_t   rythm_limit;
    uint8_t    rythm_bpm_mult;
    SAMPLE sample;
}RYTHM;



#endif