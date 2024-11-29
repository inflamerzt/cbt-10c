/*
 * asm_def.h
 *
 * Created: 29.11.2024 22:44:34
 *  Author: inflamer
 */ 


#ifndef ASM_DEF_H_
#define ASM_DEF_H_


#ifdef __ASSEMBLER__

#include <avr/io.h>


#  define sreg_save	r2

#define TXCount r24
#define TXRowCount r23
#define TXZCount r18
#define arg r19
#define spenreg r21
#define controlreg r22
#define inv_dis 0

#define tmpreg r25
#define TXXpos r17

#define TXYpos r20

#define zeroreg r1



#else  /* !ASSEMBLER */

//#include <stdint.h>

register uint8_t sreg_save asm("r2");

#endif /* ASSEMBLER */




#endif /* ASM_DEF_H_ */