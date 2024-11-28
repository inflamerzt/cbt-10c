/*
 * data.h
 *
 * Created: 26.11.2024 1:40:30
 *  Author: inflamer
 */ 


#ifndef DATA_H_
#define DATA_H_

#include <avr/pgmspace.h>

PROGMEM volatile const uint8_t MINI_CIFRA_SP[] = {
	1, 1,
	0x00,1
	};

PROGMEM volatile const uint8_t MINI_CIFRA_0[] = {
	5, 1,
	0x00,1,0x3E,0x41,0x41,0x3E
	};

PROGMEM volatile const uint8_t MINI_CIFRA_1[] = {
	5, 1, 
0x00,1,0x04,0x02,0x7F,0x00,1,0xFF
	};

PROGMEM volatile const uint8_t MINI_CIFRA_2[] = {
	5, 1, 
0x00,1,0x62,0x51,0x49,0x46
	};

PROGMEM volatile const uint8_t MINI_CIFRA_3[] = {
	5, 1, 
0x00,1,0x22,0x41,0x49,0x36
	};

PROGMEM volatile const uint8_t MINI_CIFRA_4[] = {
	5, 1, 
0x00,1,0x18,0x14,0x12,0x7F
	};

PROGMEM volatile const uint8_t MINI_CIFRA_5[] = {
	5, 1, 
0x00,1,0x27,0x45,0x45,0x39
	};
	
PROGMEM volatile const uint8_t MINI_CIFRA_6[] = {
	5, 1, 
0x00,1,0x3E,0x49,0x49,0x32
	};

PROGMEM volatile const uint8_t MINI_CIFRA_7[] = {
	5, 1,
0x00,1,0x01,0x61,0x1D,0x03
	};

PROGMEM volatile const uint8_t MINI_CIFRA_8[] = {
	5, 1,
0x00,1,0x36,  0x49,   0x49,0x36
	};

PROGMEM volatile const uint8_t MINI_CIFRA_9[] = {
	5, 1,
0x00,1,0x26,   0x49,0x49,0x3E
	};

PROGMEM volatile const uint8_t ne_CIFRA[] = {
	8, 2,
0x00, 16
	};

PROGMEM volatile const uint8_t MINI_dot[] = {
	1,1, 
0x80, 0
	};

PROGMEM volatile const uint8_t MINI_Ddot[] = {
	1,1, 
0x24 , 0
	};

#endif /* DATA_H_ */