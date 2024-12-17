/*
 * cinterrupts.c
 *
 * Created: 29.11.2024 11:46:45
 *  Author: sheiko
 */ 

#include "cinterrupts.h"
#include "avr/cpufunc.h"

extern uint8_t T1_ovf_count;

/* moved to interrupts.S
ISR(INT0_vect)
{
	//PORTB = 42;
};
*/

extern volatile uint16_t int_cps;

ISR(INT1_vect){
	
		int_cps++;
};

ISR(PCINT1_vect){

};

ISR(TIMER1_OVF_vect){T1_ovf_count++;};
	
	
ISR(TIMER0_COMPA_vect){
	_NOP();
	};


ISR(TIMER0_OVF_vect){
	_NOP();
	};