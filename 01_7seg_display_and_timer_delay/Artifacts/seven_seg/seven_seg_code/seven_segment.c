// seven_segment.c
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

//declaration of external function to send a zero to the display - implement in assembler
extern uint8_t zero_to_display();

//function declarations
void hex_to_disp(uint8_t hex_num);
void init_disp()

// PIN DEFINITIONS:
// 
// PB5 -- segment b
// PB4 -- segment a
// PB3 -- segment f
// PC3 -- segment g
// PC2 -- segment e
// PC1 -- segment d
// PC3 -- segment c
//
//      a
//     ---  
//  f |   | b
//     --- <- g
//  e |   | c
//     ---
//      d

//Lookup table for decoding the seven segment display.
static const uint8_t seg_table[15] = 
{
//Outputting 0 on a pin activates the segment, a 1 turns off the segment. 
//	 
// [# 0 0 0 0 0 0 0]
//	^ ^ ^ ^ ^ ^ ^ ^
//	| b a f g e d c
//	|
//	Bit seven is unneeded. Or the decimal point, maybe?
//
//		0 			1			2			3
	0b00001000, 0b00111110, 0b00010001, 0b00010100,
//		4			5			6			7
	0b00100110, 0b01000100, 0b10000000, 0b00011110,
//		8			9			A			b
	0b00000000, 0b00000100, 0b00000010, 0b01100000,
//		c 			d 			E			F
	0b01110001, 0b00110000, 0b01000001, 0b01000011 
};

void hex_to_disp(hex_num)
{
  uint8_t seg_code = seg_table[hex_num]
	
  //Lowest four bits of seg_table match with PORTC lower bits, but we'll need to replace them.
  //Bitmask clears high bits to keep OR'd PORTC high bits unchanged
  PORTC |= (seg_code & ((1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0)))//0s to 1s //  [0XXX####]&[00001111]=[0000####]
  //bitmask the upper four bits as ones to keep AND'd PORTC bits
  PORTC &= (seg_code | ~((1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0)));//1s to 0s //[0XXX####]|[11110000]=[1111####]
  
  //seg_table bits need to be right shifted one place to match PORTB
  //Then just bitmask for the PORTB bits
  PORTB |= ((seg_code>>1) & ((1<<PB5)|(1<<PB4)|(1<<PB3)));//0s to 1s // [00XXX###]&[00111000]=[00XXX000]
  PORTB &= ((seg_code>>1) | ~((1<<PB5)|(1<<PB4)|(1<<PB3)));//1s to 0s //[00XXX###]|[11000111]=[11XXX111]
}

void init_disp()
{
  // Set the 7 pins to output, DDRC|=[00001111]
  DDRC |= ((1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0);
  //DDRB|=[00111000]
  DDRB |= (1<<PB5)|(1<<PB4)|(1<<PB3);
  
  // Initialize PORTC and PORTB assuming seven seg display is connected to corresponding pins.
  // Turn on all segments.
  PORTC &= ~(1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0);// PORTC&=[11110000]
  PORTB &= ~(1<<PB5)|(1<<PB4)|(1<<PB3);// PORTB [00111000]
  //NOTE: check the ATmega168 datasheet for current output limits for the ports (Notes on bottom of page 305).
  //Seven pins outputing a high signal all at once might need a lot of current.	
}

int main() {
  // start up the LCD
  lcd_init();
  lcd_home();
  
  //Turn on all segments of the display
  init_disp();
  
  //Send an initial zero to the display.
  zero_to_display();
  
  while(1) {
	int i;//Only need the first four bits in this int.
	//Count up on the display with timer delay.
	for(i=0;i<=15;i++)
	{
		hex_to_disp(i);
		lcd_write_byte(i);//Should really find the LCD documentation	
		
		//NOTE: We could use Task 2 to set up the timing/delay and replace the delay_ms() function here. 
		delay_ms(500);
	}
	
  }
  return 0;
}
