// Assignment2.c
// for NerdKits with ATmega168

#define F_CPU 14745600

#include <stdio.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <ctype.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"
#include "../libnerdkits/uart.h"

#include "keypad.h"
#include "timer.h"
#include "LED.h"

//Global declarations
volatile char CURR_PROG = 'C';
volatile char PREV_PROG = 'C';
FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
volatile char keychar = 'C'; // Just a default character

void program_one()
{
	static char currState = '3'; //persistent value to maintain current state of program_one
	/*
	 * State List
	 *		G - Green LED sequence is running
	 *		1 - A one second delay after G & before Y
	 *		Y - Yellow LED sequence is running
	 *		2 - A one second delay after Y & before R
	 *		R - Red LED sequence is running
	 *		3 - A one second delay after R & before G
	 */
	
	//Define behaviour for input of '1', 
	//		while program_one is already running
	if(PREV_PROG == '1'){
		//Behaviour:
		//		Start again.
		CURR_PROG = 'C';
		PREV_PROG = 'C';
	}

	//Effectively works as an init for the first loop
	if(CURR_PROG != '1'){
		//Turn off all LEDs, set default state, set 1sec timer to ensure gap between LEDs
		LED_OFF();
		currState = '3';
		SET_TIMER(0,1);
		
		//Setup display layout for program_one
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program One-"));
		lcd_line_two();
		lcd_write_string(PSTR("       G   Y   R"));
		lcd_line_three();
		lcd_write_string(PSTR("Secs:  0   0   0"));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
	}
	
	//if the timer is cleared,
	//	then turn on the next LED in sequence
	//	!NOTE! - LEDs must be initialised before the timer, as LED sets 1 second delay
	
	if(TIMER_IS_CLEAR()){
		switch(currState)
		{
			case 'G': //Transition to '1'
				//Set state flag to '1', turn off LEDs & set a timer
				currState = '1';
				LED_OFF(); //Don't use LEDS_DISABLE, otherwise it messes up main() & Emergency ISR()
				SET_TIMER(0, 1);
				
				//Set the 'G' timer value on the LCD to '0'
				lcd_goto_position(2,7);
				lcd_write_string(PSTR("0"));
			break;
			case '1': //Transition to 'Y'
				//Set the delay timer value on the LCD to '0'
				lcd_goto_position(3,7);
				lcd_write_string(PSTR("0"));
				
				//Set state flag to 'Y', turn on 'Y' LED & set a timer
				currState = 'Y';
				LED_ON(currState);				
				SET_TIMER(0,2);
			break;
			case 'Y': //Transition to '2'	
				//Set state flag to '2', turn off LEDs & set a timer
				currState = '2';
				LED_OFF(); //Don't use LEDS_DISABLE, otherwise it messes up main() & Emergency ISR()
				SET_TIMER(0, 1);
				
				//Set the 'Y' timer value on the LCD to '0'
				lcd_goto_position(2,11);
				lcd_write_string(PSTR("0"));
			break;
			case '2': //Transition to 'R'
				//Set the delay timer value on the LCD to '0'
				lcd_goto_position(3,7);
				lcd_write_string(PSTR("0"));
				
				//Set state flag to 'R', turn on 'R' LED & set a timer
				currState = 'R';
				LED_ON(currState);
				SET_TIMER(0,3);
			break;
			case 'R': //Transition to '3'
				//Set state flag to '3', turn off LEDS & set a timer
				currState = '3';
				LED_OFF(); //Don't use LEDS_DISABLE, otherwise it messes up main() & Emergency ISR()
				SET_TIMER(0,1);
				
				//Set the 'R' timer value on the LCD to '0'
				lcd_goto_position(2,15);
				lcd_write_string(PSTR("0"));
			break;
			case '3': //Transition to 'G'
				//Set the delay timer value on the LCD to '0'
				lcd_goto_position(3,7);
				lcd_write_string(PSTR("0"));
				
				//Set state flag to 'G', turn on 'G' LED & set a timer
				currState = 'G';
				LED_ON(currState);
				SET_TIMER(0,5);
			break;
		}
	}
	
	//Display remaining seconds of current LED
	switch(currState)
	{
		case 'G':
			lcd_goto_position(2,7); //Go to G counter position
			fprintf(&lcd_stream, "%u", GET_SECS());
		break;
		case 'Y':
			lcd_goto_position(2,11); //Go to Y counter position
			fprintf(&lcd_stream, "%u", GET_SECS());
		break;
		case 'R':
			lcd_goto_position(2,15); //Go to R counter position
			fprintf(&lcd_stream, "%u", GET_SECS());
		break;
		default:
			lcd_goto_position(3, 7); //Go to delay counter position
			fprintf(&lcd_stream, "%u", GET_SECS());
		break;
	}
}

