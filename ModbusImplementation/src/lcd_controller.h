/*
 * lcd_controller.h
 *
 *  Created on: 05.10.2018
 *      Author: Darqwski
 */

#ifndef LCD_CONTROLLER_H_
#define LCD_CONTROLLER_H_
#include <stdint.h>
#include "stm32f10x.h"
extern void delay_ms(int time);

#define LCD_DC			GPIO_Pin_1
#define LCD_CE			GPIO_Pin_2
#define LCD_RST			GPIO_Pin_3

#define LCD_WIDTH				84
#define LCD_HEIGHT				48
void lcd_setup(void);
void lcd_clear(void);
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_pixel(int x, int y);
void lcd_draw_text(int row, int col, const char* text);
void lcd_draw_line(int x1, int y1, int x2, int y2);
void lcd_copy(void);
const unsigned char furnace_error [];
const unsigned char furnace_add_1 [];
const unsigned char furnace_add_2 [];
const unsigned char furnace_add_5 [];
const unsigned char furnace_on [];
const unsigned char furnace_going [];
#endif /* LCD_CONTROLLER_H_ */
