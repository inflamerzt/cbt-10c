/*
 * init.c
 *
 * Created: 06.12.2024 2:19:22
 *  Author: inflamer
 */ 

#include "init.h"


extern volatile const uint8_t* smDig[10];
extern volatile const uint8_t* bigDig[10];

void init(void){
	
		//disable unused periph
		PRR = (1<<PRTWI)|(1<<PRUSART0);
		//SMCR = (1<<SE);//|(2<<SM0); //; idle sm=000
		DDRD = (1<<P_LCD_RES)|(1<<P_bDiode)|(1<<P_bCap)|(1<<P_bTrans)|(1<<PD6);
		PORTD &= ~(1<<P_LCD_RES);
		//DDRC =  (1<<P_bTrans);
		

		
		//;=SPI init
		DDR_SPI = (1<<P_SS)|(1<<P_SCK)|(1<<P_MOSI);
		DDR_SPI &= ~(1<<P_MISO);
		
		Vmeas_DDR |= (1<<P_Vmeas);

		Vmeas_port |= P_Vmeas;
		
		DDR_buzz |= (1<<P_buzz);
		
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
		TIFR2 = 0;
		TIMSK2 |= (1<<OCIE2B);
		OCR2B = 80;
		TCCR2B |= (5<<CS00);
		TCNT2 = 0;
		

		
/*=======================================================
Do something usefull when LCD reset pulled low and sleep
=========================================================*/

	smDig[0] = MINI_CIFRA_0;
	smDig[1] = MINI_CIFRA_1;
	smDig[2] = MINI_CIFRA_2;
	smDig[3] = MINI_CIFRA_3;
	smDig[4] = MINI_CIFRA_4;
	smDig[5] = MINI_CIFRA_5;
	smDig[6] = MINI_CIFRA_6;
	smDig[7] = MINI_CIFRA_7;
	smDig[8] = MINI_CIFRA_8;
	smDig[9] = MINI_CIFRA_9;
	
		bigDig[0] = CIFRA_0;
		bigDig[1] = CIFRA_1;
		bigDig[2] = CIFRA_2;
		bigDig[3] = CIFRA_3;
		bigDig[4] = CIFRA_4;
		bigDig[5] = CIFRA_5;
		bigDig[6] = CIFRA_6;
		bigDig[7] = CIFRA_7;
		bigDig[8] = CIFRA_8;
		bigDig[9] = CIFRA_9;



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

		sleep_cpu();
		
		PORTD |= (1<<P_LCD_RES);
/*=======================================================
Do something usefull LCD delay after reset and sleep
=========================================================*/		
		
		sleep_cpu();
		
		TCCR2B = 0;
		TIMSK2 = 0;
		
		
		cli();
//;----------------- remove this string with booster enabled
		//PORTC |= (1<<P_boostFB);
		
		
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
			
			//EICRA |= (3<<ISC10);
			//EIMSK |= (1<<INT1);
			
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
			
			// init booster
			OCR0A = 155; //156
			OCR0B = 155-16;
			TIMSK0 = (1<<OCIE0A)|(1<<OCIE0B)|(1<<TOIE0);
			TCCR0A = (1<<WGM01);
			
			//init systick
			//DDRD |= (1<<PD3);
			OCR2A = 250-1; //156
			OCR2B = 100;
			TCCR2B =(3<<CS00);
			TIMSK2 = (1<<OCIE2A)|(1<<OCIE2B);
			TCCR2A = (2<<WGM20);//(2<<COM0B0)|;
			
			/*
				backlight_on:
				;===== enable timer0 and configure pwm LED_backlight
				ldi tmpreg, (1<<CS00)
				out TCCR0B,tmpreg
				
				sbi DDRD, PD5 ; set compare output pin as out

				ldi tmpreg,0xEF
				;ldi tmpreg, 0x00
				out OCR0B,tmpreg

				ldi tmpreg, (2<<COM0B0)|(3<<WGM00) ; (3<<COM0B0) if out must be inverted
				out TCCR0A, tmpreg
				
				ret
			
			*/
			
			
			
			
	
}

void start_count_cps(void){
	TCNT1 = 0;
	//TCCR1A = 0;
	TCCR1B = (7<<CS00) ;
	//TCCR1C = 0;
	TIMSK1 = (1<<TOIE1);
	};

void stop_count_cps(void){
	TCCR1B = 0;
	TIMSK1 = 0;	
	};
	
	
void start_booster(void){
	
		TCCR0B = (4<<CS00);
	};
	
	
	
void stop_booster(void){
			TCCR0B = 0;
			TCNT0 = 0;
	};