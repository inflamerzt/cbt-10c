/*
 * asm_def.h
 *
 * Created: 29.11.2024 22:44:34
 *  Author: inflamer
 */ 


#ifndef ASM_DEF_H_
#define ASM_DEF_H_


#ifdef __ASSEMBLER__

#  define sreg_save	r2

#define TXCount r24
#define TXRowCount r23

#else  /* !ASSEMBLER */

//#include <stdint.h>

register uint8_t sreg_save asm("r2");

#endif /* ASSEMBLER */




#endif /* ASM_DEF_H_ */