void program_two()
{
	//Persistent variable to track state of program_two
	static char currState = '2'; //Default to '2' so switch starts at G
	/*
	 *	State List:
	 *		G - Green LED sequence is running
	 *		1 - A 1 second delay after G and before R
	 *		R - Red LED sequence is running
	 *		2 - A 1 second delay after R and before G
	 */
	
	//Define behaviour for input of '2', 
	//		while program_two is already running
	if(PREV_PROG == '2'){
		
		//Behaviour:
		//		Start again.
		CURR_PROG = 'C';
		PREV_PROG = 'C';
	}

	//Works as an init for the first loop
	if(CURR_PROG != '2'){
		//Turn off all LEDs, set default state, set 1 sec timer to ensure gap between LEDs
		LED_OFF();
		currState = '2';
		SET_TIMER(0,1);
		
		//Setup display layout for program_two
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program Two-"));
		lcd_line_two();
		lcd_write_string(PSTR("       R   G"));
		lcd_line_three();
		lcd_write_string(PSTR("Secs:  0   0"));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
	}
	
	if(TIMER_IS_CLEAR()){
		switch(currState){
			case 'R': //transition to '1'
				//Set state flag to '1', turn off LEDS & set a timer
				currState = '1';
				LED_OFF();
				SET_TIMER(0,1);
				
				//Set the 'R' timer value on the LCD to '0'
				lcd_goto_position(2,7);
				lcd_write_string(PSTR("0"));
			break;
			case '1': //Transition to 'G'
				//Set the delay timer value on the LCD to '0'
				lcd_goto_position(3,7);
				lcd_write_string(PSTR("0"));
				
				//Set state flag to 'G', turn on 'G' LED & set a timer
				currState = 'G';
				LED_ON(currState);
				SET_TIMER(0,3);
			break;
			case 'G': //Transition to '2'
				//Set the state flag to '2', turn off LEDS & set a timer
				currState = '2';
				LED_OFF();
				SET_TIMER(0,2);
				
				//Set the 'G' timer value on the LCD to '0'
				lcd_goto_position(2,11);
				lcd_write_string(PSTR("0"));
			break;
			case '2'://transition to R
				//Set the delay timer value on the LCD to '0'
				lcd_goto_position(3,7);
				lcd_write_string(PSTR("0"));
				
				//Set state flag to 'R', turn on 'R' LED & set a timer
				currState = 'R';
				LED_ON(currState);
				SET_TIMER(0,4);
			break;
		}
	}
	
	//Display updated timer countdown
	switch(currState){
			case 'R':
				//update Red LED timer
				lcd_goto_position(2,7);
				fprintf(&lcd_stream, "%u", GET_SECS());
			break;
			case 'G':
				//update Green LED timer
				lcd_goto_position(2,11);
				fprintf(&lcd_stream, "%u", GET_SECS());
			break;
			default:
				//update delay timer
				lcd_goto_position(3, 7); //Go to delay counter position
				fprintf(&lcd_stream, "%u", GET_SECS());
			break;
		}
}

