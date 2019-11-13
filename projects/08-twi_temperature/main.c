/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2017-10-27
 * Last update: 2019-11-08
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Scan TWI bus for all connected slave devices and transmit info
 *    to UART.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "uart.h"
#include "twi.h"

/* Typedef -----------------------------------------------------------*/
typedef enum {
    IDLE_STATE = 1,
    TRY_STATE,
    ACK_STATE
} state_t;

/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
state_t current_state = IDLE_STATE;

/* Function prototypes -----------------------------------------------*/
void fsm_twi_scanner(void);

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Test all TWI slave addresses and scan 
 *          connected devices.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // TWI
    twi_init();

    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);              //rychlost čítání
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // Enables interrupts by setting the global interrupt mask
   

    // Put strings to ringbuffer for transmitting via UART.
    sei();
    //uart_puts("\r\n---TWI scanner---");
    //uart_puts("\r\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
    
    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Update state of TWI Finite
 *         State Machine.
 */
ISR(TIMER1_OVF_vect)
{
    uint8_t temp;
    uint8_t temp1;
    twi_start((0x5c<<1)+TWI_WRITE);
    twi_write(0x02);
    twi_stop();
    twi_start((0x5c<<1)+TWI_READ);
    temp = twi_read_ack();
    temp1 = twi_read_nack();
    twi_stop();

    char temp_string[4];
    char temp1_string[4];
    itoa(temp,temp_string,10);
    itoa(temp1,temp1_string,10);
    uart_puts("Teplota je: ");
    uart_puts(temp_string);
    uart_puts(",");
    uart_puts(temp1_string);
    uart_puts("°C");
    uart_puts("\r\n");

}




