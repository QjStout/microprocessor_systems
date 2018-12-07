// keypad.c
// for NerdKits with ATmega168
// Ganesh Arulampalam 14/09/2012
// ganesh.arulampalam@ecu.edu.au
// Reading keypad

// PIN DEFINITIONS:
// 
// PB5 -- KEYPAD COL1(PIN3) (Left column)
// PC0 -- KEYPAD COL2(PIN1) (centre column)
// PC1 -- KEYPAD COL3(PIN5) (right column)
// PC2 -- KEYPAD ROW1(PIN2) (top row)
// PC3 -- KEYPAD ROW2(PIN7) (second row)
// PC4 -- KEYPAD ROW3(PIN6) (third row)
// PC5 -- KEYPAD ROW4(PIN4) (forth row)

void keypad_init() {
  // set ports to proper input and output
  // enable pull up resistors for inputs
  
  // Set the columns as ouput
  // Set row pins to input mode  
  // PortC bits 0 and 1 and 2 as output (columns)
  DDRC |=  (1<<PC1 | 1<<PC0);//[00000111]
  DDRB |= (1<<PB5);
  // Set PortC pins 2, 3, 4 and 5 to input
  DDRC &= ~(1<<PC5 | 1<<PC4 | 1<<PC3 | 1<<PC2);//[11000111]
	  
  // turn on the internal pull up resistors for the input pins
  PORTC |= (1<<PC5 | 1<<PC4 | 1<<PC3 | 1<<PC2);

  // Set column output pins low, so input low if contact made
  PORTC &= ~(1<<PC1 | 1<<PC0 ); // set PC0, PC1 and PC2 low
  PORTB &= ~(1<<PB5);

}  // END keypad_init

//**************************************************

 uint8_t KEY_PRESSED() {
  // checks to see if any key has been pressed
  // sets columns to 0, and checks if any row goes low
  // Returns a 1 (true) if a key has been pressed, 0 if not pressed
 
  // Set column output pins low, so input low if contact made
  
  uint8_t rowvl;
  uint8_t kp;
  
  // Set column output pins low, so input low if contact made
  PORTC &= ~(1<<PC1 | 1<<PC0 ); // set PC0, PC1 and PC2 low
  PORTB &= ~(1<<PB5);
  
  delay_us(10);// delay to allow signals to settle
  // Read value from Pins on PortC and PortB
  // Then shift right 3 to get pins3, 4 and 5 in first 3 bits
  rowvl =  (PINC >> 2); 									  //[00####00] -> [0000####]
  rowvl &= (1<<PC3 | 1<<PC2 | 1<<PC1 | 1<<PC0);   // mask out other bits [0000####] & [00001111]
  
  kp = (rowvl != 15);              // if 15, all high so no key	[0000####] != 0b00001111
 
  return kp;
}


//**************************************************

char keypad_read(char lastchar) {
  // read from keypad 
  // (assumes key has been pressed)
  // returns keyval 0..9
  // takes in lastchar, and returns that if invalid read from keypad
  uint8_t rowval;
  char keych;     // Initialise to $
  keych = '$';
  
  PORTC |= (1<<PC1 | 1<<PC0);  // set  other 2 high
  PORTB &= ~(1<<PB5);               // set PB5  low - check column 1
  delay_us(10);                      // delay to allow signals to settle
  
  // Read value from Pins on PortC
  // Then shift right 2 to get pin values into first four bits of rowval
  rowval =  (PINC >> 2);
  rowval &= (1<<PC3 | 1<<PC2 | 1<<PC1 | 1<<PC0);   // mask out other bits
  
  switch(rowval) {
  case 14: keych = '1';
          break;
  case 13: keych = '4';
          break;		  
  case 11: keych = '7';
          break;
  case 7 : keych = '*';
		  break;
  default: keych = '$';
           break;
  }
  
  if (keych=='$') {                     // if still not valid key
	 
	 PORTC |= (1<<PC1);  // set  other 2 high
	 PORTB |= (1<<PB5);
     PORTC &= ~(1<<PC0);               // set PC0 low - check column 2
     delay_us(10);                       // delay to allow signals to settle
    
    // Read value from Pins on PortC
    // Then shift right 2 to get pin values into first four bits of rowval
    rowval =  (PINC >> 2);
    rowval &= (1<<PC3 | 1<<PC2 | 1<<PC1 | 1<<PC0);   // mask out other bits
  
     switch(rowval) {
     case 14: keych = '2';
              break;
     case 13: keych = '5';
             break;		  
     case 11: keych = '8';
             break;
	 case 7 : keych = '0';
			 break;
     default: keych = '$'; 
	         break;		
     }  	  
  }
		  
  if (keych=='$') {         // if still not valid key
	 
	 PORTC |= (1<<PC0);  // set  other 2 high
	 PORTB |= (1<<PB5);
     PORTC &= ~(1<<PC1);               // set PC1 low - check column 3
     delay_us(10);                       // delay to allow signals to settle
     
    // Read value from Pins on PortC
    // Then shift right 2 to get pin values into first four bits of rowval
    rowval =  (PINC >> 2);
    rowval &= (1<<PC3 | 1<<PC2 | 1<<PC1 | 1<<PC0);   // mask out other bits
  
     switch(rowval) {
     case 14: keych = '3';
              break;
     case 13: keych = '6';
             break;		  
     case 11: keych = '9';
             break;
	 case 7 : keych = '#';
			 break;
     default: keych = '$'; 
	         break;		
     } 	  		  
  }	  
  
  if (keych != '$')  {     // if valid character
      lastchar = keych;    // update last char
	}  
  return lastchar;
  
}  // END keypad_read
