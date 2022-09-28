/*
 * oled_menu.h
 *
 * Created: 21.09.2022 16:29:43
 *  Author: andrea7
 */ 


#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include "oled.h"
#include "oled_shapes.h"
#include <string.h>

static char* oled_menu_items[] = {
	"Menu item 1", 
	"Menu item 2", 
	"Menu item 3", 
	"Menu item 4", 
	"Menu item 5", 
	"Menu item 6", 
	"Menu item 7",
	"Menu item 8",
	"Menu item 9",
	"Menu item 10"
};
static uint8_t n_items = 11;
static int top_item = 0;
static int sel_item = 0;

void draw_selected_arrow(uint8_t p)
{
	
	oled_goto_page(p);
	oled_goto_col(128 - menu_arrow_len);
	for(int i = 0; i < menu_arrow_len; i++){
		oled_write(menu_arrow[i]);
	}
}

void oled_menu_display()
{
	for(uint8_t i = 0; i < 8; i++)
	{
		char* s;
		if (i + top_item < n_items - 1) s = oled_menu_items[i + top_item];
		else						s = "----";
		oled_write_string_on_line(s, strlen(s), i);
	}
	draw_selected_arrow(sel_item);
}

void oled_menu_scroll_up(){
	top_item += -1;
	if (top_item < 0) top_item = 0;
}

void oled_menu_scroll_down(){
	top_item += 1;
	if (top_item > n_items - 3) top_item = n_items - 3;
}

void oled_menu_sel_down(){
	sel_item += 1;
	if (sel_item > 7) {
		oled_menu_scroll_down();
		sel_item = 7;
	}
	oled_menu_display();
}

void oled_menu_sel_up(){
	sel_item += -1;
	if (sel_item < 0) {
		oled_menu_scroll_up();
		sel_item = 0;
	}
	oled_menu_display();
}

#endif /* OLED_MENU_H_ */