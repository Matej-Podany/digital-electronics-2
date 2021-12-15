/*
 * platform.c
 *
 * Created: 08.12.2021 15:41:01
 *  Author: Honza
 */ 

#include "platform.h"

/* Variables ---------------------------------------------------------*/
// Custom character definition using https://omerk.github.io/lcdchargen/

char lcd_string[16] = "0";
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
#define echo_pin PD3
#define trig_pin PD2
#define relay_pin PB4


static uint8_t pump_out_limit = 50; //cm
static uint8_t pump_running = 0;
static volatile uint16_t pulse_length = 10000;
static uint16_t length = 200;
static uint8_t raining = 0;

void setup_platform()
{
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
	
	// configure pins
	GPIO_config_output(&DDRD, trig_pin);
	GPIO_write_low(&PORTD, trig_pin);
	GPIO_config_input_nopull(&DDRD, echo_pin);
	GPIO_config_output(&DDRB, relay_pin);
	GPIO_write_high(&PORTB, relay_pin);
	
	// Interrupt for display
	TIM0_overflow_262ms();
	TIM0_overflow_interrupt_enable();
	
	// enable external interrupt to any logical change (datasheet page 80)
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC00);
	EICRA &= ~(1<<ISC01);

	// Enables interrupts by setting the global interrupt mask
	sei();
}
void is_raining(uint16_t length)
{
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
}

void start_measure()
{
	TCNT1 = 0;
	TIM1_overflow_33ms();
}

void stop_measure()
{
	TIM1_stop();
}

void update_values()
{
	pulse_length = TCNT1;
	length = pulse_length * 0.008325; // number of edges * prescaler (8) / FCPU * speed of sound (333ms-1) / 2 (travels the distance twice)
}

void trigger_distance_sensor()
{
	GPIO_write_high(&PORTD, trig_pin);
	_delay_us(10);
	GPIO_write_low(&PORTD, trig_pin);
}

void update_LCD()
{
	if (raining)
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
}

void update_LCD_error()
{
	lcd_gotoxy(5,1);
	lcd_puts("Meas err");
}

void pump_state_control()
{
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
