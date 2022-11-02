/*
 * dac_driver.h
 *
 * Created: 02.11.2022 11:30:43
 *  Author: andrea7
 */ 


#ifndef DAC_DRIVER_H_
#define DAC_DRIVER_H_

#include "pin_util.h"
#include <stdint.h>

void dac_config(){
	// write protect disable
	DACC->DACC_WPMR = ('D' << 24) + ('A' << 16) + ('C' << 8) + 0;
	
	// set mode
	DACC->DACC_MR = (1 << 4);
	
	// enable channel 0
	DACC->DACC_CHER = 1;
	
	
	
	// write protect enable
	DACC->DACC_WPMR = ('D' << 24) + ('A' << 16) + ('C' << 8) + 1;
}

void init_dac(){
	// enable pmc
	PMC->PMC_PCR = (1 << 28) + (1 << 12) + 38;
	// configure dac
	dac_config();
	
}

void dac_write(uint16_t d){
	// 12 bit resolution
	uint16_t data = d;//PIOC->PIO_PDSR & ((1<<9) - 1);
	
	DACC->DACC_CDR = (data & ((1<<9) - 1)) << 20;
}


#endif /* DAC_DRIVER_H_ */