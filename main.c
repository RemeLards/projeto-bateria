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
#include "touch.h"

/*****************************************************************************
 * @brief  SysTick interrupt handler
 *
 * @note   Called every 1/DIVIDER seconds (1 ms)
 */

//{
#define SYSTICKDIVIDER 44100
#define TOUCH_PERIOD 300

static const char start_str[6] = "START";
static const char *genres[] = {"ROCK", "MPB", "SAMBA"};
static short int len_genres = 3;
static short int current_genre = -1;
static char current_bpm[5] = "0000";


void set_bpm_display(uint16_t bpm)
{
    current_bpm[0] = '0' + (bpm / 1000);
    current_bpm[1] = '0' + ((bpm / 100)%10);
    current_bpm[2] = '0' + ((bpm / 10)%10);
    current_bpm[3] = '0' + (bpm % 10);
    current_bpm[4] = '\0';
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

    static uint16_t touchcounter = 0;
    static uint8_t v = 0;
   
    play_rythm();

    /* Touch processing */
    if( touchcounter != 0 ) {
        touchcounter--;
    } else {
        touchcounter = TOUCH_PERIOD;
        Touch_PeriodicProcess();
        v = Touch_Read();
        int tc = Touch_GetCenterOfTouch(v);
        if (tc > 0)
        {
            uint16_t bpm = 0;
            if ( tc == 1 )
            {
                bpm = multiply_bpm(4);
            }
            else if ( tc == 3 )
            {
                bpm = multiply_bpm(3);
            }
            else if ( tc == 5 )
            {
                bpm = multiply_bpm(2);
            }
            else if ( tc == 7)
            {
                bpm = multiply_bpm(1);
            }
            if (bpm > 0)
            {
                set_bpm_display(bpm);
                LCD_WriteNumericDisplay((char*)current_bpm);
            }
        }
    }

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
    }
    if( b&BUTTON2 )
    {
        choose_genre();
        LCD_WriteAlphanumericDisplay((char*)genres[current_genre]);
        uint64_t bpm = estimate_initial_bpm();
        set_bpm_display(bpm);
        LCD_WriteNumericDisplay((char*)current_bpm);
    }
}

void config_ios(void){

    /* Configure LEDs */
    LED_Init(LED1|LED2);


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
    LCD_ClearAll();
    LCD_WriteAlphanumericDisplay((char*)start_str);
    LCD_WriteNumericDisplay((char*)current_bpm);

    // Configure touch sensor 
    Touch_Init();

    // Configure rythm player
    rythm_init();
}

int main(void) {
    

    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    // Configure Every I/O
    config_ios();

    /* Enable interrupts */
    __enable_irq();

    // Configure SysTick
    SysTick_Config(SystemCoreClock/SYSTICKDIVIDER);


    while (1) {
        __WFI();
    }

}







