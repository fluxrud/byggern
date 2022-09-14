/*
 * slider.h
 *
 * Created: 14.09.2022 16:12:39
 *  Author: andrea7
 */ 


#ifndef SLIDER_H_
#define SLIDER_H_

#include "adc.h"

uint8_t get_slider_right_analog()
{
	struct adc_t a = read_adc();
	return a.c2;
}

uint8_t get_slider_left_analog()
{
	struct adc_t a = read_adc();
	return a.c3;
}



#endif /* SLIDER_H_ */