void program_four()
{
	/*
	 * I know this is some pretty shoddy code, but it's late and I can't focus.
	 * Don't hate me, cos yah ain't me. (Shakes finger sassilly)
	 */
	static uint8_t isReady = 0;
	
	//Define behaviour for input of '4', 
	//		while program_four is already running
	if(PREV_PROG == '4'){
		//Set PREV_PROG to default so it can be run again
		PREV_PROG = 'C';
		
		//Behaviour:
		//		Extend program_four time by 5sec from current time
		if(isReady == 0){
			//if the timer is clear, the LED will be off
			//	in which case, set isReady flag so LED is turned back on
			if(TIMER_IS_CLEAR()){
				isReady = 1;
			}
			else{
				ADD_SECS(5); //Don't change. Must be here, otherwise program can get stuck when adding time to a cleared timer.
			}
		}
	}

	//Works as an init for the first loop
	if(CURR_PROG != '4'){
		//Turn off all LEDs, set 1 second delay for LED switch, set isReady flag
		LED_OFF();
		SET_TIMER(0,1);
		isReady = 1;
		
		//Format LCD display layout for program_four
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program Four-"));
		lcd_line_two();
		lcd_write_string(PSTR("      R"));
		lcd_line_three();
		lcd_write_string(PSTR("Mins: 0, Secs: 0  "));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
	}
	
	if(TIMER_IS_CLEAR() == 1 && isReady == 1){
		//clear isReady flag
		isReady = 0;
		
		//Turn on 'R' led and set timer for 5sec
		LED_ON('R');
		SET_TIMER(0,5);
		
		//set current 'R' counter
		lcd_line_three();
		lcd_write_string(PSTR("Mins: 0, Secs: 5  "));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 0"));
	}
	else if(isReady == 0){
		//set 'R' counter on LCD
		lcd_line_three();
		fprintf(&lcd_stream, "Mins: %u, Secs: %u   ", GET_MINS(), GET_SECS());
	}
	
	//if timer has hit zero, turn off LEDs.
	if(TIMER_IS_CLEAR()){
		LED_OFF();
	}
}

void program_six()
{
	/*
	 * I know this is some pretty shoddy code, but it's late and I can't focus.
	 * Don't hate me, cos yah ain't me. (Shakes finger sassilly)
	 */
	static uint8_t isReady = 0;
	
	//Define behaviour for input of '6', 
	//		while program_six is already running
	if(PREV_PROG == '6'){
		//Set PREV_PROG to default so it can be run again
		PREV_PROG = 'C';
		
		//Behaviour:
		//		Extend program_four time by 5sec from current time
		if(isReady == 0){
			//if the timer is clear, the LED will be off
			//	in which case, set isReady flag so LED is turned back on
			if(TIMER_IS_CLEAR()){
				isReady = 1;
			}
			else{
				ADD_SECS(5); //Don't change. Must be here, otherwise program can get stuck when adding time to a cleared timer.
			}
		}
	}

	//Works as an init for the first loop
	if(CURR_PROG != '6'){
		//Turn off all LEDs, set 1 second delay for LED switch, set isReady flag
		LED_OFF();
		SET_TIMER(0,1);
		isReady = 1;
		
		//Format LCD display layout for program_four
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program Six -"));
		lcd_line_two();
		lcd_write_string(PSTR("      G"));
		lcd_line_three();
		lcd_write_string(PSTR("Mins: 0, Secs: 0  "));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
	}
	
	if(TIMER_IS_CLEAR() == 1 && isReady == 1){
		//clear isReady flag
		isReady = 0;
		
		//Turn on 'R' led and set timer for 5sec
		LED_ON('G');
		SET_TIMER(0,5);
		
		//set current 'R' counter
		lcd_line_three();
		lcd_write_string(PSTR("Mins: 0, Secs: 5  "));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 0"));
	}
	else if(isReady == 0){
		//set 'R' counter on LCD
		lcd_line_three();
		fprintf(&lcd_stream, "Mins: %u, Secs: %u   ", GET_MINS(), GET_SECS());
	}
	
	//if timer has hit zero, turn off LEDs.
	if(TIMER_IS_CLEAR()){
		LED_OFF();
	}
}

