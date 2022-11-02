/*
 * timer_driver.h
 *
 * Created: 02.11.2022 14:40:20
 *  Author: andrea7
 */ 


#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include "motor_driver.h"

void timer_config(){
	TC0->TC_WPMR = ('T' << 24) + ('I' << 16) + ('M' << 8) + 0;
	
	TC0->TC_CHANNEL[0].TC_CCR = (1 << 2) + 1;	// enable and start clk
	TC0->TC_CHANNEL[0].TC_CMR = (1 << 6) + 3;		// stop counter on reg_c cmp and set clk = mclk / 128
	TC0->TC_CHANNEL[0].TC_RC = 60000 * 10;			// period = 60000 * 128 / 84MHz = 0.09s
	TC0->TC_CHANNEL[0].TC_IER = 1 << 4;				// counter rc cmp interrupt
	
	TC0->TC_WPMR = ('T' << 24) + ('I' << 16) + ('M' << 8) + 1;
}

void init_timer(){
	// enabable peripheral clock on timer channel 0
	PMC->PMC_PCR = (1 << 28) + (1 << 12) + 27;
	
	timer_config();
	
	NVIC_EnableIRQ(ID_TC0);
}

void TC0_Handler(){
	//printf("interrupt: tc0\n\r");
	
	motor_interrupt();
	
	TC0->TC_CHANNEL[0].TC_CCR = (1 << 2) + 1;
	uint32_t isr = TC0->TC_CHANNEL[0].TC_SR; // read status, cleared by read
	NVIC_ClearPendingIRQ(ID_TC0);
}

#endif /* TIMER_DRIVER_H_ */