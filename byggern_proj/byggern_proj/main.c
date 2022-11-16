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
#include "can.h"

#include "joystick_button.h"

#include "timer_driver.h"

#include "game.h"

void init_pin_directions()
{
	// set PORTB PIN0 direction as output
	set_bit(DDRB, DDB0);
	// mcp transistor
	set_bit(DDRB, DDB1);
	char reg = PINB;
	set_bit(reg, DDB1);
	PORTB = reg;
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
	
	//init_can();
	//init_interrupt();
	
	
	_delay_us(2); // the MCP takes a maximum of 2 us to be ready for a reset
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
	
	//_delay_ms(10000);
	toggle_pin('B', 1);
	//_delay_ms(2000);
	//toggle_pin('B', 1);
	
	oled_write_string_on_line("string test", strlen("string test"), 0);
	
	//mcp2515_write(0x0F, 0xF0);
	//mcp2515_bit_mod(0x0F, 0xFF, 0xFF);
	
	//mcp2515_reset();
	//_delay_ms(1000);
	
	//init_timer();
	init_can();
	
	//can_set_config_mode(MODE_LOOPBACK);
	
	uint8_t slider_value = get_slider_right_analog();
	
    while (1) 
    {
		/* TEST */
		//printf("\n\r-- LOOP --\n\r");
		//mcp2515_reset();
		/*
		struct can_msg_t msg;
		msg.id = 0x01;
		msg.data_l = 4;
		msg.data = (uint8_t*)malloc(msg.data_l);
		msg.data[0] = 0x12;
		msg.data[1] = 0x34;
		msg.data[2] = 0x56;
		can_transmit_tx_buf0(msg);
		//printf("sent can frame\n\r");
		//display_can_frame(msg);
		free(msg.data);
		*/
		//printf("\n%2x", mcp2515_read(0x0e));			// mpc read CANSTAT, should be 0x80, configuration mode
		//oled_write_char((unsigned char)'a', 8);
		//oled_fill_entire();
		// display_adc_info(); // using printf
		
		/* OLED */
		if (game_has_started == 0){
			oled_menu_display();
		} else {
			display_game();
		}
		
		
		/* JOYSTICK TX */
		switch (get_joystick_direction())
		{
			case DOWN:
				oled_menu_sel_down();
				//can_transmit(0x1f, 1);
				break;
			case UP:
				oled_menu_sel_up();
				//can_transmit(0x1f, 2);
				break;
			case RIGHT:
				oled_menu_sel_right();
				init_oled();
				//can_transmit(0x1f, 3);
				break;
			case LEFT:
				//can_transmit(0x1f, 4);
				oled_menu_sel_left();
				init_oled();
				break;
			default:
				break;
		}
		
		can_transmit((uint8_t)get_joystick_direction(), (uint8_t)get_slider_right_analog(), (uint8_t)get_slider_left_analog());
		
		/* RIGHT SLIDER TX */
		uint8_t n_slider_value = get_slider_right_analog();
		if (abs(slider_value - n_slider_value) > 5){
			//can_transmit(0x00, n_slider_value, 0x00);
		}
		slider_value = n_slider_value;
		
		/* LEFT SLIDER 
		uint8_t l_slider_val = get_slider_left_analog();
		if (l_slider_val > 100){
			can_transmit(0x3f, l_slider_val);
		}
		slider_value = n_slider_value;
		*/
		/* OLED RENDER */
		oled_render();
		
		// read rx buffer
		
		//print_game();
		//display_can_frame(msg);
		
		/* LOOP PERIOD AND LED */
		toggle_pin('B', 0);
		_delay_ms(100);
		//printf("canstat: %x\n", mcp2515_read(0x0e));
		//printf("eflg: %x\n", mcp2515_read(0x2d));
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