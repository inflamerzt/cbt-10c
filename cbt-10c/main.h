#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
//#include <avr/interrupt.h> //included in cinterrupts.h
#include <util/atomic.h>
#include <time.h>

#include <stdbool.h>

#include "data.h"
#include "cinterrupts.h"
#include "LCD.h"
#include "defines.h"
#include "init.h"


#include "data.h"

#include "graphics.h"


volatile const uint8_t * smDig[10];
volatile const uint8_t * bigDig[10];

uint8_t BCD[8];
uint8_t test;

void init(void);

extern void asmfunc_calledfrom_c(uint8_t val);
extern void SPI_start(void);

extern void BCD_conversion24(uint32_t number);
extern void BCD_conversion16(uint16_t number);
extern uint16_t BCD_conversion8(uint8_t number);

extern uint32_t div24_8(uint32_t digit,uint8_t divider); 


PROGMEM volatile const uint8_t LCD_init[] = {
5,0, LC_nallon_dis,LC_pwron,LC_fillall_dis,LC_nor_dis, LC_nrev_dis, 
0xFF//; padding byte
};

#endif /* MAIN_H_ */