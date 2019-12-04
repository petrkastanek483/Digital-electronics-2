/***********************************************************************
 * 
 * Implementation of LFSR-based (Linear Feedback Shift Register) 
 * pseudo-random generator in AVR assembly.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2019 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

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

#define f_cpu           16000000UL


#define LED_D1          PB5 //13
#define LED_D2          PB4 //12
#define LED_D3          PB3 //11
#define LED_D4          PB2 //10

#define buzz            PD3 //3
#define BUZZ_DELAY 100

#define echo            PD0 //5
#define trig            PD1 //6


//#define BTN_S1          PC1 // PCINT 9
//#define BTN_S2          PC2 // PCINT 10
//#define BTN_S3          PC3 // PCINT 11

volatile uint16_t VZ = 0;
volatile uint16_t OMEZ = 100;
uint8_t c ;
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on 7-segment display.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    /* D1 led */
    
    GPIO_config_output (&DDRB,LED_D1);
    GPIO_config_output (&DDRB,LED_D2);
    GPIO_config_output (&DDRB,LED_D3);
    GPIO_config_output (&DDRB,LED_D4);

    GPIO_config_output (&DDRD,trig);
    GPIO_config_input_pullup (&DDRD,echo);

    GPIO_config_output (&DDRD,buzz);
    //GPIO_write (&PORTD, buzz, 1);

    GPIO_write (&PORTB, LED_D1, 1);
    GPIO_write (&PORTB, LED_D2, 1);
    GPIO_write (&PORTB, LED_D3, 1);
    GPIO_write (&PORTB, LED_D4, 1);

    /* Pin Change Interrupts 11:9 */
    // TODO: Configure Pin Change Interrupts 11, 10, and 9
    PCICR |= _BV(PCIE1); 
    PCMSK1 |= (_BV(PCINT11) | _BV(PCINT10) | _BV(PCINT9));

    /* 7-segment display interface */
    // TODO: Configure 7-segment display pins
    GPIO_config_output (&DDRB , SEGMENT_DATA) ;
    GPIO_config_output (&DDRD , SEGMENT_CLK) ;
    GPIO_config_output (&DDRD , SEGMENT_LATCH) ;
    /* Enable interrupts by setting the global interrupt mask */

    /* Timer 0 configuration for segment switching*/
    TIM_config_prescaler(TIM0, TIM_PRESC_256);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

    /* Timer 1 configuration for value incrementation */
    TIM_config_prescaler(TIM1, TIM_PRESC_8);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);


    sei();
    uart_puts("Zadejte omezeni vzdalenosti v rozsahu 0 - 200 cm\r\n");


    c = uart_getc () ;
        if ( c != ’ \0 ’) {
        if ( c == ’1 ’) {
    
        }
    }

    /* Infinite loop */
    for (;;) {
        // TODO: Use function to display digit 1 at position 0
        //SEG_putc(9, 3);
        if (VZ < OMEZ) {
            if (VZ < 10) {
            //  PORTD ^= _BV(buzz);      /* PORTB = PORTB xor (0010 0000) */
                GPIO_write (&PORTB, LED_D1, 0);
                GPIO_write (&PORTB, LED_D2, 0);
                GPIO_write (&PORTB, LED_D3, 0);
                GPIO_write (&PORTB, LED_D4, 0);
                _delay_ms(10);     /* Wait for several milisecs */
            
            }
            else if (VZ < 30) {
            //  PORTD ^= _BV(buzz);      /* PORTB = PORTB xor (0010 0000) */
                GPIO_write (&PORTB, LED_D1, 0);
                GPIO_write (&PORTB, LED_D2, 0);
                GPIO_write (&PORTB, LED_D3, 0);
                GPIO_write (&PORTB, LED_D4, 1);
                _delay_ms(30);     /* Wait for several milisecs */
            
            }
            else if (VZ < 50) {
            //  PORTD ^= _BV(buzz);      /* PORTB = PORTB xor (0010 0000) */
                GPIO_write (&PORTB, LED_D1, 0);
                GPIO_write (&PORTB, LED_D2, 0);
                GPIO_write (&PORTB, LED_D3, 1);
                GPIO_write (&PORTB, LED_D4, 1);
                _delay_ms(50);     /* Wait for several milisecs */
            
            }
            else {
            // PORTD ^= _BV(buzz);      /* PORTB = PORTB xor (0010 0000) */
                GPIO_write (&PORTB, LED_D1, 0);
                GPIO_write (&PORTB, LED_D2, 1);
                GPIO_write (&PORTB, LED_D3, 1);
                GPIO_write (&PORTB, LED_D4, 1);
                _delay_ms(100);     /* Wait for several milisecs */
            
            }
        }
        else {
            
          //  PORTD ^= _BV(buzz);      /* PORTB = PORTB xor (0010 0000) */
            GPIO_write (&PORTB, LED_D1, 1);
            GPIO_write (&PORTB, LED_D2, 1);
            GPIO_write (&PORTB, LED_D3, 1);
            GPIO_write (&PORTB, LED_D4, 1);
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
    if(segment >= 4)
    {
        segment = 0;
    }

    uint8_t ones, tens, hundreds, thousands;

    ones = VZ % 10;
    tens = (VZ / 10) % 10;
    hundreds = (VZ / 100) % 10;
    thousands = VZ / 1000;

    switch(segment)
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
    VZ++;
    if (VZ > 9999)
    {
        VZ= 0;
    }
    
}



        