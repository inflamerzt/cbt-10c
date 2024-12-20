#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"

void SPI_tx(uint8_t data);
void LCD_send(const volatile uint8_t *data,uint8_t dc);
void LCD_xy(uint8_t X,uint8_t Y); // maybe make as define or macro
void LCD_clr(void);
void LCD_rclr(uint8_t xs,uint8_t ys);
void LCD_sp(uint8_t i);
void LCD_element(Element data);


#endif /* LCD_H_ */