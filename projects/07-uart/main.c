/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-23
 * Last update: 2019-11-01
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Analog-to-digital conversion with displaying result on LCD and 
 *    transmitting via UART.
 * 
 * Note:
 *    Peter Fleury's UART library.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>             // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Read ADC result and transmit via UART.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // LCD display
    lcd_init(LCD_DISP_ON);
    lcd_puts("LCD ERROR");
    /* ADC
     * TODO: Configure ADC reference, clock source, enable ADC module, 
     *       and enable conversion complete interrupt */
    ADMUX |=_BV(REFS0);
    ADCSRA |=_BV(ADPS2) | _BV(ADPS1)| _BV(ADPS0);
    ADCSRA|=_BV(ADEN);
    ADCSRA|=_BV(ADIE);
    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);              //rychlost čítání
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put string to ringbuffer for transmitting via UART.
    uart_puts("UART testing\r\n");

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Start ADC conversion.
 */
ISR(TIMER1_OVF_vect)
{
    // TODO: Start ADC conversion
    ADCSRA|=_BV(ADSC);

}

/**
 *  Brief: ADC complete interrupt routine. Update LCD and UART 
 *         transmiter.
 */
ISR(ADC_vect)
{
 uint16_t value = 0;
    char uart_string[4];

	// Read 10-bit ACD value
     value = ADC;

    // TODO: Update LCD and UART transmiter
    itoa(value, uart_string, 10);
    //uart_puts ( " \033[0 m " ) ;
    //uart_puts ( " \033[4;32 m " ) ;
    lcd_puts (uart_string);
    lcd_puts("  ");
    uart_puts(uart_string);
    uart_puts("  ");

    if (value == 0)
    {
        lcd_clrscr();
        lcd_gotoxy(0,1);
        
        uart_puts ("\033[4;32m");
        uart_puts("RIGHT\r\n");
        uart_puts("\033[0m");
        lcd_puts("RIGHT");
        
        
    }
    else if(value == 99)   
    {
        lcd_clrscr();
        lcd_gotoxy(0,1);
        uart_puts ("\033[3;33m");
        uart_puts("UP\r\n" );
        uart_puts("\033[0m");
        lcd_puts("UP");
    }
    else if(value == 256)   
    {
        lcd_clrscr();
        lcd_gotoxy(0,1);
        uart_puts ("\033[9;34m");
        uart_puts("DOWN\r\n" );
        uart_puts("\033[0m");
        lcd_puts("DOWN");
    }
    else if(value == 409)   
    {
        lcd_clrscr();
        lcd_gotoxy(0,1);
        uart_puts ("\033[5;94m");
        uart_puts("LEFT\r\n" );
        uart_puts("\033[0m");
        lcd_puts("LEFT");
    }
   else if(value == 639)   
    {
        lcd_clrscr();
        lcd_gotoxy(0,1);
        uart_puts ("\033[3;91m");
        uart_puts("SELECT\r\n" );
        uart_puts("\033[0m");
        lcd_puts("SELECT");
    }
    else
     
    {
        lcd_clrscr();
        lcd_gotoxy(0,1);
        uart_puts("value\r\n" );
         lcd_puts("FATAL ERROR");
    }
}