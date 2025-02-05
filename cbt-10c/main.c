/*
 * cbt-10c.c
 *
 * Created: 25.11.2024 0:45:51
 * Author : inflamer
 */ 


#include "main.h"
#include "avr\common.h"

#include "math.h"

struct {
	uint8_t a;
	uint8_t b;
	
} Mystruct;



Element alarm_el;
Element count_el;


volatile uint8_t systick; //0..125
volatile uint8_t systickh;

//256 clock divider *250*125 = 1s at 8MHz
volatile uint8_t second_count = 0; //systick interrupt every second count

volatile uint16_t current_cps_count; //maybe will be not a global var
volatile uint8_t T1_ovf_count;
volatile uint8_t booster_delay = 0; //skip periods counter for booster preset
volatile uint8_t booster_cnt = 0; //current booster counter



volatile uint8_t inversion;

volatile uint8_t alarm;
volatile uint8_t click;
volatile uint16_t cpstmp;
volatile uint8_t volume;
volatile uint8_t volpulsecnt;
volatile uint8_t mute;



volatile uint32_t cps_array[121]; //maximum cps_size = 120
volatile uint8_t cps_ptr;
volatile uint8_t cps_size;
volatile uint32_t cps_acc;
volatile uint32_t cps_mid_acc;
volatile uint8_t cps_cnt;

//#include "interrupts.s"

/*
get middle value of cps array
variables:
summ of elements
array of elements
array pointer

new summ = summ + current element(array[pointer-1]) - next element (array[pointer]) 

middle value = summ / elements


*/


void BCD_display32( unsigned long num );

uint32_t GetCPS(void);
inline void number_display(uint8_t size);
inline void dnumber_display(uint8_t number);
inline void big_number_display(uint8_t x,uint8_t y,uint8_t size);
void BCD_display32_big(uint8_t x, uint8_t y, unsigned long num );

register uint8_t bitstore asm("r4");


typedef enum {
	FIRST = 1 << 0, // same as 1
	SECOND = 1 << 1, // same as 2, binary 10
	THIRD = 1 << 2, // same as 4, binary 100
	FOURTH = 1 << 3 // same as 8, binary 1000
} flags;





int main()
{	

volatile uint32_t test32;
volatile uint8_t test8;
volatile float result32;

alarm = 0;
volume = 3;
click = 0;
volpulsecnt = 0;
mute = 0;

cps_acc = 0;
cps_ptr = 0;
cps_size = 10;
cps_cnt = 0;
	

	test32 = 0xFFFFFFFF;
	test8 = 0x0f;
	result32 = test32/test8;
	_NOP();
		test32 = 11100111;
		test8 = 110;
		result32 = (float)test32/(float)test8; //float aa aa aa 4e, uint32_t 55 55 55 00
	_NOP();
	
	test32 = (uint32_t)result32;
	
	result32 -= test32;
	
	test32 = (uint32_t)(result32*1000);
	
		_NOP();


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
	
	//LCD_element(alarm_el);
	

	//BCD_conversion24(123456);
	//big_number_display(0,6,8);


/*
	
		LCD_xy(75,1);
		//inv_dis;
		LCD_rclr(6,4);
		//nor_dis;
		
	LCD_element(count_el);
*/	



	//LCD_xy(0,4);
	
	//dnumber_display(99);
	
	//LCD_xy(0,3);
	//BCD_display32(0xFFFFFFFF);
	

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
		
		//LCD_xy(0,0);
		volatile uint32_t cps_value =  GetCPS();
		cps_acc += cps_value;
		//cps_acc += 2500000;
			
		BCD_conversion24(cps_value);
		big_number_display(0,0,8);
		
		//LCD_xy(0,1);
		
		cps_array[cps_ptr] = cps_value;
				
		
		cps_ptr++;
		if (cps_ptr == (cps_size + 1)) {cps_ptr = 0;};
		cps_mid_acc = cps_mid_acc + cps_value - cps_array[cps_ptr];	
			
		if (cps_cnt < cps_size) {cps_cnt++;};


		
				
		BCD_display32_big(0,2,cps_acc);
		
		//LCD_xy(0,2);
		
		
		//uint32_t intpart =  cps_mid_acc/cps_cnt;
		uint32_t intpart = div24_8(cps_mid_acc,cps_cnt);
		uint32_t fractpart = ((intpart>>24)*1000)/cps_cnt;
		//fractpart = (fractpart*1000)/cps_cnt;
		//intpart = intpart && 0xFFFFFF;
				
		BCD_conversion24(intpart);
		big_number_display(0,4,8);
		
		LCD_xy(64,5);
		LCD_send(Dot,tx_data);
		
		
		
		BCD_conversion24(fractpart);
		big_number_display(66,4,3);
		
		/*
		LCD_xy(50,3);
		
		//intpart =  1000*cps_mid_acc/cps_cnt;
		fractpart = (1000*cps_mid_acc)/cps_cnt - (cps_mid_acc/cps_cnt)*1000;
		
		BCD_conversion24(fractpart);
		number_display(8);
	
		LCD_xy(0,4);
		dnumber_display(cps_cnt);

		LCD_xy(0,5);
		dnumber_display(cps_ptr);	
		
		LCD_xy(0,6);	
		BCD_conversion24(cps_mid_acc);
		number_display(8);
		
		LCD_xy(0,7);
		BCD_conversion24(cps_array[cps_ptr]);
		number_display(8);
		*/
		
		// = 1;
		
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
	
	
void BCD_display32( unsigned long num )
{
	uint8_t datarray[10];
	
	unsigned long temp ;
	signed char index ;
	
	for ( index=9; num>0; index-- ) {
		temp = num / 10 ;
		datarray[index] = num-10*temp ;
		num = temp ;
	}
	for ( ; index>=0; index-- )
	datarray[index] = 0 ;
	
	for (index =0;index < 10;index++){
		LCD_send(smDig[datarray[index]],tx_data);
	}
}

inline void big_number_display(uint8_t x,uint8_t y,uint8_t size){
	uint8_t xpos;
	xpos = x;
	do
	{	
		LCD_xy(xpos,y);
		LCD_send(bigDig[BCD[size-1]],tx_data);
		size--; xpos += 8;
	} while (size);
};


void BCD_display32_big(uint8_t x, uint8_t y, unsigned long num )
{
	uint8_t datarray[10];
	uint8_t xpos;
	xpos = x;
	
	unsigned long temp ;
	signed char index ;
	
	for ( index=9; num>0; index-- ) {
		temp = num / 10 ;
		datarray[index] = num-10*temp ;
		num = temp ;
	}
	for ( ; index>=0; index-- )
	datarray[index] = 0 ;
	
	for (index =0;index < 10;index++){
		LCD_xy(xpos,y);
		LCD_send(bigDig[datarray[index]],tx_data);
		xpos += 8;
	}
}