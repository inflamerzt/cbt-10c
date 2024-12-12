/*
 * init.h
 *
 * Created: 06.12.2024 2:18:58
 *  Author: inflamer
 */ 


#ifndef INIT_H_
#define INIT_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "defines.h"


extern volatile const uint8_t MINI_CIFRA_0[];
extern volatile const uint8_t MINI_CIFRA_1[];
extern volatile const uint8_t MINI_CIFRA_2[];
extern volatile const uint8_t MINI_CIFRA_3[];
extern volatile const uint8_t MINI_CIFRA_4[];
extern volatile const uint8_t MINI_CIFRA_5[];
extern volatile const uint8_t MINI_CIFRA_6[];
extern volatile const uint8_t MINI_CIFRA_7[];
extern volatile const uint8_t MINI_CIFRA_8[];
extern volatile const uint8_t MINI_CIFRA_9[];

void init(void);
void start_count_cps(void);
void stop_count_cps(void);


#endif /* INIT_H_ */