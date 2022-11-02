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

static const uint8_t controller_kp = 10;
static const uint8_t controller_ki = 10;
static const uint8_t controller_T_inv = 50;
static uint8_t position = 0;
static uint8_t new_position = 0;

uint32_t e_sum;

void motor_set_direction(uint8_t d){
	if (d)	PIOD->PIO_SODR = 1 << 10;
	else	PIOD->PIO_CODR = 1 << 10;
}

// 8 bit
void motor_set_speed(uint16_t s){
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
	// output pins
	pin_util_set_dir('D', 9, OUTPUT_ENABLE);
	pin_util_set_dir('D', 10, OUTPUT_ENABLE);
	motor_set_speed(0);
	motor_set_direction(0);
	motor_set_enable(0);
}

void pid_reset(){
	e_sum = 0;
}

void motor_interrupt(){
	printf("pos: %d, %d", position, new_position);
	
	uint8_t dir;
	int e = position - new_position;
	e_sum += e;
	int u = controller_kp * e + controller_ki * e_sum / controller_T_inv;
	if (u < 0){
		dir = 0;
	}else{
		dir = 1;
		u = -u;
	}
	if(u > 0xff) u = 0xff;
	
	if(u < 0x05){
		motor_set_enable(0);
		motor_set_speed(0);
		pid_reset();
		return;
	}
	
	motor_set_speed(u);
	motor_set_direction(dir);
	motor_set_enable(1);
}

#endif /* MOTOR_DRIVER_H_ */