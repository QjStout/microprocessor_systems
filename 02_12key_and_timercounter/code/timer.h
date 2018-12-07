//SET_TIMER.h
#ifndef _SET_TIMER_H
#define _SET_TIMER_H

#include "timer.c"

uint8_t GET_MINS();
uint8_t GET_SECS();
uint8_t TIMER_IS_CLEAR();
void ADD_MINS(uint8_t mins);
void ADD_SECS(uint8_t secs);
void CLEAR_TIMER();
void SET_TIMER(uint8_t min, uint8_t sec);
void PAUSE_TIMER();
void RESUME_TIMER();
ISR(TIMER1_COMPA_vect);


#endif