void program_seven()
{
	static uint8_t led_on = 1;
		//Define behaviour for input of '7', 
	//		while program_seven is already running
	if(PREV_PROG == '7'){
		//Behaviour:
		//		Start again.
		PREV_PROG = 'C';
		
		//toggle flag to enable/disable led
		if(led_on == 0)
		{
			led_on = 1;
		}
		else
		{
			led_on = 0;
		}
	}

	//Effectively works as an init for the first loop
	if(CURR_PROG != '7'){
		
		//Turn off all LEDs and set one second LED change delay
		LED_OFF();
		SET_TIMER(0,1);
		
		//set led flag so initial state is on
		led_on = 1;
		
		//Format LCD display layout for program_four
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program Seven-"));
		lcd_line_two();
		lcd_write_string(PSTR("      R"));
		lcd_line_three();
		lcd_write_string(PSTR("      OFF"));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
		
	}
	
	//
	if(TIMER_IS_CLEAR())
	{
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 0"));
		
		if(led_on == 1)
		{
			lcd_line_three();
			lcd_write_string(PSTR("      ON "));
		
			LED_ON('R');	
		}
		else
		{
			lcd_line_three();
			lcd_write_string(PSTR("      OFF"));
			LED_OFF();
		}
		
	}
	
}

void program_eight()
{
	static uint8_t led_on = 1;
		//Define behaviour for input of '8',
	//		while program_eight is already running
	if(PREV_PROG == '8'){
		//Behaviour:
		//		Start again.
		PREV_PROG = 'C';
		
		//toggle flag to enable/disable led
		if(led_on == 0)
		{
			led_on = 1;
		}
		else
		{
			led_on = 0;
		}
	}

	//Effectively works as an init for the first loop
	if(CURR_PROG != '8'){
		
		//Turn off all LEDs and set one second LED change delay
		LED_OFF();
		SET_TIMER(0,1);
		
		//set led flag so initial state is on
		led_on = 1;
		
		//Format LCD display layout for program_four
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program Eight-"));
		lcd_line_two();
		lcd_write_string(PSTR("      Y"));
		lcd_line_three();
		lcd_write_string(PSTR("      OFF"));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
		
	}
	
	//
	if(TIMER_IS_CLEAR())
	{
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 0"));
		
		if(led_on == 1)
		{
			lcd_line_three();
			lcd_write_string(PSTR("      ON "));
		
			LED_ON('Y');	
		}
		else
		{
			lcd_line_three();
			lcd_write_string(PSTR("      OFF"));
			LED_OFF();
		}
		
	}
	
}

void program_nine()
{
	static uint8_t led_on = 1;
		//Define behaviour for input of '9', 
	//		while program_nine is already running
	if(PREV_PROG == '9'){
		//Behaviour:
		//		Start again.
		PREV_PROG = 'C';
		
		//toggle flag to enable/disable led
		if(led_on == 0)
		{
			led_on = 1;
		}
		else
		{
			led_on = 0;
		}
	}

	//Effectively works as an init for the first loop
	if(CURR_PROG != '9'){
		
		//Turn off all LEDs and set one second LED change delay
		LED_OFF();
		SET_TIMER(0,1);
		
		//set led flag so initial state is on
		led_on = 1;
		
		//Format LCD display layout for program_four
		lcd_clear_and_home();
		lcd_write_string(PSTR("   -Program Nine-   "));
		lcd_line_two();
		lcd_write_string(PSTR("      G "));
		lcd_line_three();
		lcd_write_string(PSTR("      OFF"));
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 1"));
		
	}
	
	//
	if(TIMER_IS_CLEAR())
	{
		lcd_line_four();
		lcd_write_string(PSTR("DELAY: 0"));
		
		if(led_on == 1)
		{
			lcd_line_three();
			lcd_write_string(PSTR("      ON "));
		
			LED_ON('G');	
		}
		else
		{
			lcd_line_three();
			lcd_write_string(PSTR("      OFF"));
			LED_OFF();
		}
	}
}

