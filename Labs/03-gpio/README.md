# Lab 3: MATĚJ PODANÝ

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/Matej-Podany/digital-electronics-2](https://github.com/Matej-Podany/digital-electronics-2)

### Data types in C

1. Complete table.

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0 to 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -128 to 127 | Signed 8-bit integer |
| `uint16_t` | 16 | 0 to 65535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | -32768 to 32767 | Signed 16-bit integer |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | 0 or 8 | none | Empty variable or void pointer |


### GPIO library

1. In your words, describe the difference between the declaration and the definition of the function in C.
   * Function declaration means, that we define what type will be this function (void, uint8_t and so on), so if function has output or not. We also define how many or none inputs there will be and also their data types.
   * Function definition means, that here we define, what our function accually does.

2. Part of the C code listing with syntax highlighting, which toggles LEDs only if push button is pressed. Otherwise, the value of the LEDs does not change. Use function from your GPIO library. Let the push button is connected to port D:

```c
    // Configure the second LED at port C
    GPIO_config_output(&DDRC, OUT_LED); //setting port C as output // this operations will be always one
	
    GPIO_write_low(&PORTC, OUT_LED);; //turning LED on // this operations will be always zero

    // Configure Push button at port D and enable internal pull-up resistor   
 
    GPIO_config_input_pullup(&DDRD, BUTTON); //setting port D as input // this operation is always zero
	                                         //internal pull-up resistor is enabled // this operation is always one
	
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);
        if(GPIO_read(&PIND, BUTTON) == 0)
		{
			GPIO_toggle(&DDRB, LED_GREEN);
			GPIO_toggle(&DDRC, OUT_LED);
		}
		
    }
```


### Traffic light

1. Scheme of traffic light application with one red/yellow/green light for cars and one red/green light for pedestrians. Connect AVR device, LEDs, resistors, one push button (for pedestrians), and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()