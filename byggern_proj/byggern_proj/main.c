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

void init_ext_mem()
{
	// enable XMEM
	set_bit(MCUCR, SRE);
	// release 4 msb to jtag
	set_bit(SFIOR, XMM2);
	clear_bit(SFIOR, XMM1);
	clear_bit(SFIOR, XMM0);
}

void SRAM_test(void);

int main(void)
{
    init_pin_directions();
	init_ext_mem();
	USART_Init(MYUBRR);
	// todo: change transmit and receive to int return, error handling?
	fdevopen(USART_Transmit, USART_Receive);
	
	SRAM_test();
    while (1) 
    {
		
		_delay_ms(100);
		toggle_pin('B', 0);
		//volatile char *reg = (char *) 0x1FFF;
		//*reg = 0x00;
		//printf("Base content: 0x%x\n\r", *reg);
		//USART_Transmit('a');s
		//c = USART_Receive();
		//if(c == 'w')
		//{
		//	toggle_pin('B', 0);
		//}
		//test_usart();
		//printf("hello world\n\n\r");
		//printf("This is a value: %d\n\r", 13);
		//printf("This is a float: %f\n\r", 0.314);
    }
}