void program_zero()
{
	/*
	 * Initialise program_zero()
	 *
	 *	- Use state to determine if waiting for mins or secs
	 *	- '*' = exit,	'#' = confirm selection
	 */
	
	//Array to maintain user inputs
	char state[4] = {
		'0', //0 : Minutes, 1st digit
		'0', //1 : Minutes, 2nd digit
		'0', //2 : Seconds, 1st digit
		'0'  //3 : Seconds, 2nd digit
	};
	
	//Array to find lcd display positions of M, m, S & s
	uint8_t lcdPos[4] = {
		12,
		13,
		15,
		16
	};
	
	//variables to maintain state
	uint8_t currState = 0;
	volatile uint8_t paused = 0;
	
	//local variable for user input
	char localKeychar = 'C'; //a default value
	
	//Format lcd display
	lcd_clear_and_home();
	lcd_write_string(PSTR("    Program Zero    "));
	lcd_line_three();
	lcd_write_string(PSTR("Enter Time: 00:00"));
	
	//Use while to 'lock' into program_zero until done,
	//	while also allowing an exit after ISR.
	while(CURR_PROG != 'C')
	{
		//check if key is pressed
		if(KEY_PRESSED()){
			localKeychar = keypad_read(localKeychar);
			
			//Debounce input
			delay_ms(200); //can't use our timer as it interferes with some sequences, e.g. program_four
		}
		
		//State == 0-3 means input M, m, S or s, respectively
		if(currState >=0 && currState <=3){
			//Clear line two & display a '*' above the current selected digit
			lcd_line_two();
			lcd_write_string(PSTR("                    "));
			lcd_goto_position(1,lcdPos[currState]);
			lcd_write_string(PSTR("*"));
			
			//If the user input a digit, display to lcd & store value
			if(isdigit(localKeychar)){
				lcd_goto_position(2,lcdPos[currState]); //set to the relevant LCD position
				fprintf(&lcd_stream, "%c", localKeychar); //Display user input
				state[currState] = localKeychar; //Store user input into associated index
			}
			//if user hit the 'BACK' key
			else if(localKeychar == '*'){
				localKeychar = 'C'; //So the input doesn't loop
				currState -= 1;
			}
			//if the user hits the 'NEXT' key
			else if(localKeychar == '#'){
				localKeychar = 'C'; //So the input doesn't loop
				currState += 1;
			}
		}
		//State == 4 means user inputs need to be validated and sequence started
		else if(currState == 4){
			//Calculate minutes
			uint8_t mins = (state[0]-48)*10 + (state[1]-48); //Subtract 48 to convert from char to the corresponding decimal
			
			//Calculate seconds
			uint8_t secs = (state[2]-48)*10 + (state[3]-48);//Subtract 48 to convert from char to the corresponding decimal
			
			//Flag whether to run sequence
			uint8_t runSequence = 0;
			
			//Validate user input
			if((mins < 80 && secs < 60) || (mins == 80 && secs == 0)){
				runSequence = 1;
			}
			
			//Check if user hit back button
			if(localKeychar == '*'){
				localKeychar = 'C'; //clear so doesn't loop
				currState -= 1;
			}
			//Initialise sequence if no problems validating
			else if(runSequence){
				//Set a timer and turn on LED
				SET_TIMER(mins, secs);
				LED_ON('R');
				
				//Format lcd display
				lcd_clear_and_home();
				lcd_write_string(PSTR("    Program Zero    "));
				lcd_line_two();
				lcd_write_string(PSTR("Time Left:"));
				
				currState += 1; //To change to run state
			}
			else {
				lcd_line_two();
				lcd_write_string(PSTR("INVALID - MAX=80:00"));
				
			}
			
		}
		//State == 5 means the sequence is running
		else if(currState == 5){
			//Display remaining time
			lcd_line_three();
			
			fprintf(&lcd_stream, "           %u:%u  ", GET_MINS(), GET_SECS());
			
			if(TIMER_IS_CLEAR()){
				currState = -1; //Exit program_zero
			}
			else if(localKeychar == '#'){
				//Clear input so doesn't repeat
				localKeychar = 'C';
				
				//Toggle the timer & motor
				if(paused == 0){
					PAUSE_TIMER();
					LED_OFF();
					paused = 1;
				}
				else if(paused == 1){
					RESUME_TIMER();
					LED_ON('R');
					paused = 0;
				}
			}
			else if(localKeychar == '*'){
				//exit program_zero
				currState = -1;
			}
		}
		//State < 0 means exit program_zero
		else {
			LED_OFF(); //ensure LEDs are off before exiting
			CURR_PROG = 'C';
		}
		
		//Display line four controls
		lcd_line_four();
		if(currState == 0){
			lcd_write_string(PSTR("EXIT: *, NEXT: #"));
		}
		else if(currState == 3){
			lcd_write_string(PSTR("BACK: *, DONE: #"));
		}
		else if(currState == 4){
			lcd_write_string(PSTR("BACK: *           "));
		}
		else if(currState == 5){
			lcd_write_string(PSTR("EXIT: *, PAUSE #"));
		}
		else{
			lcd_line_four();
			lcd_write_string(PSTR("BACK: *, NEXT: #"));
		}
	}
}

