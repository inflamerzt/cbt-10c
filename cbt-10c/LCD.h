#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"

void SPI_tx(uint8_t data);
void LCD_puts(const volatile uint8_t *data,uint8_t dc);
void LCD_xy(uint8_t X,uint8_t Y);


#endif /* LCD_H_ */