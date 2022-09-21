/*
 * joystick.h
 *
 * Created: 14.09.2022 13:02:35
 *  Author: andrea7
 */ 

/*

9*n*2 / f_adc = 30 us
n = number of channels converted

*/

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>

#include "adc.h"

enum joystick_direction {LEFT = 0, RIGHT, UP, DOWN, NEUTRAL};

struct joystick_analog 
{
	uint8_t x, y;
};

void joystick_auto_calibration();
struct joystick_analog get_joystick_analog()
{
	struct joystick_analog out;
	
	struct adc_t a = read_adc();
	
	out.x = a.c1;
	out.y = a.c0;
	
	//printf("[ %d, %d]", out.x, out.y);
	
	return out;
}

int get_joystick_direction()
{
	struct joystick_analog js_analog = get_joystick_analog();
	
	uint8_t thresh = 20;
	
	int ret = NEUTRAL;
	
	if (js_analog.x < thresh)					ret = LEFT;
	else if (js_analog.x > 255 - thresh)		ret = RIGHT;
	else 
	{
		if (js_analog.y < thresh)				ret = DOWN;
		else if (js_analog.y > 255 - thresh)	ret = UP;
	}
	
	return ret;
}

char* joystick_direction_to_string(int dir)
{	
	switch(dir)
	{
		case NEUTRAL:
			return "NEUTRAL";
		case DOWN:
			return "DOWN";
		case UP:
			return "UP";
		case RIGHT:
			return "RIGHT";
		case LEFT:
			return "LEFT";
		default:
			return NULL;
	}
}

#endif /* JOYSTICK_H_ */