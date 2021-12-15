
/**
 * @file main.c
 * @author Ondrej Dudasek (221100@vutbr.cz)
 * @brief Main file of project
 * @version 1.0
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <inttypes.h>
#include <avr/interrupt.h>
#include "library/platform.h"

/**
 * @brief Main function. While can be later used to implement
 * power saving mode.
 */ 
int main(void)
{
    setup_platform();

    LOG("Init done");
    LOG("Enabling interrupts");
    sei();
            
    LOG("Interrupts enabled, going into loop");
    while (1);
    
}


/**
 * @brief INT0 is connected to HC-SR04 output. 
 * This interrupt routine starts or stops measurement
 * of impulse length.
 * 
 */
ISR(INT0_vect)
{
    static uint8_t is_measuring = 0;
    if(!is_measuring)
    {
        //LOG("Sensor interrupt - begin measure");
        // Reset and activate measuring timer
        start_measure();
        is_measuring = 1;
        
    } else {
        // Stop measuring timer
        stop_measure();
        // Calculate new values from measurement
        // And possibly evaluate 
        update_values();
        update_LCD();
        //LOG("Sensor interrupt - end measure");
        is_measuring = 0;
    }
}

/**
 * @brief 1s overflow with 1/60 divider. 
 * Trigger measurement once per 1 minute. 
 * 
 */
ISR(TIMER0_OVF_vect)
{
    static uint8_t prescale = 249;
    static uint8_t seconds = 59;
    prescale ++;
    if(prescale >= 250) //prescale 4ms interrupt
    {
        prescale = 0;
        seconds ++;
        GPIO_toggle(&DDRB, PB5);    // 1s blink onboard led
        if(seconds >= 2)
        {
            // Trigger distance measurement once per minute
            trigger_distance_sensor();
            LOG("Distance sensor triggered");
            seconds = 0;
        } else {
            //LOG(".");
        }
    }
    
}

/**
 * @brief This should never happen. Sensor response took too long.
 */
ISR(TIMER1_OVF_vect)
{
    
    // stop timer and write error message to LCD
    stop_measure();
    update_LCD_error();
    LOG("Sensor timer overflow");

}