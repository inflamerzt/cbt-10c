
/*
 * functions.S
 *
 * Created: 26.11.2024 1:36:16
 *  Author: inflamer
 */ 

#define __SFR_OFFSET 0          // Use 0 for the I/O register offset
#include <avr/io.h>             // Defines I/O port aliases
#include "defines.h"
      
//.extern my_value
      
.extern systick_low
.extern systick_high

.extern click
.extern alarm
.extern cpstmp
.extern volume
.extern volpulsecnt
.extern mute

.section .text                  ; Defines a code section

.global INT0_vect
INT0_vect:
nop

reti

.global TIMER2_COMPA_vect
//every 8ms (25 times = 0.2s)
TIMER2_COMPA_vect:
push tmpreg
in tmpreg, SREG ;1
push tmpreg; 2

nop
//sbi PORTD, PD3
;working with systick vars
lds tmpreg, systick
dec tmpreg
brne T2_COMPA_end
lds tmpreg, systickh
inc tmpreg
cpi tmpreg, 0x0F
brlo T2_nosec

lds tmpreg, second_count
inc tmpreg
sts second_count,tmpreg

clr tmpreg
T2_nosec:
sts systickh, tmpreg

ldi tmpreg, 125

T2_COMPA_end:
sts systick,tmpreg

lds tmpreg, mute
cpi tmpreg, 0
brne alarmend

lds tmpreg, alarm
cpi tmpreg, 0
breq noalarm
;set port alarm
push tmpregh
lds tmpregh, volume
lds tmpreg, volpulsecnt
inc tmpregh
inc tmpreg
cpi tmpreg, 15
brlo volpulseok
clr tmpreg
volpulseok:
sts volpulsecnt, tmpreg
cp tmpreg, tmpregh
brlo setal
resetal:
cbi PORT_buzz, P_buzz
rjmp popvolreg
setal:
sbi PORT_buzz, P_buzz

popvolreg:
pop tmpregh


rjmp alarmend
noalarm:
;reset port beep
cbi PORT_buzz, P_buzz

lds tmpreg, click
cpi tmpreg, 0
breq noclick

// maybe need to push registers
push tmpreg
push tmpregh
push r18
push r19
// click or not?
lds tmpreg,TCNT1L
lds tmpregh,TCNT1H
lds r18,cpstmp
lds r19,cpstmp+1

cp tmpreg, r18
cpc tmpregh, r19
breq nclick

sbi PORT_buzz, P_buzz
sts cpstmp, tmpreg
sts cpstmp+1, tmpregh
rjmp clickpop
nclick:
cbi PORT_buzz, P_buzz
clickpop:
pop r19
pop r18
pop tmpregh
pop tmpreg

rjmp alarmend
noclick:
cbi PORT_buzz, P_buzz

alarmend:





pop tmpreg; 2
out SREG,tmpreg ;1
pop tmpreg
reti

.global TIMER2_COMPB_vect
TIMER2_COMPB_vect:
nop
//cbi PORTD, PD3
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