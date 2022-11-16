/*
 * timer_driver.h
 *
 * Created: 02.11.2022 14:40:20
 *  Author: andrea7
 */ 


#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "motor_driver.h"
#include "adc_driver.h"

void timer_config(uint8_t timer_num, uint16_t ms){
	Tc* timer_base;
	uint8_t timer_chan;
	
	if(timer_num / 3 == 0)		timer_base = TC0;
	else if(timer_num / 3 == 1) timer_base = TC1;
	else if(timer_num / 3 == 2) timer_base = TC2;
	else return;
	
	if(timer_num % 3 == 0)		timer_chan = 0;
	else if(timer_num % 3 == 1) timer_chan = 1;
	else if(timer_num % 3 == 2) timer_chan = 2;
	else return;
	
	timer_base->TC_WPMR = ('T' << 24) + ('I' << 16) + ('M' << 8) + 0;
	
	timer_base->TC_CHANNEL[timer_chan].TC_CCR = (1 << 2) + 1;		// enable and start clk
	timer_base->TC_CHANNEL[timer_chan].TC_CMR = (1 << 6) + 3;		// stop counter on reg_c cmp and set clk = mclk / 128
	timer_base->TC_CHANNEL[timer_chan].TC_RC = 600 * ms;			// period = 600 * 128 / 84MHz = 0.0009s
	timer_base->TC_CHANNEL[timer_chan].TC_IER = 1 << 4;				// counter rc cmp interrupt
	
	timer_base->TC_WPMR = ('T' << 24) + ('I' << 16) + ('M' << 8) + 1;
}

void init_timer(){
	// enable all timer peripheral clocks
	for (int i = 0; i < 9; i++) PMC->PMC_PCR = (1 << 28) + (1 << 12) + 27 + i;
	
	timer_config(0, 50);
	NVIC_EnableIRQ(ID_TC0);
	timer_config(1, 1000);
	NVIC_EnableIRQ(ID_TC1);
	timer_config(2, 2000);
	NVIC_EnableIRQ(ID_TC2);
	//timer_config(3, 1000);
	//timer_config(4, 1000);
	//timer_config(5, 1000);
	//timer_config(6, 1000);
	//timer_config(7, 1000);
	//timer_config(8, 1000);
}

void TC0_Handler(){
	//printf("interrupt: tc0\n\r");
	
	motor_interrupt();
	//printf("motor data: %d\n\r", motor_read_enc() * 7 / 255);
	
	// clear ch 0 and nvic
	TC0->TC_CHANNEL[0].TC_CCR = (1 << 2) + 1;
	uint32_t isr = TC0->TC_CHANNEL[0].TC_SR;
	NVIC_ClearPendingIRQ(ID_TC0);
}

void TC1_Handler(){
	//printf("interrupt: tc1\n\r");
	
	pin_util_toggle('A', 19);
	pin_util_toggle('A', 20);
	//pin_util_toggle('C', 22);
	
	// clear ch 0 and nvic
	//TC1->TC_CHANNEL[0].TC_CCR = (1 << 2) + 1;
	//uint32_t isr = TC1->TC_CHANNEL[0].TC_SR;
	
	// clear ch 1 and nvic
	TC0->TC_CHANNEL[1].TC_CCR = (1 << 2) + 1;
	uint32_t isr = TC0->TC_CHANNEL[1].TC_SR;
	
	NVIC_ClearPendingIRQ(ID_TC1);
}

void TC2_Handler(){
	printf("interrupt: tc2\n\r");
	
	printf("%d\n\r", adc_interrupt_flag);
	struct can_message_t msg;
	msg.id = 0x69;
	msg.data_length = 1;
	msg.data[0] = 0;
	int ret = can_send(&msg, 0);
	/*
	if (adc_interrupt_flag == 1){
		struct can_message_t msg;
		msg.id = 0x02;
		msg.data_length = 1;
		msg.data[0] = adc_interrupt_flag;
		int ret = can_send(&msg, 0);
		adc_interrupt_flag = 0;
	} else {
		struct can_message_t msg;
		msg.id = 0x02;
		msg.data_length = 1;
		msg.data[0] = adc_interrupt_flag;
		int ret = can_send(&msg, 0);
	}*/
	//if (ret != 0) printf(" can tx mb busy");
	
	//servo_pos_us = ((servo_pos_us - 900 + 20) % 1200) + 900;
	//printf("servopos: %d\n\r", servo_pos_us);
	//pwm_set_dc(5, servo_pos_us);
	
	//pin_util_toggle('A', 19);
	//pin_util_toggle('A', 20);
	
	// clear ch 0 and nvic
	TC0->TC_CHANNEL[2].TC_CCR = (1 << 2) + 1;
	uint32_t isr = TC0->TC_CHANNEL[2].TC_SR;
	NVIC_ClearPendingIRQ(ID_TC2);
}

#endif /* TIMER_DRIVER_H_ */