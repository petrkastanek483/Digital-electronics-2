/* Couvaci senzory 
   Autoři: Petr Kaštánek, Martin Krejčí
   Pomocí ultrazvukového vysílače měřit vzdálenost od překážky a zobrazovat
   na 7mi segmentovém displejji, pomocí rozsvěcení diod a zvukovým projevem bzučáku */

 /* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer.h"
#include "uart.h"
#include "gpio.h"
#include "lcd.h"
#include "segment.h"
#include "twi.h"
#include "uart.h"
/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/

#define LED_D1          PB5 //13
#define LED_D2          PB4 //12
#define LED_D3          PB3 //11
#define LED_D4          PB2 //10

#define buzz            PD3 //3

#define echo            PD0 //5
#define trig            PD1 //6


volatile uint16_t VZ = 0; 
volatile uint16_t vzd = 0;
volatile uint16_t OMEZ = 100;
uint8_t c;
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program.
 *  Input:  None
 *  Return: None
 */

 /* Trigger pulse */

void sendTrigger(uint8_t pin_numD) {
    GPIO_config_output(&DDRD, trig);
    _delay_us(2);
    GPIO_write(&PORTD, pin_numD, 1);
    _delay_us(10);  // 10 us delay
    GPIO_write(&PORTD, pin_numD, 0);

}

int main(void)
{
    /* D1, D2, D3, D4 led */

    GPIO_config_output(&DDRB, LED_D1);
    GPIO_config_output(&DDRB, LED_D2);
    GPIO_config_output(&DDRB, LED_D3);
    GPIO_config_output(&DDRB, LED_D4);

    GPIO_write(&PORTB, LED_D1, 1);
    GPIO_write(&PORTB, LED_D2, 1);
    GPIO_write(&PORTB, LED_D3, 1);
    GPIO_write(&PORTB, LED_D4, 1);

    /* Trigger */

    GPIO_config_output(&DDRD, trig);
    GPIO_write(&PORTD, trig, 0);

    /* Echo */
    
    GPIO_config_input_pullup(&DDRD, echo);

    /* Buzzer */

    GPIO_config_output(&DDRD, buzz);
    GPIO_write (&PORTD, buzz, 1);

   

    /* 7-segment display interface */
    
    GPIO_config_output(&DDRB, SEGMENT_DATA);
    GPIO_config_output(&DDRD, SEGMENT_CLK);
    GPIO_config_output(&DDRD, SEGMENT_LATCH);
    

    /* Timer 0 configuration for segment switching*/
    TIM_config_prescaler(TIM0, TIM_PRESC_256);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

    /* Timer 1 configuration for distance measure */
    TIM_config_prescaler(TIM1, TIM_PRESC_8);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);


    sei();

    

    /* Infinite loop */
    for (;;) {
        
        if (VZ < OMEZ) {
            if (VZ < 2) {
                PORTD ^= _BV(buzz);      
                GPIO_write(&PORTB, LED_D1, 0);
                GPIO_write(&PORTB, LED_D2, 0);
                GPIO_write(&PORTB, LED_D3, 0);
                GPIO_write(&PORTB, LED_D4, 0);
                _delay_ms(10);     /* Wait for several milisecs */

            }
            else if (VZ < 4) {
                PORTD ^= _BV(buzz);      
                GPIO_write(&PORTB, LED_D1, 0);
                GPIO_write(&PORTB, LED_D2, 0);
                GPIO_write(&PORTB, LED_D3, 0);
                GPIO_write(&PORTB, LED_D4, 1);
                _delay_ms(30);     /* Wait for several milisecs */

            }
            else if (VZ < 8) {
                PORTD ^= _BV(buzz);      
                GPIO_write(&PORTB, LED_D1, 0);
                GPIO_write(&PORTB, LED_D2, 0);
                GPIO_write(&PORTB, LED_D3, 1);
                GPIO_write(&PORTB, LED_D4, 1);
                _delay_ms(50);     /* Wait for several milisecs */

            }

                else if (VZ < 15) {
                PORTD ^= _BV(buzz);      
                GPIO_write(&PORTB, LED_D1, 0);
                GPIO_write(&PORTB, LED_D2, 1);
                GPIO_write(&PORTB, LED_D3, 1);
                GPIO_write(&PORTB, LED_D4, 1);
                _delay_ms(100);     /* Wait for several milisecs */

            }
            else {
                GPIO_write(&PORTD, buzz, 1);
                GPIO_write(&PORTB, LED_D1, 1);
                GPIO_write(&PORTB, LED_D2, 1);
                GPIO_write(&PORTB, LED_D3, 1);
                GPIO_write(&PORTB, LED_D4, 1);
                _delay_ms(100);     /* Wait for several milisecs */

            }
        }
        else {

            GPIO_write(&PORTD, buzz, 1);
            GPIO_write(&PORTB, LED_D1, 1);
            GPIO_write(&PORTB, LED_D2, 1);
            GPIO_write(&PORTB, LED_D3, 1);
            GPIO_write(&PORTB, LED_D4, 1);
            _delay_ms(10);     /* Wait for several milisecs */

        }



    }

    return (0);
}

/**
 *  Brief: Pin Change Interrupt 11:9 routine. Toggle a LED.
 */
ISR(PCINT1_vect)
{
    //GPIO_toggle(&PORTB, LED_D1);
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t segment = 0;
    segment++;
    if (segment >= 4)
    {
        segment = 0;
    }

    uint8_t ones, tens, hundreds, thousands;

    VZ = vzd / 58.31;
    ones = VZ % 10;
    tens = (VZ / 10) % 10;
    hundreds = (VZ / 100) % 10;
    thousands = VZ / 1000;

    switch (segment)
    {
    case 0:
        SEG_putc(ones, segment);
        break;
    case 1:
        SEG_putc(tens, segment);
        break;
    case 2:
        SEG_putc(hundreds, segment);
        break;
    case 3:
        SEG_putc(thousands, segment);
        break;
    default:
        SEG_putc(0, segment);
    }
}

ISR(TIMER1_OVF_vect)
{
    /* Distance measurement */
    uint8_t ReadStatus = 0;

    sendTrigger(trig);
    while (ReadStatus == 0) {
        while (GPIO_read(&PIND, echo)) {
            vzd++;
            ReadStatus = 1;
        }
    }
}

        