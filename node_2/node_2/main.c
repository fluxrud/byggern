/*
 * node_2.c
 *
 * Created: 12.10.2022 11:55:49
 * Author : andrea7
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "sam.h"
#include "pin_util.h"
#include "uart.h"
#include "can_controller.h"
#include "pwm_driver.h"

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
	if(ret == 1) printf("can init failed");
	init_pwm();
	
	//printf("begin");	
	//uart_putchar("c");
	
    while (1) 
    {
		pin_util_toggle('A', 19);
		pin_util_toggle('A', 20);
		/*
		struct can_message_t msg;
		msg.id = 0x02;
		msg.data_length = 4;
		msg.data[0] = 0x12;
		msg.data[1] = 0x34;
		msg.data[2] = 0x44;
		msg.data[3] = 0x44;
		int ret = can_send(&msg, 0);
		//if (ret != 0) printf(" can tx mb busy");
		*/
		delay(500);
    }
}
