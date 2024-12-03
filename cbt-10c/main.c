/*
 * cbt-10c.c
 *
 * Created: 25.11.2024 0:45:51
 * Author : inflamer
 */ 


#include "main.h"
#include "avr\common.h"



//#include "interrupts.s"



uint8_t TXCountMem;
uint8_t TXRowCountMem;


uint8_t my_value;

register uint8_t bitstore asm("r4");


uint8_t my_C_function(uint8_t var){
	var += 20;
	_NOP();
	
	//asm volatile("nop");
	return var;
}  

uint8_t my_value;

void LCD_tx(const volatile uint8_t *data){
	// need to remove size variable
	volatile uint8_t out;
	volatile uint8_t local,xcnt,x,y,zeroes = 0;
	


	asm volatile
		(
		"movw r30,r24" "\n\t"
		"lpm %0, Z+" "\n\t"
		"lpm %1, Z+" "\n\t"
		: "=r" (x), "=r" (y):);
		if (!y){y++;};
		xcnt = x;
	volatile uint16_t size = x*y;
		y--;
		
	do{

		
	//local = pgm_read_byte(data[size]);
	asm volatile("lpm %0, Z+" "\n\t" : "=r" (local) :);
	if (!local)
	{

		asm volatile("lpm %0, Z+" "\n\t" : "=r" (zeroes):);
		do {
			
			
			zeroes--; size--;xcnt--;
			if (!xcnt){if(y){y--;xcnt = x;}}
			// check if tx complete
			//do{_NOP();}
			while(!(SPSR & (1<<SPIF)));
			SPDR = local;
			out = local;
		} while(zeroes);
	}
	else{
		size--;xcnt--;
		if (!xcnt){if(y){y--;xcnt = x;}}
		// check if tx complete
		//do{_NOP();}
		while(!(SPSR & (1<<SPIF)));
		SPDR = local;
		out = local;
	}
	
	} while(xcnt); //size
	
	//return local;
};



int main()
{

#define bittest (1<<0);

	init();
	
	LCD_tx(LCD_init);

	LCD_tx(MINI_CIFRA_SP);

	//SPSR |= (1<<SPIF);
	
	LCD_tx(ne_CIFRA);
	
	LCD_tx(MINI_CIFRA_3);
	
	bitstore |= bittest; 

	SP = RAMEND;
	
	bitstore &= ~bittest;
	
	





	while(1);
	//return 0;
	// never run section for tests only

	
}

void init(void){

		PRR = (1<<PRTWI)|(1<<PRUSART0);
		SMCR = (1<<SE);//|(2<<SM0); //; idle sm=000
		DDRD = (1<<P_LCD_RES)|(1<<P_bDiode)|(1<<P_bCap)|(1<<P_bTrans)|(1<<PD6);
		
		//;=SPI init
		DDR_SPI = (1<<P_SS)|(1<<P_SCK)|(1<<P_MOSI);
		DDR_SPI &= ~(1<<P_MISO);
		
		Vmeas_DDR |= (1<<P_Vmeas);

		Vmeas_port |= P_Vmeas;
		
		set_sleep_mode(SLEEP_MODE_IDLE);
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
		
		TIFR0 = 0;
		TIMSK0 |= (1<<OCIE0A);
		OCR0A = 80;
		TCCR0B |= (5<<CS00);
		TCNT0 = 0;
		
		PORTD &= ~(1<<P_LCD_RES);
		
/*=======================================================
Do something usefull when LCD reset pulled low and sleep
=========================================================*/

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
		SPDR = 0;

		//sleep_cpu();
		
		PORTD |= (1<<P_LCD_RES);
		
		//sleep_cpu();
		
		TCCR0B = 0;
		TIMSK0 = 0;
		
		cli();
		
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


void drafts(void){
	
	
	
		volatile uint8_t test;
		
		//my_value = sizeof(MINI_CIFRA_SP);
		test = pgm_read_byte(&MINI_CIFRA_SP[0]);
		
		for (uint8_t i=0;i<sizeof(MINI_CIFRA_SP);i++)
		{
			my_value = pgm_read_byte(&MINI_CIFRA_SP[i]);
			PORTB = my_value;
		}
		
		PORTB = test;
		DDRB = 0xff;
		asmfunc_calledfrom_c(3);
		
		volatile uint8_t test_at;
		
		ATOMIC_BLOCK(ATOMIC_FORCEON){
			test_at = 10;
		};
		
		
		
		_NOP();

		
		
			//const volatile uint8_t *addr = &MINI_CIFRA_0[0];
			//volatile uint8_t res;
			
			asm volatile(
			"ldi ZL,lo8(%[addr])\n\t"
			"ldi ZH,hi8(%[addr])\n\t"
			//"lpm %0,Z+"
			://"=r" (res)
			:[addr] "i" (&MINI_CIFRA_0[0]));
			//:[addr] "i" (addr));
			
			set_Z_pointer(&MINI_CIFRA_0[0]);
			SPI_start();


			uint16_t addr16 = (uint16_t)(&MINI_CIFRA_SP[0]);
			volatile uint8_t result;
			asm volatile
			(
			"lpm %0, Z" "\n\t"
			: "=r" (result)
			: "z" (addr16)
			);
		
		result = 0;
	
	
}


void LCD_data(void){
	
	};
void LCD_cmd(void){
	
	};
	
