//LED.c

#include "timer.h"

void init_LEDS();

volatile uint8_t LED_ENABLED = 0; //volatile so compiler doesn't optimise out the ISR changes

void init_LEDS()
{
	//Set PortB pins associated with LEDs to output
	DDRB |= (1<<PB4 | 1<<PB3 | 1<<PB2);//DDRB = DDRB |[00011100]
	
	//Output 1 on PortB LED pins to turn off all LEDs
	PORTB |= (1<<PB4 | 1<<PB3 | 1<<PB2);
}

void LEDS_DISABLE()
{
	LED_ENABLED = 0;
}

void LEDS_ENABLE()
{
	LED_ENABLED = 1;
}

void LED_OFF()
{
	//Output 1 on PortB LED pins to turn off all LEDs
	PORTB |= (1<<PB4 | 1<<PB3 | 1<<PB2);
}

void LED_ON(char colour)
{
	// //turn off LEDs & set a 1 second timer
	// LED_OFF(); //Don't use LEDS_DISABLE, otherwise it messes up main() & Emergency ISR()
	
	// SET_TIMER(0, 1); //set one second timer to delay before changing LEDs
	// uint8_t timerClear = TIMER_IS_CLEAR();
	// while(timerClear != 1){
		// timerClear = TIMER_IS_CLEAR();
	// }

	if(LED_ENABLED){
		//turn on selected LED
		switch(colour)
		{
			case 'G':
				PORTB &= ~(1<<PB2);
				break;
			case 'Y':
				PORTB &= ~(1<<PB3);
				break;
			case 'R':
				PORTB &= ~(1<<PB4);
				break;
		}
	}
}