/*
 * cbt-10c.c
 *
 * Created: 25.11.2024 0:45:51
 * Author : inflamer
 */ 


#include "main.h"



//#include "interrupts.s"

extern void asmfunc_calledfrom_c(uint8_t val);


ISR(INT0_vect)
{
	    PORTB = 42;
}


uint8_t my_value;


uint8_t my_C_function(uint8_t var){
	var += 20;
	_NOP();
	
	//asm volatile("nop");
	return var;
}  

uint8_t my_value;

int main()
{
	init();
	volatile uint8_t test;
	
	//my_value = sizeof(MINI_CIFRA_SP);
	test = pgm_read_byte(&MINI_CIFRA_SP[0]);
	
	for (uint8_t i=0;i<sizeof(MINI_CIFRA_SP);i++)
	{
		my_value = pgm_read_byte(&MINI_CIFRA_SP[i]);
		PORTB = my_value;
	}
	
	PORTB = test;
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