
/*
 * functions.S
 *
 * Created: 26.11.2024 1:36:16
 *  Author: inflamer
 */ 

#define __SFR_OFFSET 0          // Use 0 for the I/O register offset
#include <avr/io.h>             // Defines I/O port aliases
      
.global asmfunc_calledfrom_c    ; Makes asmfunc_calledfrom_c visible in other source files

//.extern my_value
.extern my_C_function
      
.section .text                  ; Defines a code section

/*      
asmfunc_calledfrom_c:           ; Start of asmfunc_calledfrom_c subroutine
	lds r16, my_value
	ldi r24, 1
	rcall my_C_function
     out  PORTB, r24             ; Send value passed to asmfunc_calledfrom_c to PORTB
     ret

*/

.global INT0_vect
INT0_vect:


reti

//.global 

;.global TIMER1_OVF_vect
;TIMER1_OVF_vect:

;reti