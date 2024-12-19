#include <avr/io.h>
#include <util/atomic.h>
#include <avr/cpufunc.h>
#include <avr/pgmspace.h>
//#include "data.h"

PROGMEM volatile const uint8_t DRAFT_DATA[] = {
	1, 1,
	0x00,1
};

uint8_t my_C_function(uint8_t var){
	var += 20;
	_NOP();
	
	//asm volatile("nop");
	return var;
}

void drafts(void){
	
	uint8_t my_value;
	
	volatile uint8_t test;
	
	//my_value = sizeof(MINI_CIFRA_SP);
	test = pgm_read_byte(&DRAFT_DATA[0]);
	
	for (uint8_t i=0;i<sizeof(DRAFT_DATA);i++)
	{
		my_value = pgm_read_byte(&DRAFT_DATA[i]);
		PORTB = my_value;
	}
	
	PORTB = test;
	DDRB = 0xff;
	asmfunc_calledfrom_c(3);
	
	volatile uint8_t test_at;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		test_at = 10;
	};
	
	
	
	_NOP();

	
	
	//const volatile uint8_t *addr = &MINI_CIFRA_0[0];
	//volatile uint8_t res;
	
	asm volatile(
	"ldi ZL,lo8(%[addr])\n\t"
	"ldi ZH,hi8(%[addr])\n\t"
	//"lpm %0,Z+"
	://"=r" (res)
	:[addr] "i" (&DRAFT_DATA[0]));
	//:[addr] "i" (addr));
	
	set_Z_pointer(&DRAFT_DATA[0]);
	SPI_start();


	uint16_t addr16 = (uint16_t)(&DRAFT_DATA[0]);
	volatile uint8_t result;
	asm volatile
	(
	"lpm %0, Z" "\n\t"
	: "=r" (result)
	: "z" (addr16)
	);
	
	result = 0;
	
	
}