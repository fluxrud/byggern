/*
 * byggern_proj.c
 *
 * Created: 31.08.2022 10:09:11
 * Author : andbreis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
// #include <sam.h> // no such file?
#define F_CPU 4915200
#include "util/delay.h"
#include <stdio.h>

#include "pin_util.h"
#include "utilities.h"
#include "uart_driver.h"
#include "pwm_gen.h"
#include "adc.h"
#include "joystick.h"
#include "slider.h"
#include "oled.h"

#include "fonts.h"
#include "oled_menu.h"

#include "mcp2515.h"

#include "joystick_button.h"

void init_pin_directions()
{
	// set PORTB PIN0 direction as output
	set_bit(DDRB, DDB0);
}

void init_ext_mem()
{
	// enable XMEM
	set_bit(MCUCR, SRE);
	// release 4 msb to jtag
	set_bit(SFIOR, XMM2);
	clear_bit(SFIOR, XMM1);
	clear_bit(SFIOR, XMM0);
}

void SRAM_test(void);
void display_adc_info();

void test_static_address();

int main(void)
{
    init_pin_directions();
	init_ext_mem();
	init_pwm();
	USART_Init(MYUBRR);
	init_adc();
	init_oled();
	
	init_interrupt();
	
	SPI_MasterInit();
	mcp2515_reset();
	
	// todo: change transmit and receive to int return, error handling?
	fdevopen(USART_Transmit, USART_Receive);
	// todo: FDEV_SETUP_STREAM to utilize multi output printf
	//fdevopen(oled_write_char, USART_Receive);
	
	// test functions
	//test_static_address();
	//SRAM_test();
	
	oled_fill_entire(0x0F);
	// oled_goto_page(0);
	//printf("\n\r arrow = ");
	//oled_draw_arrow();
	
	//oled_write_string_on_line("string test", strlen("string test"), 0);
	
	//mcp2515_write(0x0F, 0xF0);
	//mcp2515_bit_mod(0x0F, 0xFF, 0xFF);
	
    while (1) 
    {
		printf("\n\r%2x", mcp2515_read(0x0e));
		oled_render();
		//printf("\n\r-- LOOP --\n\r");
		toggle_pin('B', 0);
		//oled_write_char((unsigned char)'a', 8);
		//oled_fill_entire();
		display_adc_info();
		_delay_ms(100);
		oled_menu_display();
		if (get_joystick_direction() == DOWN)	oled_menu_sel_down();
		if (get_joystick_direction() == UP)		oled_menu_sel_up();
    }
}



/* test functions */

void display_adc_info()
{
	printf("Joystick: %s        \n\r", joystick_direction_to_string(get_joystick_direction()));
	printf("Slider right: %d    \n\r", get_slider_right_analog());
	printf("Slider left: %d     \n\r", get_slider_left_analog());
}

void test_static_address(){
	
	printf("Static address test\n\r");
	
	// set pin direction (a 0-3)
	set_bit(DDRC, DDC0);
	set_bit(DDRC, DDC1);
	set_bit(DDRC, DDC2);
	set_bit(DDRC, DDC3);
		
	uint8_t reg = PINC;
	clear_bit(reg, PINC0);
	clear_bit(reg, PINC1);
	clear_bit(reg, PINC2);
	clear_bit(reg, PINC3);
	PORTC = reg;
}