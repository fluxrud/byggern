/*
 * oled.c
 *
 * Created: 21.09.2022 15:25:01
 *  Author: andrea7
 */ 
#include "oled.h"

static volatile char* oled_c_base = (char*)0x1000;
static volatile char* oled_d_base = (char*)0x1200;

static volatile char* oled_sram_base = (char*)0x1C00;
static uint8_t current_page = 0;
static uint16_t current_adr = 0;

static unsigned char init_data[22] = {0xae, 0xa1, 0xda, 0x12, 0xc8, 0xa8, 0x3f, 0xd5, 0x80, 0x81, 0x50, 0xd9, 0x21, 0x20, 0x02, 0xdb, 0x30, 0xad, 0x00, 0xa4, 0xa6, 0xaf};

void init_oled()
{
	for(int i = 0; i < 22; i++) {
		oled_c_base[0] = init_data[i];
		//printf("\n\r%x", init_data[i]);
	}
	*oled_c_base = 0x20;
	*oled_c_base = 0x02;
}

void oled_fill_entire(unsigned char c)
{	
	for(int i = 0; i < 1024; i++) {
		//if (i % 128 == 0) *oled_c_base = 0xb0 + i / 128;
		oled_sram_base[i] = c;
	}
	current_adr = 0;
}

void oled_goto_page(uint8_t p)
{
	if (p > 7) {
		return;
	}
	*oled_c_base = 0xb0 + p;
}

void oled_goto_col(uint8_t col)
{
	*oled_c_base = 0x21;
	*oled_c_base = 0x00 + col;
	*oled_c_base = 0xFF;
}

int oled_write_char(char c, FILE* file)
{
	if (c == '\n'){
		//current_page++;
		//current_page %= 8;
		//*oled_c_base = 0xb0 + current_page;
		current_adr += 128;
		return 0;
	} else if (c == '\r') {
		//*oled_c_base = 0x21;
		//*oled_c_base = 0x00;
		//*oled_c_base = 0xFF;
		current_adr = current_adr - (current_adr % 128);
	}
	int size = 4;
	if (c < 32 || c > 127) return 1;
	switch(size){
		case 4:
			for(int i = 0; i < size; i++){
				oled_sram_base[current_adr] = pgm_read_byte(&(font4[c - 32][i]));
				current_adr++;
				current_adr %= 1024;
			}
			break;
		case 5:
			for(int i = 0; i < size; i++){
				*oled_d_base = pgm_read_byte(&(font5[c - 32][i]));
			}
			break;
		case 8:
			for(int i = 0; i < size; i++){
				*oled_d_base = pgm_read_byte(&(font8[c - 32][i]));
			}
			break;
		default: 
			return 1;
	}
	return 0;
}

void oled_write_string_on_line(char* s, uint8_t len, uint8_t line)
{
	if (line > 7) return;
	current_adr = 128 * line;
	for (int i = 0; i < 128; i++)
	{
		if(i < len) oled_write_char(s[i], NULL);
		else		oled_write_char(' ', NULL);
	}
}

void oled_draw_arrow()
{
	*oled_d_base = 0b00011000;
	*oled_d_base = 0b00111100;
	*oled_d_base = 0b01111110;
	*oled_d_base = 0b11011011;
	*oled_d_base = 0b00011000;
	*oled_d_base = 0b00011000;
}

void oled_draw_menu_arrow()
{
	
}

void oled_render()
{
	/*
	SRAM 0x1800 - 0x1FFF
	*/
	//oled_fill_entire(0xff);
	current_adr = 0;
	oled_goto_page(0);
	oled_goto_col(0);
	for(int i = 0; i < 1024; i++){
		*oled_d_base = oled_sram_base[i];
		if (i % 128 == 0) oled_goto_page(i / 128);
	}
}