/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-09-27
 * Last update: 2019-07-16
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Blink a LED with delay function.
 * TODO: Verify functionality of LED blinker.
 * NOTE: Macro _BV converts a bit number into a byte value (see AVR Libc
 * Reference Manual).
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN1    PB0
#define LED_PIN     PB5
#define BLINK_DELAY 100
#define BTN     PD2

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/

int main(void)
{
    /* Set output pin */
    GPIO_config_output (&DDRB,LED_PIN);
    //DDRB |= _BV(LED_PIN);
    GPIO_config_output (&DDRB,LED_PIN1);            
    //DDRB |= _BV(LED_PIN1);          
    DDRD &= ~_BV(BTN);
    PORTD |= _BV(BTN);

    /* Turn LED off */
    GPIO_write (&PORTB,LED_PIN,1);
    GPIO_write (&PORTB,LED_PIN1,0);

    //PORTB &= ~_BV(LED_PIN);          
    //DDRD &= ~_BV(BTN); 

    /* Infinite loop */
     for (;;)
    {
      if (bit_is_clear(PIND, BTN)){
        /* Invert LED and delay */
        GPIO_toggle (&PORTB,LED_PIN);
        //PORTB ^= _BV(LED_PIN); 
        GPIO_toggle (&PORTB,LED_PIN1);     
        //PORTB ^= _BV(LED_PIN1);
        _delay_ms(BLINK_DELAY);}

    }

    return (0);
}
