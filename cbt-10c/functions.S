
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


//.global SPI_start    ; Makes asmfunc_calledfrom_c visible in other source files

.extern my_value
.extern my_C_function
      
.section .text                  ; Defines a code section
 ;==========================================================================
	; Pepare SPI data
.global SPI_start
	SPI_start:
		lpm TXCount, Z+
		lpm TXRowCount,Z+
	SPI_start_defined: ; if TXCount,TXRowCount preloaded possible reduce size
		clr TXZCount
		lpm arg,Z+

		cp arg,zeroreg
		brne SPSnz
		lpm TXZCount,Z+
		SPSnz:

		cp TXRowCount,zeroreg
		breq SPI_TX
		;IF not save parameters to memory
		sts TXCountMem, TXCount
		sts TXRowCountMem, TXRowCount
	
		
	SPI_rstart:
		dec TXRowCount
		cp TXRowCount,zeroreg
		breq SPI_TX		
		rcall SPI_TX
		inc TXYpos
		rcall LCD_goto_XY
		sbi _SFR_IO_ADDR(PORTB),1//P_MOSI
		lds TXCount, TXCountMem
		
		
		cp TXZCount, zeroreg
		breq argnz
		dec TXZCount
		breq argnz
		rjmp SPrS_nz

		argnz:
		lpm arg,Z+
		cp arg, zeroreg
		brne SPI_rstart

		lpm TXZCount,Z+
		rjmp SPI_rstart		
		SPrS_nz:
	
		rjmp SPI_rstart


		
	SPI_TX:
		out _SFR_IO_ADDR(SPCR), zeroreg		;disable hardware SPI
		cli
		sbi _SFR_IO_ADDR(PORTB), P_SCK		;pull up SCK to send D/C SPI signal
		out _SFR_IO_ADDR(SPCR), spenreg		;enable hardware SPI
	
		sbis _SFR_IO_ADDR(PORTB),P_MOSI
		rjmp no_inv
		bst controlreg,inv_dis
		brtc no_inv
		mov tmpreg,arg
		com tmpreg 
		out _SFR_IO_ADDR(SPDR),tmpreg ;starting transfer
		rjmp inv_end			

		no_inv:
		out _SFR_IO_ADDR(SPDR),arg			;starting transfer
		inv_end:
		cbi _SFR_IO_ADDR(PORTB), P_SCK		;release SCK, after start to reduce cpu cycles
		sei
		;prepare new data before byte transmit complete
		dec TXCount
		breq wait_TX_complete
	;=========================

		cpi arg, 0
		breq no_load
		lpm arg, Z+
		cpi arg, 0
		brne transmit
		lpm TXZCount,Z+
		rjmp transmit

		no_load:
		cp TXZCount, zeroreg
		breq preload

		dec TXZCount
		brne transmit 
		preload:
		lpm arg,Z+
		transmit:

	wait_TX_complete:
		in tmpreg, _SFR_IO_ADDR(SPSR)
		sbrs tmpreg,SPIF
		rjmp wait_TX_complete
		cp TXCount, zeroreg
		brne SPI_TX 
		ret

;=================
	LCD_goto_XY:
		;.def TXXpos = r10
		;.def TXYpos = r11

		;=== set X
		;=== low 4 bits
		;load position
		push arg
		push TXRowCount
		clr TXRowCount
		cbi _SFR_IO_ADDR(PORTB),P_MOSI
		mov arg, TXXpos
		andi arg, 0x0F
		inc TXCount
		rcall SPI_TX
		;=== high 3 bits
		mov arg, TXXpos
		swap arg
		andi arg, 0x0F
		ori arg,0x10
		inc TXCount
		rcall SPI_TX

		;=== set Y
		mov arg,TXYpos //load position
		ori arg,0xB0 ; y2..0 (0..7)
		inc TXCount
		rcall SPI_TX
		pop TXRowCount
		pop arg
		ret

