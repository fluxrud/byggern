/*
 * joystick_button.h
 *
 * Created: 28.09.2022 13:16:16
 *  Author: Anders
 */ 


#ifndef JOYSTICK_BUTTON_H_
#define JOYSTICK_BUTTON_H_
#include "avr/interrupt.h"
#include "avr/io.h"
#include "util/delay.h"
#include "game.h"
#include "oled_menu.h"


ISR(INT2_vect){
	
	init_oled(); // fix potential solenoid disturbance
	
	num_goals = 0;
	if (sel_item == 0 && game_has_started == 0){
		// start game
		game_has_started = 1;
	} else if (game_has_started == 1){
		game_has_started = 0;
	}
	
}

void init_interrupt(){
	clear_bit(GICR,5);		//clear the INT2 bit in GICR to avoid unintentional interrupts
	clear_bit(EMCUCR, 0);	//set ISC2 bit low to enable INT2 triggered by falling edge
	set_bit(GIFR, 5);		//clear interrupt flagg by writing a logic one to it
	set_bit(GICR, 5);		//set the INT2 bit in GICR to enable interrupts
	sei();					//set the I-bit in SREG (Global interrupt enable)
};
	
#endif /* JOYSTICK_BUTTON_H_ */