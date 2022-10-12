/*
 * pin_util.c
 *
 * Created: 31.08.2022 11:51:35
 *  Author: andbreis
 */ 

#include "pin_util.h"

void toggle_pin(char port, uint8_t pin)
{
	if(pin > 7){
		// too high pin number
		return;
	}
	
	switch (port)
	{
		unsigned char reg;
		uint8_t pin_value;
		case 'A':
			reg = PINA;
			// 0000x000
			// 0000000x & 00000001
			pin_value = (reg>>pin)&1;
			// reg = xxxxxxxx
			// set_bit(reg, 2)
			// reg = xxxxx1xx
			if (pin_value){
				clear_bit(reg, pin);
			} else {
				set_bit(reg, pin);
			}
			PORTA = reg;
			break;
		case 'B':
			reg = PINB;
			pin_value = (reg>>pin)&1;
			if (pin_value){
				clear_bit(reg, pin);
			} else {
				set_bit(reg, pin);
			}
			PORTB = reg;
			break;
		case 'C':
			reg = PINC;
			pin_value = (reg>>pin)&1;
			if (pin_value){
				clear_bit(reg, pin);
			} else {
				set_bit(reg, pin);
			}
			PORTC = reg;
			break;
		case 'D':
			reg = PIND;
			pin_value = (reg>>pin)&1;
			if (pin_value){
				clear_bit(reg, pin);
			} else {
				set_bit(reg, pin);
			}
			PORTD = reg;
			break;
	}
}