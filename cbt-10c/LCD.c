#include "LCD.h"

extern Element alarm_el;

volatile uint8_t LCD_pos_X;
volatile uint8_t LCD_pos_Y;

static inline void LCD_data(void){ PORTB |= (1<<P_MOSI); };
static inline void LCD_cmd(void){ PORTB &= ~(1<<P_MOSI); };

void LCD_send(const volatile uint8_t *data,uint8_t dc){
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
		if (!xcnt){if(y){
			y--;xcnt = x;
			LCD_goto_xy(LCD_pos_X,LCD_pos_Y-y);
			}}
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
	LCD_pos_X = X;
	LCD_pos_Y = Y;
	LCD_goto_xy(X,Y);
}

void LCD_goto_xy(uint8_t X,uint8_t Y){
	LCD_cmd();
	//;=== set X
	//;=== low 4 bits
	SPI_tx((X&0x0F));
	//;=== high 3 bits
	SPI_tx(((X>>4)&0x0F)|0x10);
	//SPI_tx((X>>4)|0x10);
	
	//;=== set Y
	SPI_tx((Y|0xB0));
	
}

void LCD_clr(void){
	uint8_t x = 96, y=9;

	LCD_data();
	do 
	{
		do 
		{
			x--;
			SPI_tx(0);	
		} while (x);
		x = 96; y--;
	} while (y);
	
	
	};
	
	
void LCD_sp(uint8_t i){
	LCD_cmd();
	do 
	{
		i--;
		SPI_tx(0);
	} while (i);
	
	
	};


void LCD_element(Element data){
	LCD_xy(data.X,data.Y);
	LCD_send(data.img,tx_data);
	};
	

