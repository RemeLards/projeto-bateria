/** ***************************************************************************
 * @file    main.c
 * @brief   Simple LED Blink Demo for EFM32GG_STK3700
 * @version 1.0
******************************************************************************/

#include <stdint.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "em_device.h"
#include "led.h"
#include "lcd.h"
#include "pwm.h"
#include "button.h"
#include "clock_efm32gg2.h"
#include "audio.h"

/*****************************************************************************
 * @brief  SysTick interrupt handler
 *
 * @note   Called every 1/DIVIDER seconds (1 ms)
 */

//{
#define SYSTICKDIVIDER 44100
#define SOFTDIVIDER 44100

static const char start_str[6] = "start";
static const char *genres[] = {"ROCK", "MPB", "SAMBA"};
static short int len_genres = 3;
static short int current_genre = -1;
static char current_bpm[4] = "000";


void set_bpm_display(uint8_t bpm)
{
    current_bpm[0] = '0' + (bpm / 100);
    current_bpm[1] = '0' + ((bpm / 10) % 10);
    current_bpm[2] = '0' + (bpm % 10);
    current_bpm[3] = '\0';
}


void choose_genre(void){
    if (current_genre == -1) current_genre = 0;
    else if (++current_genre > len_genres-1)
    {
        current_genre=0;
    }
    change_rythm(current_genre);
}

void SysTick_Handler(void) {

   //play_sample();
   play_rythm();

}



/*****************************************************************************
 * @brief  Main function
 *
 * @note   Using default clock configuration
 *         HFCLK = HFRCO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 */


#define DELAYVAL 2


void buttoncallback(uint32_t v)
{
    uint32_t b = Button_ReadReleased();
    if( b&BUTTON1 ) {
        
        toggle_song();
        LED_Toggle(LED1);

    }
    if( b&BUTTON2 )
    {
        choose_genre();
        LED_Toggle(LED2);
        LCD_WriteAlphanumericDisplay((char*)genres[current_genre]);
        uint64_t bpm = estimate_initial_bpm();
        set_bpm_display(bpm);
        LCD_WriteNumericDisplay((char*)current_bpm);
    }
}

void config_ios(void){

    /* Configure LEDs */
    LED_Init(LED1|LED2);
    LED_Toggle(LED1);
    

    /* Configure Buttons */
    Button_Init(BUTTON1|BUTTON2);
    Button_SetCallback(buttoncallback);


    /* Configure PWM */
    PWM_Init(TIMER3,PWM_LOC1,PWM_PARAMS_CH2_ENABLEPIN);
    PWM_Write(TIMER3,2,0);
    PWM_Start(TIMER3);

    /* Configure LCD */
    LCD_Init();
    LCD_SetAll();
    // Delay(DELAYVAL);

    LCD_ClearAll();
    // Delay(DELAYVAL);
    LCD_WriteAlphanumericDisplay((char*)start_str);
    LCD_WriteNumericDisplay((char*)current_bpm);

    rythm_init();
}

int main(void) {
    

    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    config_ios();


    /* Enable interrupts */
    __enable_irq();

    // Configure SysTick
    SysTick_Config(SystemCoreClock/SYSTICKDIVIDER);


    while (1) {
        __WFI();
    }

}







