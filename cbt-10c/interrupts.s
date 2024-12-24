
/*
 * functions.S
 *
 * Created: 26.11.2024 1:36:16
 *  Author: inflamer
 */ 

#define __SFR_OFFSET 0          // Use 0 for the I/O register offset
#include <avr/io.h>             // Defines I/O port aliases
#include "defines.h"
      
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
nop

reti

.global TIMER2_COMPA_vect
TIMER2_COMPA_vect:
nop
sbi PORTD, PD3
reti

.global TIMER2_COMPB_vect
TIMER2_COMPB_vect:
nop
cbi PORTD, PD3
reti

.global TIMER2_OVF_vect
TIMER2_OVF_vect:
nop
reti



.global TIMER0_COMPA_vect
TIMER0_COMPA_vect:
push tmpreg
in tmpreg, SREG ;1
push tmpreg; 2
lds tmpreg, booster_cnt
tst tmpreg
brne skip_discharge
;nop
;discharge cap
;sbi transistor
;delay 20us
;cbi transistor 
;check feedback and manipulate frequency
cbi PORT_booster, P_bCap
sbi PORT_booster, P_bTrans
push tmpregh
ldi tmpreg,52

;delay 20us //160 cycles maybe do something useful?
dly20us:
dec tmpreg
brne dly20us

lds tmpreg, booster_delay

sbis PINC,P_boostFB
rjmp freq_up
;freq_down
ldi tmpregh, 255 ; higest delay
cp tmpreg,tmpregh
brsh end_T0_CMPA
sec
rol tmpreg
rjmp end_T0_CMPA

freq_up:
tst tmpreg
breq end_T0_CMPA
lsr tmpreg


end_T0_CMPA:

sts booster_cnt, tmpreg
sts booster_delay, tmpreg

pop tmpregh
pop tmpreg; 2
out SREG,tmpreg ;1
pop tmpreg
reti

skip_discharge:
dec tmpreg
sts booster_cnt, tmpreg
pop tmpreg; 2
out SREG,tmpreg ;1
pop tmpreg
reti



.global TIMER0_COMPB_vect
TIMER0_COMPB_vect:
push tmpreg
in tmpreg, SREG ;1
push tmpreg; 2
lds tmpreg, booster_cnt
tst tmpreg
brne skip_charge


;charge capacitor and shift to open IRF840
;------------------------------------------
;sbic suppressor pin
//sbic PINC,P_boostFB
//reti

;cbi transistor
;sbi diod
;cbi capacitor

cbi PORT_booster, P_bTrans
sbi PORT_booster, P_bDiode
cbi PORT_booster, P_bCap

;do anything minimum 6 cpu cycles to charge cap x8 = 48cycles


; 38 cycles
ldi tmpreg, 10 ; simplify and charge 6,5us
w6us:
nop ;1
dec tmpreg ; 1
brne w6us ;2/1


;sbi capacitor
;cbi diod
sbi PORT_booster, P_bCap
cbi PORT_booster, P_bDiode

pop tmpreg; 2
out SREG,tmpreg ;1
pop tmpreg
reti

skip_charge:
pop tmpreg; 2
out SREG,tmpreg ;1
pop tmpreg
reti


//.global 

;.global TIMER1_OVF_vect
;TIMER1_OVF_vect:

;reti