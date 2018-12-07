// timer_delay.c
// for NerdKits with ATmega168

#define F_CPU 14745600

#include <stdio.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"
#include "../libnerdkits/uart.h"

void init_timer();
void timer_delay(uin8_t min, uint8_t sec);

void init_timer();
{
//Set correct bits in timer control register, flags, interrupts, etc.
//Set the timer up so that the OCF1A flag interrupts every second
  PRR &= ~(1<<PRTIM1); // Ensure PRTIM1 bit is cleared in the Power Reduction Register
  TCCR1A ^= TCCR1A;//Clear TCCR1A register 
  
  //WGM12 - Set Timer/Counter Control Register to CTC mode
  TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);//CS12/CS10 - Set clock source to prescale clk/1024
  OCR1A = 14400;//Count up to the desired time for the prescaled clock. i.e.(14400/14.7456M/1024)= 1 second
  
//Set interrupts last
  TIMSK1 |= (1<<OCIE1A);//Enable timer counter interrupts for Output Compare Register 1 A
  TIFR1 &= ~((1<<OCF1A);
  sei();//Set global interrupts enable.

}

uint8_t minutes;//globals to be used by ISA
uint8_t seconds;

void timer_delay(min, sec)
{
//Both minutes and seconds go up to 255 for a maximum delay of 4 hours, 19 minutes and 15 seconds.
  minutes = min;
  seconds = sec;
  init_timer();
}

ISR(TIMER1_COMPA_vect)
{
//increment/decrement, whatever we want.
  if(minutes!=0)
  {
    if(seconds==0)
     {
      seconds = 60;
      minutes--;
     }
    seconds--;
  }
// else, if no minutes left to count down
  else(minutes==0)
  {
	// check if seconds are still counting down
    if(seconds!=0)
	{
		seconds--;//decrement seconds
	}
	else //If no minutes or seconds to count down, disable timer counter
    {
	 TIMSK1 &= ~(1<<OCIE1A);//zero the mask to disable the interrupt.
	 PRR |= (1<<PRTIM1);//lower power consumption by disabling timer/counter1 in PRR.
    }	
  }
}

int main(){	
	while(1){}
}