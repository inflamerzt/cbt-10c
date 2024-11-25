// raw.s
#define __SFR_OFFSET 0          // Use 0 for the I/O register offset
#include <avr/io.h>             // Defines I/O port aliases
      
.global asmfunc_calledfrom_c    ; Makes asmfunc_calledfrom_c visible in other source files

.extern my_value
.extern my_C_function
      
.section .text                  ; Defines a code section
      
asmfunc_calledfrom_c:           ; Start of asmfunc_calledfrom_c subroutine
	lds r16, my_value
	ldi r24, 1
	rcall my_C_function
     out  PORTB, r24             ; Send value passed to asmfunc_calledfrom_c to PORTB
     ret
