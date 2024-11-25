/*
 * cbt-10c.c
 *
 * Created: 25.11.2024 0:45:51
 * Author : inflamer
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <time.h>

#include "main.h"

//#include "interrupts.s"

extern void asmfunc_calledfrom_c(uint8_t val);


ISR(INT0_vect)
{
	    PORTB = 42;
}



uint8_t my_C_function(uint8_t var){
	var += 20;
	_NOP();
	
	//asm volatile("nop");
	return var;
}  

unsigned char my_value;

int main()
{
	init();
	
	my_value = 0x55;
	DDRB = 0xff;
	asmfunc_calledfrom_c(3);
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		
	}
	
	while(1);
	return 0;
}

void init(void){
		PRR = (1<<PRTWI)|(1<<PRUSART0);
		SMCR = (1<<SE);//|(2<<SM0); //; idle sm=000
		DDRD = (1<<P_LCD_RES)|(1<<P_bDiode)|(1<<P_bCap)|(1<<P_bTrans)|(1<<PD6);
	
}