# Lab 4: MATĚJ PODANÝ

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/Matej-Podany/digital-electronics-2](https://github.com/Matej-Podany/digital-electronics-2)


### Overflow times

1. Complete table with overflow times.

| **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| Timer/Counter0 | 8  | 16u | 128u | -- | 1024u | -- | 4096u | 16384u |
| Timer/Counter1 | 16 | 4,096m | 32,768m | -- | 262144m | -- | 1048,576m | 4194,304m |
| Timer/Counter2 | 8  | 16u | 128u | 512u | 1024u | 2048u | 4096u | 16384u |


### Timer library

1. In your words, describe the difference between common C function and interrupt service routine.
   * Function je samostatně definovaný určitý počet instrukcí, příkazů či výpočtů, který může a nemusí mít vstupní i výstupní hodnoty. Je volána svým názvem společně se zadáním případních vstupů standardně ve funkci main ve while cyklu (avšak lze ji volat odkudkoli).
   * Interrupt service routine také obsahuje určitý počet instrukcí, příkazů či výpočtů, avšak všechny tyto kroky jsou provedeny automaticky po splnění podmínky pro přerušení chodu mikroprocesoru. Podmínka přerušení může být třeba přetečení časovače, tudíž se tato operace periodicky opakuje při každém přetečení.

2. Part of the header file listing with syntax highlighting, which defines settings for Timer/Counter0:

```c
/**
 * @name  Definitions for 8-bit Timer/Counter0
 * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 8, F_CPU = 16 MHz
 */
/** @brief Stop timer, prescaler 000 --> STOP */
#define TIM0_stop()           TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
/** @brief Set overflow 4ms, prescaler 001 --> 1 */
#define TIM0_overflow_4ms()   TCCR0B &= ~((1<<CS02) | (1<<CS01)); TCCR0B |= (1<<CS00);
/** @brief Set overflow 33ms, prescaler 010 --> 8 */
#define TIM0_overflow_33ms()  TCCR0B &= ~((1<<CS02) | (1<<CS00)); TCCR0B |= (1<<CS01);
/** @brief Set overflow 262ms, prescaler 011 --> 64 */
#define TIM0_overflow_262ms() TCCR0B &= ~(1<<CS02); TCCR0B |= (1<<CS01) | (1<<CS00);
/** @brief Set overflow 1s, prescaler 100 --> 256 */
#define TIM0_overflow_1s()    TCCR0B &= ~((1<<CS01) | (1<<CS00)); TCCR0B |= (1<<CS02);
/** @brief Set overflow 4s, prescaler // 101 --> 1024 */
#define TIM0_overflow_4s()    TCCR0B &= ~(1<<CS01); TCCR0B |= (1<<CS02) | (1<<CS00);
/** @brief Enable overflow interrupt, 1 --> enable */
#define TIM0_overflow_interrupt_enable()  TIMSK0 |= (1<<TOIE0);
/** @brief Disable overflow interrupt, 0 --> disable */
#define TIM0_overflow_interrupt_disable() TIMSK0 &= ~(1<<TOIE0);

/**
```

3. Flowchart figure for function `main()` and interrupt service routine `ISR(TIMER1_OVF_vect)` of application that ensures the flashing of one LED in the timer interruption. When the button is pressed, the blinking is faster, when the button is released, it is slower. Use only a timer overflow and not a delay library.

   ![Figure](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/04-interrupts/images/figure.jpg "Figure")


### Knight Rider

1. Scheme of Knight Rider application with four LEDs and a push button, connected according to Multi-function shield. Connect AVR device, LEDs, resistors, push button, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![Schematic](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/04-interrupts/images/schematic.jpg "Schematic")