/*
 * cinterrupts.c
 *
 * Created: 29.11.2024 11:46:45
 *  Author: sheiko
 */ 

#include "cinterrupts.h"

extern uint8_t T1_ovf_count;

/* moved to interrupts.S
ISR(INT0_vect)
{
	//PORTB = 42;
};
*/

ISR(PCINT1_vect){
	
};

ISR(TIMER1_OVF_vect){T1_ovf_count++;};