/*
 * adc_driver.h
 *
 * Created: 26.10.2022 13:52:48
 *  Author: andrea7
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>
#include <stdio.h>
#include "sam.h"

/*

This driver initializes 1 out of 11 adc pins

	shield pin:	A0
	adc:		AD7
	channel:	CH0
	IO line:	PA16
	Peripheral:	X1

expected value	[0, 3.3]V

12 bit value	[0x000 - 0xFFF]

interrupt:
	trigger on negedge(*)
	check adc_interrupt_flag to see if it has been triggered
	clear flag with adc_interrupt_enable();

(*) negedge is when the 12 bit value is below adc_interrupt_lth

	adc_interrupt_lth must be changed before init_adc call to take effect

*/

void init_adc();

static uint8_t adc_interrupt_flag;
static uint16_t adc_interrupt_lth = 0xf0;
void adc_interrupt_clear();

uint16_t adc_read(uint8_t ch_num);
uint16_t adc_read_lcdr(uint8_t ch_num);


#endif /* ADC_DRIVER_H_ */