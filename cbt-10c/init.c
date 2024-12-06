/*
 * init.c
 *
 * Created: 06.12.2024 2:19:22
 *  Author: inflamer
 */ 

#include "init.h"



void init(void){

		PRR = (1<<PRTWI)|(1<<PRUSART0);
		//SMCR = (1<<SE);//|(2<<SM0); //; idle sm=000
		DDRD = (1<<P_LCD_RES)|(1<<P_bDiode)|(1<<P_bCap)|(1<<P_bTrans)|(1<<PD6);
		PORTD &= ~(1<<P_LCD_RES);
		
		//;=SPI init
		DDR_SPI = (1<<P_SS)|(1<<P_SCK)|(1<<P_MOSI);
		DDR_SPI &= ~(1<<P_MISO);
		
		Vmeas_DDR |= (1<<P_Vmeas);

		Vmeas_port |= P_Vmeas;
		
		set_sleep_mode(SLEEP_MODE_IDLE);
		//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		//sei();
		
		//sleep_cpu();
		//sleep_disable();
		
		sei();
		
		
		
		/*
		;delay for LCD reset must be implemented 2500ns pull low, 2500ns after reset
		; 10 - 100ms hx1230 recomendation
		;use timer+idle sleep to reduce power consumption.  LCD backlight  (timer0)
		; out OCR0A, 10  - 10ms period or other stable reset timeout ex 100ms * 8 (1->8MHz)
		; configure prescaller 1024
		; out TCNT0, zeroreg -  resets timer0 count
		; PULL LCD_reset to 0
		; 10ms is a long time do something before sleep and release reset
		; code injection under LCD reset
*/
		//TIFR0 = 0;
		//TIMSK0 |= (1<<OCIE0A);
		//OCR0A = 80;
		//TCCR0B |= (5<<CS00);
		//TCNT0 = 0;
		

		
/*=======================================================
Do something usefull when LCD reset pulled low and sleep
=========================================================*/



	s_d.d0 = MINI_CIFRA_0;
	s_d.d1 = MINI_CIFRA_1;
	s_d.d2 = MINI_CIFRA_2;
	s_d.d3 = MINI_CIFRA_3;
	s_d.d4 = MINI_CIFRA_4;
	s_d.d5 = MINI_CIFRA_5;
	s_d.d6 = MINI_CIFRA_6;
	s_d.d7 = MINI_CIFRA_7;
	s_d.d8 = MINI_CIFRA_8;
	s_d.d9 = MINI_CIFRA_9;


/* 
		;=init predefined registers
		;clr zeroreg ;--------- defined at reset
		ldi r16,(1<<MSTR)|(1<<SPE)
		mov spenreg, tmpreg
		
		ldi Zl,low(wait_TX_complete)
		ldi Zh,high(wait_TX_complete)
		movw TXC_ptrl,Zl

		;ldi r16,(1<<MSTR)|(1<<SPE);|(1<<SPR0)
		;out SPCR, spenreg

		in tmpreg, SPSR
		ori tmpreg, (1<<SPI2x)
		out SPSR, tmpreg


		; fill pointers

*/ 

		SPCR |= SPI_ENABLE;
		SPSR |= (1<<SPI2X);
		SPDR = 0; // send empty byte to activate SPI_interrupt flag

		//sleep_cpu();
		
		//PORTD |= (1<<P_LCD_RES);
		
		//sleep_cpu();
		
		TCCR0B = 0;
		TIMSK0 = 0;
		
		cli();
//;----------------- remove this string with booster enabled
		PORTC |= (1<<P_boostFB);
		
		
/*			;==============================================
			;==============================================
			; sleep
			sleep
			; PULL LCD_reset to 1
			sbi PORTD, P_LCD_RES
			; out OCR0A, 10  - 10ms period uncoment 2 lines below if previous period is differ
			;ldi tmpreg, 10
			;out OCR0A, tmpreg
			; out TCNT0, zeroreg -  resets timer0 count
			;out TCNT0, zeroreg
			; sleep
			sleep
			; disable timer (or reconfigure for backlight)
			sts TCCR0B, zeroreg
			sts TIMSK0, zeroreg
			cli

			
			;----------------- remove this string with booster enabled
			sbi PORTC, P_boostFB ; enable internal pullup for test only

			;--------------------------------
			
			rcall backlight_on

			rcall enable_booster

			rcall enable_systick
			
			*/

			//;pcint 9,10,11 - buttons
			PCICR |= (1<<PCIE1);
			PCMSK1 |= (1<<PCINT9)|(1<<PCINT10)|(1<<PCINT11);
			
			/*
				.ifdef meas_pin_vcc
				sbi Vmeas_port, P_Vmeas
				.else
				cbi Vmeas_port, P_Vmeas
				.endif

				ldi tmpreg, (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(7<<MUX0)
				sts	ADMUX, tmpreg
				ldi tmpreg, (1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(2<<ADPS0)
				sts ADCSRA, tmpreg


				LCD_cmd LCD_init
				
				LCD_norm
				;LCD_inv

				LCD_XY 0,0
				LCD_dat LCD_clr
				
			*/
	
}