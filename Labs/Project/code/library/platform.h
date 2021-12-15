/**
 * @file platform.h
 * @author Jan Barto� (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

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

#ifndef LIBRARY_PLATFORM
#define LIBRARY_PLATFORM


/**
 * @brief Update values on LCD
 * 
 * @param height height of water level to display in cm
 * @param is_raining non-zero value indicates that it's raining. 
 */
void update_LCD();

/**
 * @brief Print error message caused by failure in measurement causing
 * timer overflow
 */
void update_LCD_error();

/**
 * @brief Setups the platform
 * timer overflow
 */
void setup_platform(void);

/**
 * @brief Starts the measurement
 * timer overflow
 */
void start_measure(void);

/**
 * @brief Stops the measurement
 * timer overflow
 */
void stop_measure(void);

/**
 * @brief Send 10us high signal on trigger pin
 * 
 */
void trigger_distance_sensor();


/**
 * @brief Reads output from the sensor, calculates data
 * timer overflow
 */
void update_values();

/**
 * @brief Controls the state of the pump
 * timer overflow
 */
void pump_state_control();
/**
 * @brief Checks whether or not it's raining
 * 
 * @param length distance from the top of the barrel to the bottom in cm
 */
void is_raining(uint16_t length);


#endif // LIBRARY_PLATFORM