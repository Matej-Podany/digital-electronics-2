/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define BLINK_DELAY 500
#define OUT_LED     PC0     // external led
#define BUTTON		PD0
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>
/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Configure the second LED at port C
	DDRC = DDRC | (1<<OUT_LED); //vystup
	
	PORTC = PORTC & ~(1<<OUT_LED); //nastavit led nesviti

    // Configure Push button at port D and enable internal pull-up resistor
	
	DDRD = DDRD & ~(1<<BUTTON);   //dobre zkontrolovano
	PORTD = PORTD | (1<<BUTTON); //dobre zkontrolovano
    // Infinite loop
    while (1)
    {
		
		PORTB = PORTB | (1<<LED_GREEN);
		//doplnit ZDE jedna led sviti a druha ne
		

		// Invert LED in Data Register
		// PORTB = PORTB xor 0010 0000
		if(bit_is_clear(PIND, BUTTON)) //toto by melo byt plne funkcni podle ucitele
		{	
			PORTB = PORTB ^ (1<<LED_GREEN); //dobre zkontrolovano
			PORTC = PORTC ^ (1<<OUT_LED);  //dobre zkontrolovano
			
		}
        
    }

    // Will never reach this
    return 0;
}
