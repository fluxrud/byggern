/*
 * oled.h
 *
 * Created: 21.09.2022 11:07:01
 *  Author: andrea7
 */ 


#ifndef OLED_H_
#define OLED_H_

#include <stdio.h>
#include "fonts.h"


void init_oled();
void oled_write(unsigned char c);
void oled_fill_entire(unsigned char c);

// oled low level navigation
void oled_goto_page_direct(uint8_t p);
void oled_goto_col_direct(uint8_t col);

// oled buffer navigation
void oled_goto_page(uint8_t p);
void oled_goto_col(uint8_t col);

// printf util
int oled_write_char(char c, FILE* file);
void oled_write_string_on_line(char* s, uint8_t len, uint8_t line);

// draw util
void oled_draw_arrow();
void oled_draw_menu_arrow();

// render SRAM area to oled
void oled_render();

#endif /* OLED_H_ */