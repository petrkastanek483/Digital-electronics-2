
# Projekt Couvací senzor

#### Obsah

1. [Úvod](#úvod)
2. [Hardware](#hardware)
3. [Software](#software)
4. [Reference](#reference)


## Úvod

Tento projekt je zaměřen na měření vzdálenosti pomocí snímače HC-SR04. Pomocí tohoto měření je poté implementována funkce couvacích senzorů a vzdálenost je poté zobrazena na 7-mi segmentovém displej a také pomocí 4 LED diod a frekvence bzučáku.


## Hardware

senzor HC-SR04
multi function shield
AVR ATmega


## Software

Zde lze vidět část kódu, díky které je vysílán trigger pulz z vysílače senzoru HC-SR04:

/* Trigger pulse */
uint8_t ReadStatus = 0;
void sendTrigger(uint8_t pin) {
    GPIO_config_output(&DDRD, trig);
    _delay_us(2);   // 2 us delay
    GPIO_write(&PORTD, pin, 1);
    _delay_us(10);  // 10 us delay
    GPIO_write(&PORTD, pin, 0); }
    
Poté probíhá zpracování tohoto signálu pomocí části kódu níže: 

/* Distance measurement */
    uint8_t ReadStatus = 0;

    sendTrigger(trig);
    while (ReadStatus == 0) {
        while (GPIO_read(&PIND, echo)) {
            vzd++;
            ReadStatus = 1;            } 
                            }


## Reference

1. Microchip, [AVR 8-bit Toolchain for Windows, Linux, Mac OS X](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers)
