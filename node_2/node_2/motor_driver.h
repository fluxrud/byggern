/*
 * motor_driver.h
 *
 * Created: 02.11.2022 13:41:32
 *  Author: andrea7
 */ 


#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include <stdint.h>
#include "dac_driver.h"

static const uint8_t controller_kp = 8;
static const uint8_t controller_ki = 1;
static const uint8_t controller_T_inv = 500;
static uint8_t position = 0;
static uint8_t new_position = 0;

int e_sum;

uint8_t motor_read_enc();

void motor_set_direction(uint8_t d){
	if (d)	PIOD->PIO_SODR = 1 << 10;
	else	PIOD->PIO_CODR = 1 << 10;
}

// 8 bit
void motor_set_speed(uint8_t s){
	dac_write(s);
}

void motor_set_enable(uint8_t en){
	if (en)	PIOD->PIO_SODR = 1 << 9;
	else	PIOD->PIO_CODR = 1 << 9;
}

void motor_set_position(uint8_t pos){
	new_position = pos;
}

void init_motor(){
	// PIO enable C port
	PMC->PMC_PCR = (1 << 28) + (1 << 12) + 13;
	
	// set direction of MJ2
	PIOC->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
	//for (uint8_t i = 1; i < 9; i++) pin_util_set_dir('C', i, PIO_ENABLE);
	PIOC->PIO_PER |= ((1 << 8) - 1) << 1;
	PIOC->PIO_ODR = ((1 << 8) - 1) << 1;
	//PIOD->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 1;
	/*
	set direction of MJ1
	DIR		->	PIN32	->	PD10
	EN		->	PIN30	->	PD9
	SEL		->	PIN27	->	PD2
	NOT_RST	->	PIN26	->	PD1
	NOT_OE	->	PIN25	->	PD0
	*/
	pin_util_set_dir('D', 10, OUTPUT_ENABLE);	// DIR
	pin_util_set_dir('D', 9, OUTPUT_ENABLE);	// EN
	pin_util_set_dir('D', 2, OUTPUT_ENABLE);	// SEL
	pin_util_set_dir('D', 1, OUTPUT_ENABLE);	// NOT_RST
	pin_util_set_dir('D', 0, OUTPUT_ENABLE);	// NOT_OE
	
	// reset
	PIOD->PIO_CODR = (1 << 1);
	
	delay(1);
	
	// init values
	PIOD->PIO_SODR = (1 << 1) + 1;
	PIOD->PIO_CODR = (1 << 10) + (1 << 9) + (1 << 2);
}

void pid_reset(){
	e_sum = 0;
}

void motor_interrupt(){
	position = motor_read_enc();
	//printf("pos: %d, new_pos: %d\n\r", position, new_position);
	
	uint8_t dir;
	int e = position - new_position;
	if (e < 10) pid_reset();
	e_sum += e;
	signed int u = controller_kp * e + controller_ki * e_sum / controller_T_inv;
	//printf("signed u: %d\n\r", u);
	if (u > 0){
		dir = 1; // HØYRE
	}else{
		dir = 0; // VENSTRE
	}
	u = abs(u);
	if(u > 0xff) u = 0xff;
	
	if(u < 5){
		motor_set_enable(0);
		motor_set_speed(0);
		pid_reset();
		//return;
	}
	
	//printf("u: %d, dir: %d, e_sum: %d\n\r", u, dir, e_sum);
	//return;
	
	motor_set_speed(u);
	motor_set_direction(dir);
	motor_set_enable(1);
}

uint8_t motor_read_enc(){
	uint16_t ret;
	
	// set NOT_OE low
	PIOD->PIO_CODR = 1;
	PIOD->PIO_CODR = 1 << 2;
	
	delay(1);
	
	ret = ( ((PIOC->PIO_PDSR >> 1) & ((1<<8) - 1)) ) << 8;
	
	PIOD->PIO_SODR = 1 << 2;
	delay(1);
	
	ret += (PIOC->PIO_PDSR >> 1) & ((1<<8) - 1);
	
	PIOD->PIO_SODR = 1;
	
	return (uint8_t)(ret * 7 / 255);
}

#endif /* MOTOR_DRIVER_H_ */