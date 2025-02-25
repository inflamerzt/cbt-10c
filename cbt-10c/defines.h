/*
 * defines.h
 *
 * Created: 06.12.2024 2:09:43
 *  Author: inflamer
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#include <avr/io.h>

#ifdef __ASSEMBLER__



#define tmpreg r16
#define tmpregh r17


#define DDR_booster DDRD
#define PORT_booster PORTD
#define P_bDiode PD0
#define P_bCap PD1
#define P_bTrans PD2
//#define PORT_bTrans PORTC
//#define P_bTrans PC6
#define	P_boostFB PC5

#define PORT_buzz PORTB
#define P_buzz PB0


/* this section must be removed */
#define sreg_save	r2

#define TXCount r24
#define TXRowCount r23
#define TXZCount r18
#define arg r19
#define spenreg r21
#define controlreg r22
#define inv_dis 0

//#define tmpreg r25
#define TXXpos r17

#define TXYpos r20

#define zeroreg r1
/*=================================*/

#else

/* this section must be removed */
register uint8_t sreg_save asm("r2");
/*=================================*/

/*

#define DDR_LCD_RST DDRD
#define P_LCD_RES PD7
*/

#define meas_pin_vcc 1 //; pull measuring pin to vss to measure, comment othervise

#define cps_buf_size 20 //; size of cps bytes

#define clr_data_size 100 //; size of data to cleanup

#define LC_pwroff 0x20
#define LC_pwron 0x2F
#define LC_nallon_dis 0xA4 //;normal display
#define LC_allon_dis 0xA5 //;all segments disabled ?standby mode
#define LC_nor_dis 0xA6 //;normal display
#define LC_inv_dis 0xA7 //;inverted display
#define LC_nfillall_dis 0xAE //;display ram
#define LC_fillall_dis 0xAF //;hide ram

//;original display settings
#define LC_nrev_dis 0xA0
#define LC_rev_dis 0xA1

//; TIM1 prescaller x/16
//;160 ms must be 5ms to 1s count 90...16000 0x50...0x5000

//; need to add additional variable to add aditional 256 cycles at maximum
#define DCboost_period 0x28 //;0x50 ;small timing = fast boost
#define DCboost_pulse 4 //;8 ;512us - 22us (interrupt delay)

//;set quantity bytes to clear on startup (.DSEG)
#define clrb_onreset 11

//;timer position
#define timer_posx 28 //;40 - max
#define timer_posy 0

//;rad_anim_position
#define rad_an_posx 0
#define rad_an_posy 2


//;icons shift values
#define scount_pic 2
#define salarm_pic 0
#define sthreshold_pic 2
#define sclk_vol_pic 2
#define sbright_pic 0
#define ssen_set_pic 0
#define sbat_cal_pic 6
#define scontr_pic 0

//;voltage measure pin
#define Vmeas_DDR DDRC
#define Vmeas_port PORTC
#define P_Vmeas PC0


//;buttons
#define button1 PC1
#define button2 PC2
#define button3 PC3

#define tx_data 1
#define tx_cmd 0




/*

;��������� ����� ������ ������������ 0 1 S5 S4 S3 S2 S1 S0
;��������� ����� ������ ������������ S:0?Y?63

;=== set start scan number S:0?Y?63 shift dram LCD
;ldi tmpreg,0 //load position
;andi tmpreg,0x3F
;ori tmpreg,0x40 ; y2..0 (0..7)
;inc TXCount
;rcall SPI_TX_cmd

*/

#define DDR_SPI DDRB
#define P_SS PB2
#define P_MOSI PB3
#define P_MISO PB4
#define P_SCK PB5

#define DDR_booster DDRD
#define PORT_booster PORTD
#define P_bDiode PD0
#define P_bCap PD1
#define P_bTrans PD2

#define DDR_LCD_RST DDRD
#define P_LCD_RES PD7

#define P_boostFB PC5

#define DDR_buzz DDRB
#define P_buzz PB0

//#define TXCount r16;(TXCount)

#define inv_dis inversion=0xFF;
#define nor_dis inversion=0;


#define set_Z_pointer(zaddr) \
(__extension__({                \
	uint16_t __addr16 = (uint16_t)(zaddr); \
	__asm__ __volatile__			\
	(								\
	"ldi ZL,lo8(%[addr])\n\t"	\
	"ldi ZH,hi8(%[addr])\n\t"	\
	:       \
	:[addr] "i" (__addr16)				\
	);								\
}))



#define SPI_ENABLE (1<<MSTR)|(1<<SPE)


typedef struct {
	uint8_t X;
	uint8_t Y;
	const volatile uint8_t * img;
} Element;


#endif
#endif /* DEFINES_H_ */