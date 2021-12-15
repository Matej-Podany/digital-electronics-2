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

#ifndef F_CPU
#define F_CPU 16000000
#endif

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

#define echo PD3
#define trig PD2
#define relay_pin PB4


static uint8_t pump_out_limit = 50; //cm
static uint8_t pump_running = 0;
static volatile uint8_t counting = 0;
static volatile uint16_t pulse_length = 10000;
static uint16_t length = 200;
char lcd_string[16] = "0";

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

int is_raining(uint16_t length)
{
	static uint8_t raining = 0;
	static uint8_t previous_length = 0;
	if (length < previous_length)
	{
		raining = 1;
	}
	else
	{
		raining = 0;
	}

	previous_length = length;
	return raining;
}

int main(void) {
	
	GPIO_config_output(&DDRD, trig);
	GPIO_write_low(&PORTD, trig);
	GPIO_config_input_nopull(&DDRD, echo);
	GPIO_config_output(&DDRB, relay_pin);
	GPIO_write_high(&PORTB, relay_pin);
	
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 1);
    lcd_puts("LVL:");

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 40; i++) {
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	//// for display
	//TIM0_overflow_262ms();
	//TIM0_overflow_interrupt_enable();
	
	/*TIM2_overflow_4ms();
	TIM2_overflow_interrupt_enable();*/
	
	//starting uart communication
	//uart_init(UART_BAUD_SELECT(9600, 16000000UL));
	
	// enable external interrupt to any logical change (datasheet page 80)
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC00);
	EICRA &= ~(1<<ISC01);

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1) {
        //uart_puts("while is working\r\n");
		GPIO_write_high(&DDRD, trig);
        _delay_us(10);
        GPIO_write_low(&DDRD, trig);
		length = pulse_length * 0.008325; // number of edges * prescaler (8) / FCPU * speed of sound (333ms-1) / 2 (there and back again... as Bilbo)
		if (is_raining(length))
		{
			lcd_gotoxy(15,0);
			lcd_putc(4);
		}
		else
		{
			lcd_gotoxy(15,0);
			lcd_puts(" ");
		}
		
		
		itoa((200 - length), lcd_string, 10);
		//uart_puts(lcd_string);
		//uart_puts("\r\n");
		//uart_puts("TIM0 disp ovrf/r/n");
		lcd_gotoxy(5,1);
		lcd_puts("       ");
		lcd_gotoxy(5,1);
		lcd_puts(lcd_string);
		lcd_puts("cm");
		if (length <= 25)
		{
			lcd_gotoxy(15,1);
			lcd_putc(0);
		}
		else if (length <= 100)
		{
			lcd_gotoxy(15,1);
			lcd_putc(1);
		}
		else if (length <= 175)
		{
			lcd_gotoxy(15,1);
			lcd_putc(2);
		}
		else
		{
			lcd_gotoxy(15,1);
			lcd_putc(3);
		}
		_delay_ms(1000);
		if ((length <= 10) && (!pump_running))
		{
			GPIO_write_low(&PORTB, relay_pin);
			pump_running = 1;
		}
		else if ((length >= pump_out_limit) && (pump_running))
		{
			GPIO_write_high(&PORTB, relay_pin);
			pump_running = 0;
		}
    }
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Measure water level.
 **********************************************************************/

ISR(INT0_vect) {
	if (counting == 0) {
		counting = 1;
		TIM1_overflow_33ms();
	}
	else if (counting == 1) {
		TIM1_stop();
		pulse_length = TCNT1;
		TCNT1 = 0;
		counting = 0;
	}	
}
