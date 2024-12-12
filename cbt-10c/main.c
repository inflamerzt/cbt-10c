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



Element alarm_el;

volatile uint16_t current_cps_count; //maybe will be not a global var
volatile uint8_t T1_ovf_count;


//#include "interrupts.s"

static inline void LCD_reset(void);

uint32_t GetCPS(void);


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
	
	volatile uint32_t testvar;
	//volatile uint32_t res;
	//volatile uint8_t result;
	//volatile uint8_t result1;
	
	volatile uint16_t testvar16;



alarm_el.X = 0;
alarm_el.Y = 0;
alarm_el.img = alarm_pic;


	start_count_cps();
	stop_count_cps();

	//testvar = GetCPS();
	

	init();
	
	LCD_reset();
	
	LCD_send(LCD_init,tx_cmd);
	
	//LCD_sp(5);
	
	LCD_clr();
	
	LCD_element(alarm_el);

	
	//LCD_xy(50,0);
	//LCD_send(alarm_el.img,tx_data);
	
	
		testvar = 0xF;
		uint8_t index;
	LCD_xy(0,4);
	
	testvar16 = BCD_conversion8(99);
	
	LCD_send(smDig[testvar16>>8],tx_data);
	LCD_send(smDig[testvar16&0xFF],tx_data);

	testvar16 = 0xFFFF;

	LCD_xy(0,2);
	index = 5;
			BCD_conversion24(testvar16);
		do
		{
			LCD_send(smDig[BCD[index-1]],tx_data);
			index--;
		} while (index);
	
	start_count_cps();

do {
	LCD_xy(0,0);
	
	BCD_conversion24(GetCPS());
	
	index = 8;
	do 
	{
	LCD_send(smDig[BCD[index-1]],tx_data);
	index--;
	} while (index);



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
	

uint32_t GetCPS(void){
	uint32_t value;
	value = T1_ovf_count;
	value = ((value<<16) | TCNT1);
	TCNT1 = 0;
	T1_ovf_count = 0;	
	return value;
	};
	