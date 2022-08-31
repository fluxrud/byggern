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
#include <stdio.h>

#include "pin_util.h"
#include "utilities.h"
#include "uart_driver.h"

void init_pin_directions()
{
	// set PORTB PIN0 direction as output
	DDRB = (1<<DDB0);
}

int main(void)
{
    init_pin_directions();
	USART_Init(MYUBRR);
	fdevopen(USART_Transmit, USART_Receive);
		
    while (1) 
    {
		_delay_ms(1000);
		//USART_Transmit('a');
		//c = USART_Receive();
		//if(c == 'w')
		//{
		//	toggle_pin('B', 0);
		//}
		//test_usart();
		printf("hello world\n\n\r");
		printf("This is a value: %d\n\r", 13);
		printf("This is a float: %f\n\r", 0.314);
    }
}