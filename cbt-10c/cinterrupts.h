


#ifndef CINTERRUPTS_H_
#define CINTERRUPTS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//ISR(INT0_vect);
//ISR(INT1_vect);
ISR(PCINT1_vect);
extern ISR(TIMER0_COMPA_vect);
extern ISR(TIMER0_COMPB_vect);
extern ISR(TIMER2_COMPA_vect);
extern ISR(TIMER2_COMPB_vect);
//ISR(TIMER2_OVF_vect);


#endif