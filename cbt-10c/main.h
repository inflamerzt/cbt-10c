#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <time.h>


#include "data.h"

#define DDR_booster DDRD
#define PORT_booster PORTD
#define P_bDiode PD0
#define P_bCap PD1
#define P_bTrans PD2


#define DDR_LCD_RST DDRD
#define P_LCD_RES PD7

void init(void);

#endif /* MAIN_H_ */