

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
    sei();
    while(1);
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
    if(is_measuring)
    {
        // Reset and activate measuring timer
        start_measure();
    } else {
        // Stop measuring timer
        stop_measure();
        // Calculate new values from measurement
        // And possibly evaluate 
        update_values();
        update_LCD();
    }
}

/**
 * @brief 1s overflow with 1/60 divider. 
 * Trigger measurement once per 1 minute. 
 * 
 */
ISR(TIMER0_OVF_vect)
{
    // One second passed (this is not very precise,
    // it takes 1048ms to overflow TIM0)
    static uint8_t seconds = 0;
    seconds ++;
    if(seconds >= 60)
    {
        // Trigger distance measurement once per minute
        trigger_distance_sensor();
    }
}

/**
 * @brief This should never happen. Sensor response took too long.
 * 
 * 
 */
ISR(TIMER1_OVF_vect)
{
    // stop timer and write error message to LCD
    stop_measure();
    update_LCD_error();

}