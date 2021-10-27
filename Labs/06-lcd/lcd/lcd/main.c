/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Variables ---------------------------------------------------------*/
// Custom character definition using https://omerk.github.io/lcdchargen/
uint8_t customChar[32] = {
	0b01110,
	0b01110,
	0b11111,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b01110,
	0b01110,
	0b11111,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b01110,
	0b01110,
	0b11111,
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b01110,
	0b11111,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD display when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0   a");
	lcd_gotoxy(1, 1);
	lcd_puts("b");
	lcd_gotoxy(11, 1);
	lcd_puts("c_");

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 32; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Display first custom character
	
    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set the overflow prescaler to 16 ms and enable interrupt
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD display every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
	static uint8_t number_of_overflows2 = 0;
    static uint8_t tens = 0;        // Tenths of a second
    static uint8_t secs = 0;        // Seconds
	static uint8_t tenssecs = 0;	// tens of seconds
    char lcd_string_tens[1] = " ";      // String for converting numbers by itoa()
	char lcd_string_secs[1] = " ";
	char lcd_string_tenssecs[1] = " ";

    number_of_overflows++;
	number_of_overflows2++;
    if (number_of_overflows >= 24)
    {
	    // Do this every 6 x 16 ms = 100 ms (24*4 ms = 100 ms)
	    number_of_overflows = 0;
		tens++;
		if(tens >=10)
		{
			tens = 0;
			secs++;
			if (secs >= 10)
			{
				secs = 0;
				tenssecs++;
				if (tenssecs >= 6)
				{
					tenssecs = 0;
				}
			}
		}
		if (number_of_overflows2 >= 4)
		{
			number_of_overflows2 = 0;   // not working, showing static symbols of an adress
            lcd_gotoxy(7,0);
			itoa(tens, lcd_string_tens, 10);
		    lcd_putc(lcd_string_tens);
            lcd_gotoxy(5,0);
			itoa(secs, lcd_string_secs, 10);
			lcd_putc(lcd_string_secs);
			lcd_gotoxy(4,0);
			itoa(tenssecs, lcd_string_tenssecs, 10);
			lcd_putc(lcd_string_tenssecs);
		}
    }
    // Else do nothing and exit the ISR
}