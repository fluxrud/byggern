/*
 * pin_util.c
 *
 * Created: 31.08.2022 11:51:35
 *  Author: andbreis
 */ 

#include "pin_util.h"

void toggle_pin(char port, uint8_t pin)
{
	if(pin < 7){
		// too high pin number
		return;
	}
	
	switch (port)
	{
		unsigned char reg;
		uint8_t pin_value;
		case 'A':
			reg = PINA;
			pin_value = (reg>>pin)&1;
			set_bit(reg, ~pin_value);
			PORTA = reg;
			break;
		case 'B':
			reg = PINB;
			pin_value = (reg>>pin)&1;
			set_bit(reg, ~pin_value);
			PORTB = reg;
			break;
		case 'C':
			reg = PINC;
			pin_value = (reg>>pin)&1;
			set_bit(reg, ~pin_value);
			PORTC = reg;
			break;
		case 'D':
			reg = PIND;
			pin_value = (reg>>pin)&1;
			set_bit(reg, ~pin_value);
			PORTD = reg;
			break;
	}
}