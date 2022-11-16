/*
 * timer_driver.h
 *
 * Created: 16.11.2022 09:54:39
 *  Author: andrea7
 */ 


#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "avr/interrupt.h"
#include "avr/io.h"
#include "can.h"
#include "joystick.h"
#include "adc.h"
#include <stdint.h>

#define TCNTn TCNT3

/*

timer interrupt vectors
/* Timer/Counter3 Capture Event 
#define TIMER3_CAPT_vect_num	6
#define TIMER3_CAPT_vect		_VECTOR(6)
#define SIG_INPUT_CAPTURE3		_VECTOR(6)

/* Timer/Counter3 Compare Match A 
#define TIMER3_COMPA_vect_num	7
#define TIMER3_COMPA_vect	    _VECTOR(7)
#define SIG_OUTPUT_COMPARE3A	_VECTOR(7)

/* Timer/Counter3 Compare Match B 
#define TIMER3_COMPB_vect_num	8
#define TIMER3_COMPB_vect		_VECTOR(8)
#define SIG_OUTPUT_COMPARE3B	_VECTOR(8)

/* Timer/Counter3 Overflow
#define TIMER3_OVF_vect_num		9
#define TIMER3_OVF_vect			_VECTOR(9)
#define SIG_OVERFLOW3			_VECTOR(9)
*/

ISR(TIMER3_COMPB_vect){
	
	//can_transmit(get_joystick_direction(), get_slider_right_analog(), get_slider_left_analog());
	
	// reset counter	
	TCNT3 = 0;
}

//ISR(TIMER3_CAPT_vect){}
//ISR(TIMER3_COMPA_vect){}
//ISR(TIMER3_COMPB_vect){}
//ISR(TIMER3_OVF_vect){}

void init_timer(){
	config_timer();
	ETIMSK = 0b00001000; // enable output compare interrupt for tc3 and oca
	sei();
}

void config_timer(){
	// timer interrupt 100ms / 10Hz
	// main clk 5 MHz / 1024 = 4.8kHz
	// 205 us
	// 205 * x = 0.1 => x = 488
	
	//cli();
	// timer control register a
	TCCR3A = 0b00100000; // clear OcnA on compare match
	
	// timer control register b
	TCCR3B = 0b00000101; // clock select clk/1024
	
	// output compare register
	OCR3B = 488;
}

// util from datasheet

void TIM16_WriteTCNTn( unsigned int i )
{
	unsigned char sreg;
	/* Save Global Interrupt Flag */
	//sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Set TCNTn to i */
	TCNTn = i;
	/* Restore Global Interrupt Flag */
	//SREG = sreg;
	sei();
}

unsigned int TIM16_ReadTCNTn( void )
{
	unsigned char sreg;
	unsigned int i;
	/* Save Global Interrupt Flag */
	//sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Read TCNTn into i */
	i = TCNTn;
	/* Restore Global Interrupt Flag */
	//SREG = sreg;
	sei();
	return i;
}

#endif /* TIMER_DRIVER_H_ */