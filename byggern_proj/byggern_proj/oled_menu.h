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
#include "game.h"

static char* oled_menu_items[] = {
	"Start Game", 
	"Settings", 
	"High scores"
};
static uint8_t n_items = 4;
static int top_item = 0;
static int sel_item = 0;
static int screen_size = 6;

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
	oled_fill_entire(0x00);
	for(uint8_t i = 0; i < screen_size; i++)
	{
		char* s;
		if (i + top_item < n_items - 1) s = oled_menu_items[i + top_item];
		else						s = "----";
		oled_write_string_on_line(s, strlen(s), i+1);
	}
	draw_selected_arrow(sel_item + 1);
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
	if (sel_item > screen_size - 1) {
		oled_menu_scroll_down();
		sel_item = screen_size - 1;
	}
	//oled_menu_display();
}

void oled_menu_sel_up(){
	sel_item += -1;
	if (sel_item < 1) {
		oled_menu_scroll_up();
		sel_item = 0;
	}
	//oled_menu_display();
}

void oled_menu_sel_right(){
	if(sel_item == 0){
		// start game
		//printf("SEL RIGHT\n\r");
	}
}

void oled_menu_sel_left(){
	//printf("left");
}

#endif /* OLED_MENU_H_ */