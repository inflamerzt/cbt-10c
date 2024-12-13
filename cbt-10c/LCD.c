#include "LCD.h"

extern Element alarm_el;
extern volatile uint8_t inversion;

volatile uint8_t LCD_pos_X;
volatile uint8_t LCD_pos_Y;

void LCD_goto_xy(uint8_t X,uint8_t Y);

static inline void LCD_data(void){ PORTB |= (1<<P_MOSI); };
static inline void LCD_cmd(void){ PORTB &= ~(1<<P_MOSI); };

void LCD_send(const volatile uint8_t *data,uint8_t dc){
	volatile uint8_t local,xcnt,x,y,sp=0,zeroes = 0;
	uint8_t Ystor = LCD_pos_Y;
	
	asm volatile
	(
	"movw r30,r24" "\n\t"
	"lpm %0, Z+" "\n\t"
	"lpm %1, Z+" "\n\t"
	: "=r" (x), "=r" (y):);
	if (y){y--;};
	xcnt = x;
	LCD_pos_Y++;
	
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
		if (!xcnt){if(y){y--;xcnt = x; sp = 1;};};
		SPI_tx(local^inversion);
		if (sp){
				LCD_xy(LCD_pos_X,LCD_pos_Y);//ycnt++;
				LCD_pos_Y++;
				if(dc){LCD_data();} else{LCD_cmd();};
				sp = 0;
			};


		
	} while(xcnt); //size
	LCD_pos_Y = Ystor; 
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
	SPI_tx(((Y&0x07)|0xB0));
	
}

void LCD_clr(void){
	uint8_t x = 96, y=9;

	LCD_data();
	do 
	{
		do 
		{
			x--;
			SPI_tx(inversion);	
		} while (x);
		x = 96; y--;
	} while (y);
	
	
	};
	
	
void LCD_sp(uint8_t i){
	LCD_data();
	do 
	{
		i--;
		SPI_tx(inversion);
	} while (i);
	
	
	};
	
void LCD_rclr(uint8_t xs,uint8_t ys){
		uint8_t yst = LCD_pos_Y;
		volatile uint8_t ycnt = ys;
	do
	{
		LCD_xy(LCD_pos_X,LCD_pos_Y);

		LCD_data();
		for (uint8_t x=0;x<xs;x++)
		{
			SPI_tx(inversion);
		};
		LCD_pos_Y++;
		ycnt--;		
	}while(ycnt);
	LCD_pos_Y = yst;
};



void LCD_element(Element data){
	LCD_xy(data.X,data.Y);
	LCD_send(data.img,tx_data);
	};
	

