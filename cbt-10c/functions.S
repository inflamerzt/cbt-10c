
/*
 * functions.S
 *
 * Created: 26.11.2024 1:36:16
 *  Author: inflamer
 */ 

 #include "asm_def.h"
 //#include "main.h"

#define DDR_SPI DDRB
#define P_SS PB2
#define P_MOSI PB3
#define P_MISO PB4
#define P_SCK PB5
.equ n10m, 10000000
.equ n1m, 1000000
.equ n100k, 100000
.equ n10k, 10000
.equ n1k, 1000
.equ n100, 100
.equ n10, 10


//.global SPI_start    ; Makes asmfunc_calledfrom_c visible in other source files

.extern my_value
.extern my_C_function
 
.extern BCD     
.extern test

.section .text                  ; Defines a code section

;99 is return limited
.global BCD_conversion8
BCD_conversion8:
;r24 argument
push r0
ldi r25,0xCD
mul r24,r25
mov r25,r1
lsr r25
lsr r25
lsr r25
ldi r23,10
mul r23,r25
sub r24,r0
clr r1

;return ;ldi r25,0x11
;return ;ldi r24,0x22
pop r0
ret

.global BCD_conversion16 
 BCD_conversion16:
 push r16
 movw r22,r24
 ldi XL,lo8(BCD)
 ldi XH,hi8(BCD)
 adiw XL,8
 clr r16
 st -X,r16
 st -X,r16
 st -X,r16

 ldi r18,lo8(n10k)
 ldi r19,hi8(n10k)
 
 rcall test2b 
 st -X,r16

 rjmp BCD_conversion16s

.global BCD_conversion24 
 BCD_conversion24:
 ;r19-r24 (r24 b0, r25 b1, r22 b2, r23 b3 ...r18 b6 ,r19 b7)
 ;r25 r24 r23 r22 - 32bit value
 push r16

 ldi XL,lo8(BCD)
 ldi XH,hi8(BCD)
 adiw XL,8
 ;load 10M
 ldi r18,lo8(n10m)
 ldi r19,hi8(n10m)
 ldi r20,hlo8(n10m)
 clr r16
 rcall test3b
 st -X,r16
 ;load 1M
 ldi r18,lo8(n1m)
 ldi r19,hi8(n1m)
 ldi r20,hlo8(n1m)
 clr r16
 rcall test3b
 st -X,r16
 ;load 100K
 ldi r18,lo8(n100k)
 ldi r19,hi8(n100k)
 ldi r20,hlo8(n100k)
 clr r16
 rcall test3b
 st -X,r16

 ;load 10K
 ldi r18,lo8(n10k)
 ldi r19,hi8(n10k)
 ldi r20,hlo8(n10k)
 clr r16
 rcall test3b
 st -X,r16
 BCD_conversion16s:
 ;load 1K
 ldi r18,lo8(n1k)
 ldi r19,hi8(n1k)
 clr r16
 rcall test2b 
 st -X,r16
 ;load 100
 ldi r18,lo8(n100)
 ldi r19,hi8(n100)
 clr r16
 rcall test2b 
 st -X,r16
 ;load 10
 ldi r18,10
 clr r16
 test10:
 cpi r22,10
 brlo less10
 subi r22,10
 inc r16
 rjmp test10
 less10:
 st -X,r16
 st -X,r22

 pop r16
 ret


 test3b:
 cp r22,r18
 cpc r23,r19
 cpc r24,r20
 brsh conttest3b
 ret
 conttest3b:
 sub r22,r18
 sbc r23,r19
 sbc r24,r20
 inc r16
 rjmp test3b

 test2b:
 cp r22,r18
 cpc r23,r19
 brsh conttest2b
 ret
 conttest2b:
 sub r22,r18
 sbc r23,r19
 inc r16
 rjmp test2b