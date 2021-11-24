/***********************************************************************
 * Water tank project
 * LCD display controller
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2021-Present Matej Podany
 * Brno University of Technology, Czechia
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
uint8_t customChar[40] = {
	0b10001,  //full tank
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b10001,  // almost full tank
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b10001,  // almost empty tank
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b10001,  // empty tank
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b01110,  // rainning symbol
	0b11111,
	0b01110,
	0b00000,
	0b01010,
	0b10100,
	0b00010,
	0b00100
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

    // Put string at LCD display
    lcd_gotoxy(0, 1);
    lcd_puts("LVL:");

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 40; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
    // Configure 8-bit Timer/Counter0 for lcd updates
    // Set the overflow prescaler to 262 ms and enable interrupt
	TIM0_overflow_262ms();
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
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Update LCD display every overflow.
 *           
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{
    lcd_gotoxy(5,1);
	lcd_puts("2cm");
    //itoa(tens, lcd_string_tens, 10);
	lcd_gotoxy(15,1);
    lcd_putc(3);
	lcd_gotoxy(15,0);
	lcd_putc(4);
}