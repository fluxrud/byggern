/*
 * node_2.c
 *
 * Created: 12.10.2022 11:55:49
 * Author : andrea7
 */ 

//#include <stdio.h>
//#include <stdlib.h>
#include <stdint.h>

#include "sam.h"
#include "pin_util.h"
#include "uart.h"
#include "can_controller.h"

#define F_CPU 4000

void delay(uint32_t ms){
	for(uint32_t i=0; i<F_CPU * ms / 4; i++){__NOP();}
}

void init()
{
	pin_util_set_dir('A', 19);
	pin_util_set_dir('A', 20);
}

int main(void)
{
	SystemInit();
	init();
	configure_uart();
	uint8_t ret = can_init_def_tx_rx_mb();
	if(ret == 1) printf("can init failed\n\r");
	
	//printf("begin\n");	
	//uart_putchar("c");
	
    while (1) 
    {
		printf("123456789");
		pin_util_toggle('A', 19);
		pin_util_toggle('A', 20);
		/*
		struct can_message_t msg;
		msg.id = 0x01;
		msg.data_length = 4;
		msg.data[0] = 0x12;
		msg.data[1] = 0x34;
		msg.data[2] = 0x32;
		msg.data[3] = 0x10;
		
		can_send(&msg, 0);
		*/
		delay(1000);
    }
}
