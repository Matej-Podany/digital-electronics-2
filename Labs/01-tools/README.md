# Lab 1: Matěj Podaný

Link to your `Digital-electronics-2` GitHub repository:

   [Matěj Podaný - GitHub](https://github.com/Matej-Podany/digital-electronics-2)


### Blink example

1. What is the meaning of the following binary operators in C?
   * `|` OR
   * `&` AND
   * `^` XOR
   * `~` NEGATION
   * `<<` SHIFT LEFT
   * `>>` SHIFT RIGHT

2. Complete truth table with operators: `|`, `&`, `^`, `~`

| **b** | **a** |**b or a** | **b and a** | **b xor a** | **not b** |
| :-: | :-: | :-: | :-: | :-: | :-: |
| 0 | 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 1 | 0 | 1 | 1 |
| 1 | 0 | 1 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 | 0 |


### Morse code

1. Listing of C code with syntax highlighting which repeats one "dot" and one "comma" on a LED:

```c
int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Infinite loop
    while (1)
    {
        // Invert LED in Data Register   Rozsvítí se dioda
        // PORTB = PORTB xor 0010 0000
        PORTB = PORTB | (1<<LED_GREEN);
		
		// Pause several milliseconds    Svítí dioda po 200 ms
        _delay_ms(SHORT_DELAY);

        // Invert LED in Data Register  Zhasíná se dioda
        // PORTB = PORTB xor 0010 0000
        PORTB = PORTB & ~(1<<LED_GREEN);
		
		
		// Pause several milliseconds   Je zhasnutá po 1 s
		_delay_ms(SHORT_DELAY2);
		
		// Invert LED in Data Register  Rozsvítí se dioda
		// PORTB = PORTB xor 0010 0000
		PORTB = PORTB | (1<<LED_GREEN);
		
		// Pause several milliseconds  Svítí po 1 s
		_delay_ms(SHORT_DELAY2);

		// Invert LED in Data Register  Zhasíná se dioda
		// PORTB = PORTB xor 0010 0000
		PORTB = PORTB & ~(1<<LED_GREEN);
		
		
		// Pause several milliseconds  Je zhasnutá po 200 ms
		_delay_ms(SHORT_DELAY2);
		
    }

    // Will never reach this
    return 0;
}
```


2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

	![Figure](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/01-tools/images/figure.jpg "figure")