void init_E_stop()
{
	// Set PB1 to input
	DDRB &= ~(1<<PB1);//Set emergency switch pin to input
	PORTB |= (1<<PB1);
	
	//Set pb1 PCINT0 interrupt	
	PCICR |= (1<<PCIE0);
	
	//Disable all interrupts on PCI0, except PCINT0
	PCMSK0 ^= PCMSK0;
	PCMSK0 |= (1<<PCINT1);
}

ISR(PCINT0_vect){
	// Emergency mode runs here
	// 1. diable all motors - don't just toggle off, they must be disabled
	LED_OFF();
	LEDS_DISABLE();
	
	//clear interrupt flag so it doesn't run twice
	PCIFR &= ~(1<<PCIF0);
	
	//Set globals to default values
	keychar = 'C';
	CURR_PROG = keychar;
	PREV_PROG = keychar;
	
	//Clear timer
	CLEAR_TIMER(); //Must clear timer so that program_zero exits correctly
	
	//Display emergency message
	//lcd_clear_and_home();
	lcd_line_one();
	lcd_write_string(PSTR("  EMERGENCY MODE    "));
	lcd_line_two();
	lcd_write_string(PSTR("                    "));
	lcd_line_three();
	lcd_write_string(PSTR(" Press * to reset   "));
	lcd_line_four();
	lcd_write_string(PSTR("                    "));
	//wait for emergency release key
	while(keychar != '*')
	{
		//check if key is pressed
		if(KEY_PRESSED()){
		keychar = keypad_read(keychar);
		//Debounce input
		//delay_ms(100); //can't use our timer as it interferes with some sequences, e.g. program_four
		}
	}
}


int main() {
  // start up the LCD & create stream
  lcd_init();
  lcd_home();
  
  //initialise LEDs
  init_LEDS();
  
  //initialise keypad
  keypad_init();
  
  //initialise emergency stop button
  init_E_stop();
  
  PCICR |= (1<<PCIE0); //enable interrupts on PCINT0
  sei(); //enable global interrupts
  
  lcd_clear_and_home();

  
  while(1)
  {	
	LEDS_ENABLE(); // Initialise LEDS, ready to be toggled on/off
	
	//check if key is pressed
	if(KEY_PRESSED()){
		keychar = keypad_read(keychar);
		PREV_PROG = CURR_PROG;
		
		//Debounce input
		delay_ms(200); //can't use our timer as it interferes with some sequences, e.g. program_four
	}
	
	//Go to program selected by user
	switch(keychar)
	{
		case '1':
			program_one();
			CURR_PROG = keychar; //!WARNING! - must come after function
		break;
		case '2':
			program_two();
			CURR_PROG = keychar; //!WARNING! - must come after function call
		break;
		case '4':
			program_four();
			CURR_PROG = keychar; //!WARNING! - must come after function call
		break;
		case '6':
			program_six();
			CURR_PROG = keychar; //!WARNING! - must come after function call
		break;
		case '7':
			program_seven();
			CURR_PROG = keychar; //!WARNING! - must come after function call
		break;
		case '8':
			program_eight();
			CURR_PROG = keychar; //!WARNING! - must come after function call
		break;
		case '9':
			program_nine();
			CURR_PROG = keychar; //!WARNING! - must come after function call
		break;
		case '0':
			//Sets program_zero to initialise mode
			CURR_PROG = 'I';
			program_zero();
			
			CURR_PROG = 'C';
			keychar = 'C'; //A default value so that program_zero doesn't repeat
		break;
		default: 
			CURR_PROG = 'C';
			LED_OFF();
			lcd_line_one();
			lcd_write_string(PSTR("Select a program: "));
			lcd_line_two();
			lcd_write_string(PSTR("        0-2,      "));
			lcd_line_three();
			lcd_write_string(PSTR("          4,      "));
			lcd_line_four();
			lcd_write_string(PSTR("        6-9       "));
	}
  }
}