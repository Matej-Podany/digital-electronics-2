# Lab 2: MATĚJ PODANÝ

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/Matej-Podany/digital-electronics-2](https://github.com/Matej-Podany/digital-electronics-2)


### Active-low and active-high LEDs

1. Complete tables according to the AVR manual.

| **DDRB** | **Description** |
| :-: | :-- |
| 0 | Input pin |
| 1 | Output pin |

| **PORTB** | **Description** |
| :-: | :-- |
| 0 | Output low value |
| 1 | Output high value |

| **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
| :-: | :-: | :-: | :-: | :-- |
| 0 | 0 | input | no | Tri-state, high-impedance |
| 0 | 1 | input | yes | Activated pull-up resistor |
| 1 | 0 | output | no | Output high value |
| 1 | 1 | output | no | Output low value |

2. Part of the C code listing with syntax highlighting, which blinks alternately with a pair of LEDs; let one LED is connected to port B and the other to port C:

```c
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN); // this operations will be always one
    // ...and turn LED off in Data Register - active high, set low
    PORTB = PORTB & ~(1<<LED_GREEN); // this operations will be always zero

    // Configure the second LED at port C
	DDRC = DDRC | (1<<OUT_LED); //vystup // this operations will be always one
	
	PORTC = PORTC | (1<<OUT_LED); //nastavit led nesviti - active low - set high // this operations will be always one


    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);
		// OUT_LED is active except the first time, when the code starts

		// Switch off OUT_LED
		PORTC = PORTC | (1<<OUT_LED); // active low, set high // this operations will be always one
        // Switch on LED_GREEN
		PORTB = PORTB | (1<<LED_GREEN); // active high, set high // this operations will be always one
		
		// LED_GREEN is active 
		// Pause several milliseconds
        _delay_ms(BLINK_DELAY);
		// LED_GREEN is active
		
		// Switch off LED_GREEN
		PORTB = PORTB & ~(1<<LED_GREEN); // active high, set low // this operations will be always zero
		// Switch on OUT_LED
		PORTC = PORTC & ~(1<<OUT_LED); // active low, set low // this operations will be always zero
		
		// OUT_LED is active
    }

    // Will never reach this
    return 0;
}
```


### Push button

1. Part of the C code listing with syntax highlighting, which toggles LEDs only if push button is pressed. Otherwise, the value of the LEDs does not change. Let the push button is connected to port D:

```c
	// Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN); // this operations will be always one
    // ...and turn LED off in Data Register - active high, set low
    PORTB = PORTB & ~(1<<LED_GREEN); // this operations will be always zero

    // Configure the second LED at port C
    DDRC = DDRC | (1<<OUT_LED); //setting port C as output // this operations will be always one
    
	PORTC = PORTC & ~(1<<OUT_LED); //turning LED on // this operations will be always zero
	
    // Configure Push button at port D and enable internal pull-up resistor
	
	DDRD = DDRD & ~(1<<BUTTON);   //setting port D as input // this operation is always zero
	PORTD = PORTD | (1<<BUTTON); //internal pull-up resistor is enabled // this operation is always one
	
    // Infinite loop
    while (1)
    {
		// Invert LED in Data Register
		// PORTB = PORTB xor 0010 0000
		if(bit_is_clear(PIND, BUTTON)) //if button is pushed, this if condition is not skipped
		{	
			PORTB = PORTB ^ (1<<LED_GREEN); // this operation will always inverse value of PORTB
			PORTC = PORTC ^ (1<<OUT_LED);  // this operation will always inverse value of PORTC
			
		}
        
    }
```


### Knight Rider

1. Scheme of Knight Rider application, i.e. connection of AVR device, five LEDs, resistors, one push button, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![Schematic](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/02-leds/images/schematic.jpg "Schematic")