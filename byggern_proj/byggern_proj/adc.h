/*
 * adc.h
 *
 * Created: 14.09.2022 16:02:26
 *  Author: andrea7
 */ 


#ifndef ADC_H_
#define ADC_H_

struct adc_t
{
	uint8_t c0, c1, c2, c3;
};

struct adc_t read_adc()
{
	// base address wr trigger
	volatile char *ext_adc = (char *) 0x1400;
	ext_adc[0] = 0x00;

	// wait atleast 30 us
	_delay_us(60);

	struct adc_t out;
	// read	?delay between reads?
	out.c0 = ext_adc[0]; // y joystick
	out.c1 = ext_adc[1]; // x joystick
	out.c2 = ext_adc[2]; // r slider
	out.c3 = ext_adc[3]; // l slider
	
	return out;
}

void init_adc()
{
	volatile char *ext_adc = (char *) 0x1400;
	for(int i = 0; i < 8; i++) ext_adc[0] = 0b00010000 + i;
}

#endif /* ADC_H_ */