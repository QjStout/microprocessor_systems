#include "avr/io.h"
 
.global zero_to_disp
zero_to_disp:
  ;cbi/sbi cannot be used on addressess outside the range
  ;  0x00 - 0x1F
  
  ;PORTC - 0x08 (0x28)
  ;PORTB - 0x05 (0x25)
  cbi 0x08, 2			;Clear bit 2 in PORTC - E
  cbi 0x08, 1			;Clear bit 1 in PORTC - D
  cbi 0x08, 0			;Clear bit 0 in PORTC - C
  sbi 0x08, 3			;Set bit 3 in PORTC - G
  
  cbi 0x05, 5			;Clear bit 5 in PORTB - B
  cbi 0x05, 4			;Clear bit 4 in PORTB - A
  cbi 0x05, 3			;Clear bit 3 in PORTB - F
  
  ret

.end
