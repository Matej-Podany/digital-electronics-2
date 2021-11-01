# Lab 6: MATĚJ PODANÝ

Link to your `Digital-electronics-2` GitHub repository:

	[https://github.com/Matej-Podany/digital-electronics-2](https://github.com/Matej-Podany/digital-electronics-2)


### LCD display module

1. In your words, describe what ASCII table is.
   * ASCII je tabulka znaků a písmen, které jsou uloženy na konkrétních pozicích paměti HD44780. Pokud některý ze znaků chceme vyvolat u jakéhokoliv displeje různých velikostí či výrobců a platí, že je ovládán modulem HD44780, tak vždy bude adresa jednoho symbolu stejná. Také máme možnost uložit 8 vlastně nadefinovaných znaků, které se však ukládají v paměti RAM a je potřeba je opět zapsat při každém zapnutí něbo je lze přeposovat během zapnutí displeje.

2. (Hand-drawn) picture of time signals between ATmega328P and LCD keypad shield (HD44780 driver) when transmitting three character data `De2`.

   ![Figure](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/06-lcd/images/figure.jpg "Figure")


### Stopwatch

1. Flowchart figure for `TIMER2_OVF_vect` interrupt service routine which overflows every 16&nbsp;ms but it updates the stopwatch LCD approximately every 100&nbsp;ms (6 x 16&nbsp;ms = 100&nbsp;ms). Display tenths of a second and seconds `00:seconds.tenths`. Let the stopwatch counts from `00:00.0` to `00:59.9` and then starts again. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![Flowchart](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/06-lcd/images/flowchart.png "Flowchart")


### Custom characters

1. Code listing with syntax highlighting of two custom character definition:

```c
/* Variables ---------------------------------------------------------*/
// Custom character definition - two symbols empty and full battery
uint8_t customChar[16] = {
    0b01110,
	0b01110,
	0b11111,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b01110,
	0b01110,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111

};
```


### Kitchen alarm

Consider a kitchen alarm with an LCD, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![Schematic](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/06-lcd/images/schematic.jpg "Schematic")