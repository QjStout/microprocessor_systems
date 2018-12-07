//LED.h
#ifndef _LED_H
#define _LED_H

#include "LED.c"

void init_LEDS();
void LEDS_DISABLE();
void LEDS_ENABLE();
void LED_ON(char colour);
void LED_OFF();

#endif