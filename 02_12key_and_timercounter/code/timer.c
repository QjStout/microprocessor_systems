// SET_TIMER.c
// for NerdKits with ATmega168

#include <avr/interrupt.h>

void init_timer();

void init_timer()
{
//Set correct bits in timer control register, flags, interrupts, etc.
//Set the timer up so that the OCF1A flag interrupts every second

	//CLEAR TCCR1A register
	TCCR1A ^= TCCR1A;
  
	//WGM12 - Set Timer/Counter Control Register to CTC mode
	TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);// CS12/CS10 - Set clock source to prescale clk/1024
	OCR1A = 14399;//Count up to the desired time for the prescaled clock. i.e.(14400/14.7456M/1024)= 1 second
	
	//Clear timer/counter 1 to ensure a count from 0
	TCNT1 ^= TCNT1;

	//Set interrupts last
	TIMSK1 = (1<<OCIE1A);//Enable timer counter interrupts for Output Compare Register 1 A
	sei();//Set global interrupts enable.
}

//globals to be used by ISA
//volatile so compiler doesn't optimise out the ISR changes
volatile uint8_t MINUTES = 0;
volatile uint8_t SECONDS = 0;

uint8_t GET_MINS()
{
	return MINUTES;
}

uint8_t GET_SECS()
{
	return SECONDS;
}

uint8_t TIMER_IS_CLEAR()
{
	uint8_t isClear = 0;
	if(MINUTES == 0 && SECONDS == 0){
		isClear = 1;
	}
	return isClear;
}

void ADD_MINS(uint8_t mins)
{
	mins += MINUTES;
	//Maximum time == 80min, 0sec
	//Do not allow over maximum time
	if(mins < 80){
		MINUTES = mins;
	}
	else{ //mins >= 80, set to max time
		MINUTES = 80;
		SECONDS = 0;
	}
}

void ADD_SECS(uint8_t secs)
{
	secs += SECONDS;
	//Do not allow to go over maximum time == 80min, 0sec
	if(MINUTES < 80){ //necessarry as timer stops at 80:00, & not at 80:59
		//if seconds is over a minute, remove the minutes and keep remainder
		if(secs > 59){
			SECONDS = secs % 60; //modulus to get remainder
			uint8_t mins = secs / 60; //integer division to get the minutes
			ADD_MINS(mins);
		}
		else{//otherwise, add to SECONDS timer counter
			SECONDS = secs;
		}
	}
}

void CLEAR_TIMER()
{
	MINUTES = 0;
	SECONDS = 0;
}

void SET_TIMER(uint8_t min, uint8_t sec)
{
//Both minutes and seconds go up to 255 for a maximum delay of 4 hours, 19 minutes and 15 seconds.
  MINUTES = min;
  SECONDS = sec;
  init_timer();
}

void PAUSE_TIMER()
{
	//Disable clock source 
	TCCR1B &= ~((1<<CS12)|(1<<CS10));
}

void RESUME_TIMER()
{
	TCCR1B |= (1<<CS12)|(1<<CS10);// CS12/CS10 - Set clock source to prescale clk/1024
}

ISR(TIMER1_COMPA_vect)
{
//increment/decrement, whatever we want.
  if(MINUTES!=0)
  {
    if(SECONDS==0)
    {
		SECONDS = 60;
		MINUTES--;
    }
	SECONDS--;
  }
  
  if(MINUTES==0)
  {
	if(SECONDS > 0){
		SECONDS--;
	}
	//Finally, disable timer counter
    else if(SECONDS==0)
    {
	 TIMSK1 &= ~(1<<OCIE1A);//zero the mask to disable the interrupt.  
    }
	else{
		SECONDS = 0;
	}
  }
}