#ifndef _AUDIO_HEADER_
#define _AUDIO_HEADER_
#include <stdint.h>

uint8_t play_sample(void);
void change_sample(int sample);
void stop_song(void);
void resume_song(void);
void toggle_song(void);
uint16_t estimate_initial_bpm(void);
void rythm_init(void);
void change_rythm(uint8_t rythm_choosen);
void play_rythm(void);


typedef struct sample
{
    uint8_t*    sample_type;
    uint32_t    sample_counter;
    uint32_t    sample_limit;
    uint8_t     sample_bpm_mul;   
}SAMPLE; 

typedef struct rythm
{
    uint8_t*    rythm_type;
    uint32_t    rythm_counter;
    uint32_t    rythm_limit;
    uint8_t     rythm_bpm_mult; //Desisti da feature quando fui fazer o calculo do BPM, se der tempo corrijo
    uint16_t    original_bpm;
    SAMPLE sample;
}RYTHM;



#endif