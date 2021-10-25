/***********************************************************************
 * 
 * Decimal counter with 7-segment output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

volatile uint8_t cnt0 = 0;
volatile uint8_t cnt1 = 0;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Display decimal counter values on SSD (Seven-segment 
 *           display) when 16-bit Timer/Counter1 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Test of SSD: display number '3' at position 0
    SEG_update_shift_regs(3, 0);

    // Configure 16-bit Timer/Counter1 for Decimal counter
    // Set the overflow prescaler to 262 ms and enable interrupt
   TIM1_overflow_262ms();
   TIM1_overflow_interrupt_enable();
   
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
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 59.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    //counting values for two SSD from 00 to 59 almost every second (1048 ms, 4.8 % deviation)
	static uint8_t noc = 0; // This line will only run the first time
	noc++;
	if (noc >  3) // every second will count up
	{
		noc=0;
		cnt0++;
		if (cnt0 > 9)
		{
			cnt0=0;
			cnt1++;
		}
		if (cnt1 > 5)
		{
			cnt1=0;
		}
	}
}
/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Display tens and units of a counter at SSD.
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{
    // timer for 2 displays 2*8ms <= 16 ms for human eye
	static uint8_t pos = 0;  // This line will only run the first time
	pos++;
	if (pos > 1) // every 8 ms will update one of the displays
	{
		if (pos == 2)
		{
			SEG_update_shift_regs(cnt0, 0);
		}
		if (pos == 4)
		{
			pos=0;
			SEG_update_shift_regs(cnt1, 1);
		}
	}
}