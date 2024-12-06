/*
 * cbt-10c.c
 *
 * Created: 25.11.2024 0:45:51
 * Author : inflamer
 */ 


#include "main.h"
#include "avr\common.h"

struct {
	uint8_t a;
	uint8_t b;
	
} Mystruct;



//#include "interrupts.s"

static inline void LCD_reset(void);

uint8_t TXCountMem;
uint8_t TXRowCountMem;


register uint8_t bitstore asm("r4");


typedef enum {
	FIRST = 1 << 0, // same as 1
	SECOND = 1 << 1, // same as 2, binary 10
	THIRD = 1 << 2, // same as 4, binary 100
	FOURTH = 1 << 3 // same as 8, binary 1000
} flags;





int main()
{	


	//SP = RAMEND;

	init();
	
	LCD_reset();
	
	_NOP();
	
	
	//uint8_t flags = FIRST | SECOND| THIRD | FOURTH;
	volatile flags flag;
	flag = FIRST; //| SECOND;
	
	bitstore |= 0x01;

	LCD_puts(LCD_init,tx_cmd);
	
	LCD_xy(0,0);
	
	LCD_puts(ne_CIFRA,tx_data);

do {
	LCD_puts(MINI_CIFRA_3,tx_data);
	
	_delay_ms(1000);
}
	while(1);
	//return 0;
	// never run section for tests only

	
}








	
static inline void LCD_reset(void) {
		PORTD &= (1<<P_LCD_RES);
		_delay_ms(10);
		PORTD |= (1<<P_LCD_RES);
		_delay_ms(10);

	};
	
