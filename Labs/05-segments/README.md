# Lab 5: YOUR_FIRSTNAME FAMILYNAME

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/Matej-Podany/digital-electronics-2](https://github.com/Matej-Podany/digital-electronics-2)


### 7-segment library

1. In your words, describe the difference between Common Cathode and Common Anode 7-segment display.
   * CC SSD je sedmisegmentový displej, který má společnou katodu. To znamemná, že má spojeny všechny katody diod, kde je nízká úroveň (zem). Ovládá se tedy vysokou úrovní napětí na vstupech.
   * CA SSD je sedmisegmentový displej, který má společnou anodu. To znamemná, že má spojeny všechny anody diod, kde je vysoká úroveň (napájecí napětí). Ovládá se tedy nízkou úrovní na výstupech.

2. Code listing with syntax highlighting of two interrupt service routines (`TIMER0_OVF_vect`, `TIMER0_OVF_vect`) from counter application with at least two digits, ie. values from 00 to 59:

```c
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 59.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    //counting values for two SSD from 00 to 59 almost every second (1048 ms, 4.8 % deviation)
	static uint8_t noc = 0; // This line will only run the first time
	noc++;
	if (noc >  3) // every second will count up
	{
		noc=0;
		cnt0++;
		if (cnt0 > 9)
		{
			cnt0=0;
			cnt1++;
		}
		if (cnt1 > 5)
		{
			cnt1=0;
		}
	}
}
```

```c
/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Display tens and units of a counter at SSD.
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{
    // timer for 2 displays 2*8ms <= 16 ms for human eye
	static uint8_t pos = 0;  // This line will only run the first time
	pos++;
	if (pos > 1) // every 8 ms will update one of the displays
	{
		if (pos == 2)
		{
			SEG_update_shift_regs(cnt0, 0);
		}
		if (pos == 4)
		{
			pos=0;
			SEG_update_shift_regs(cnt1, 1);
		}
	}

}
```

3. Flowchart figure for function `SEG_clk_2us()` which generates one clock period on `SEG_CLK` pin with a duration of 2&nbsp;us. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![Figure](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/05-segments/images/figure.jpg "Figure")


### Kitchen alarm

Consider a kitchen alarm with a 7-segment display, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![Schematic](https://github.com/Matej-Podany/digital-electronics-2/blob/main/Labs/05-segments/images/schematic.jpg "Schematic")