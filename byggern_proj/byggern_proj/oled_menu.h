/*
 * oled_menu.h
 *
 * Created: 21.09.2022 16:29:43
 *  Author: andrea7
 */ 


#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include "oled.h"
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
static uint8_t current_item = 4;

void oled_menu_display()
{
	for(int i = 0; i < 8; i++)
	{
		if (i + current_item < n_items){
			char* s = oled_menu_items[i + current_item];
			oled_write_string_on_line(s, strlen(s), i);
		}
		else {
			char* s = "----";
			oled_write_string_on_line(s, strlen(s), i);
		}
	}
}


#endif /* OLED_MENU_H_ */