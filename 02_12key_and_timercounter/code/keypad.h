//keypad.h

#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "keypad.c"

void keypad_init();
uint8_t KEY_PRESSED();
char keypad_read(char lastchar);


#endif