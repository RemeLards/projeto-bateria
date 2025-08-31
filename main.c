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

/*****************************************************************************
 * @brief  SysTick interrupt handler
 *
 * @note   Called every 1/DIVIDER seconds (1 ms)
 */

//{
#define SYSTICKDIVIDER 44100
#define SOFTDIVIDER 44100

static const char *genres[] = {"ROCK", "POP"};
static short int len_genres = 2;
static short int current_genre = 0;


// static uint32_t ticks = 0;


void choose_genre(void){

    if (++current_genre > len_genres-1)
    {
        current_genre=0;
    }
}

void SysTick_Handler(void) {
    static int counter = 0;             // must be static

    // ticks++;                        // overflow in 49 days

    /* Blink */
    if( counter != 0 )
    {
        counter--;
    }

    else
    {
        counter = SOFTDIVIDER-1;
    }
}

//}


/*****************************************************************************
 * @brief  Delay function based on SysTick
 *****************************************************************************/


// void
// Delay(uint32_t v) {
// uint32_t lim = ticks+v;       // Missing processing of overflow here

//     while ( ticks < lim ) {}

// }



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
    // if( b&BUTTON1 ) {
    //     LED_Toggle(LED1);
    // }
    if( b&BUTTON2 )
    {
        LED_Toggle(LED2);
        LCD_WriteAlphanumericDisplay((char*)genres[current_genre]);
        choose_genre();
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
    // Delay(DELAYVAL);

    LCD_ClearAll();
    // Delay(DELAYVAL);
}

int main(void) {
    

    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    config_ios();

    // Configure SysTick
    SysTick_Config(SystemCoreClock/SYSTICKDIVIDER);


    int cnt=0;
    unsigned val = 0;
    while (1) {
        // printf("counter=%5X status=%5X value=%5X top=%5X\n",TIMER3->CNT,
        //                 TIMER3->STATUS,
        //                 TIMER3->CC[2].CCV,
        //                 TIMER3->TOP);
        if( cnt-- == 0 ) {
            LED_Toggle(LED1); // Trying to control LED thru GPIO
            cnt = SYSTICKDIVIDER;
            val += 0x1000;
            if( val > 0XFFFF ) val = 0;
            PWM_Write(TIMER3,2,val);
        }

    }

}







