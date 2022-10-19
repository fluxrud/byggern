/*
 * pin_util.h
 *
 * Created: 12.10.2022 13:32:01
 *  Author: andrea7
 */ 


#ifndef PIN_UTIL_H_
#define PIN_UTIL_H_

#include <stdint.h>

void pin_util_set_dir(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			PIOA->PIO_OER |= 1u << pin;
			//PIOA->PIO_PER |= 1u << pin;
			break;
		}
		default: break;
	}
}

void pin_util_toggle(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			uint32_t port_data = PIOA->PIO_ODSR;
			if (port_data & (1u << pin))	PIOA->PIO_CODR = (1u << pin);
			else							PIOA->PIO_SODR = port_data | (1u << pin);
			//PIOA->PIO_SODR = port_data ^ (1u << pin);
			//PIOA->PIO_SODR = port_data & ~(1 << pin);
			break;
		}
		default: break;
	}
}

void pin_util_set(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			uint16_t port_data = PIOA->PIO_ODSR;
			PIOA->PIO_SODR = port_data | (1 << pin);
			break;
		}
		default: break;
	}
}

void pin_util_clear(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			PIOA->PIO_CODR = (1 << pin);
			break;
		}
		default: break;
	}
}



#endif /* PIN_UTIL_H_ */