/*
 * byggern_proj.c
 *
 * Created: 31.08.2022 10:09:11
 * Author : andbreis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
// #include <sam.h> // no such file?
#define F_CPU 4915200
#include "util/delay.h"

#include "pin_util.h"
#include "utilities.h"

void init_pin_directions()
{
	// set PORTB PIN0 direction as output
	DDRB = (1<<DDB0);
}

int main(void)
{
    init_pin_directions();
	
    while (1) 
    {
		_delay_ms(100);
		toggle_pin('B', 0);
    }
}