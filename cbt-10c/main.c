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
Element count_el;

volatile uint8_t systick; //0..125 every 8ms tick
//256 clock divider *250*125 = 1s at 8MHz
volatile uint8_t second_count = 0; //systick interrupt every second count

volatile uint16_t current_cps_count; //maybe will be not a global var
volatile uint8_t T1_ovf_count;
volatile uint8_t booster_delay = 0; //skip periods counter for booster preset
volatile uint8_t booster_cnt = 0; //current booster counter



volatile uint8_t inversion;

//#include "interrupts.s"


uint32_t GetCPS(void);
inline void number_display(uint8_t size);
inline void dnumber_display(uint8_t number);


register uint8_t bitstore asm("r4");


typedef enum {
	FIRST = 1 << 0, // same as 1
	SECOND = 1 << 1, // same as 2, binary 10
	THIRD = 1 << 2, // same as 4, binary 100
	FOURTH = 1 << 3 // same as 8, binary 1000
} flags;





int main()
{	

	sreg_save = SREG;

	//SP = RAMEND;

	
	nor_dis;
	


/* possible unusable part */
alarm_el.X = 50;
alarm_el.Y = 1;
alarm_el.img = alarm_pic;

count_el.X = 50;
count_el.Y = 1;
count_el.img = count_pic;
//=============================

	init();
	
	systick = 125;
	
	start_booster();
	
	LCD_send(LCD_init,tx_cmd);
	
	//LCD_sp(5);
	LCD_xy(0,0);
	
	LCD_clr();
	
	LCD_element(alarm_el);


/*
	
		LCD_xy(75,1);
		//inv_dis;
		LCD_rclr(6,4);
		//nor_dis;
		
	LCD_element(count_el);
*/	



	LCD_xy(0,4);
	
	dnumber_display(99);
	

	//EIMSK |= (1<<INT1);
	start_count_cps();
	

		LCD_xy(0,0);
		
		BCD_conversion24(GetCPS());
		number_display(8);

do { //infinite loop

	if (!second_count){	sleep_cpu();}
	else{
		// every second
		second_count--;
		
		LCD_xy(0,0);
			
		BCD_conversion24(GetCPS());
		number_display(8);
		
	}
}
	while(1);
	//return 0;
	// never run section for tests only

	
}

//=======================================================================================

uint32_t GetCPS(void){
	uint32_t value;
	value = T1_ovf_count;
	cli();
	value = ((value<<16) | TCNT1);
	TCNT1 = 0;
	sei();
	T1_ovf_count = 0;	
	return value;
	};
	
inline void number_display(uint8_t size){
		do
		{
			LCD_send(smDig[BCD[size-1]],tx_data);
			size--;
		} while (size);
	};
	
inline void dnumber_display(uint8_t number){
		uint16_t bcdnumber = BCD_conversion8(number);
		LCD_send(smDig[bcdnumber>>8],tx_data);
		LCD_send(smDig[bcdnumber&0xFF],tx_data);
	};
	