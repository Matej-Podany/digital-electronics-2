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
#include <util/delay.h>
#include "gpio.h"
#include "lcd_definitions.h"
#include "uart.h"
#define ULTRA_ECHO PD3
#define ULTRA_TRIGGER PD2 
#define F_CPU 16000000

static uint8_t counting = 0;
static uint16_t pulse_length = 0;
char uart_string[16] = "                ";

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
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();
	
	//TIM2_overflow_262ms();
	//TIM2_overflow_interrupt_enable();
	
	// enable external interrupt to any logical change (datasheet page 80)
	//EIMSK |= (1<<INT0);
	//EICRA |= (1<<ISC00);
	//EICRA &= ~(1<<ISC01);

    // Enables interrupts by setting the global interrupt mask
    sei();
	
	//starting uart communication
	uart_init(UART_BAUD_SELECT(9600, 16000000UL));
	
	// 
	GPIO_config_output(&DDRD, ULTRA_TRIGGER);
	GPIO_write_low(&PORTD, ULTRA_TRIGGER);
	
	GPIO_config_input_nopull(&DDRD, ULTRA_ECHO);
	GPIO_config_output(&DDRB, PB5);

    // Infinite loop
    while (1)
    {
        uart_puts("while is working/r/n");
		GPIO_write_high(&DDRD, ULTRA_TRIGGER);
        _delay_us(10);
        GPIO_write_low(&DDRD, ULTRA_TRIGGER);
		
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
    uart_puts("TIM0 disp ovrf/r/n");
	lcd_gotoxy(5,1);
	lcd_puts("2cm");
    //itoa(tens, lcd_string_tens, 10);
	lcd_gotoxy(15,1);
    lcd_putc(3);
	lcd_gotoxy(15,0);
	lcd_putc(4);
	
}

ISR(INT0_vect)
{
	uart_puts("inside INT0/r/n");
	if (counting == 0)
	{
		counting = 1;
		uart_puts("start/r/n");
		TIM1_overflow_4ms();
	}
	else if (counting == 1)
	{
		TIM1_stop();
		pulse_length = TCNT1;
		counting = 0;
		itoa(pulse_length, uart_string, 10);
		uart_puts(uart_string);
		uart_puts("/r/n");
		TCNT1 = 0;
	}
	
	
}