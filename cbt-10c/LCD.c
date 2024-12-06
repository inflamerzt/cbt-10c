#include "LCD.h"


static inline void LCD_data(void){ PORTB |= (1<<P_MOSI); };
static inline void LCD_cmd(void){ PORTB &= ~(1<<P_MOSI); };

void LCD_puts(const volatile uint8_t *data,uint8_t dc){
	volatile uint8_t local,xcnt,x,y,zeroes = 0;
	
	asm volatile
	(
	"movw r30,r24" "\n\t"
	"lpm %0, Z+" "\n\t"
	"lpm %1, Z+" "\n\t"
	: "=r" (x), "=r" (y):);
	if (y){y--;};
	xcnt = x;
	
	if(dc){LCD_data();} else{LCD_cmd();};
	
	do{
		
		if (!zeroes) {
			asm volatile("lpm %0, Z+" "\n\t" : "=r" (local) :);
			if (!local)	{
				asm volatile("lpm %0, Z+" "\n\t" : "=r" (zeroes):);zeroes--;
			}
		}
		else{
			zeroes--;
		}
		
		xcnt--;
		if (!xcnt){if(y){y--;xcnt = x;}}
		SPI_tx(local);
		
	} while(xcnt); //size
	
};

void SPI_tx(uint8_t data){
	while(!(SPSR & (1<<SPIF)));
	SPCR = 0; //disable SPI
	cli();
	PORTB |= (1<<P_SCK);
	SPCR |= SPI_ENABLE;//(1<<MSTR)|(1<<SPE);
	SPDR = data;
	PORTB &= ~(1<<P_SCK);
	sei();
}

void LCD_xy(uint8_t X,uint8_t Y){
	
}
