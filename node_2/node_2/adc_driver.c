/*
 * adc_driver.c
 *
 * Created: 02.11.2022 10:20:26
 *  Author: andrea7
 */ 

#include "adc_driver.h"
#include "can_controller.h"

void adc_config();

void init_adc(){
	adc_interrupt_flag = 0;
	// disable pin io
	//PIOA->PIO_PDR |= PIO_PDR_P2;
	
	// enable pio
	//PIOA->PIO_PER |= PIO_PER_P2;
	
	// output disable
	//PIOA->PIO_ODR |= PIO_PDR_P2;
	
	// enable peripheral
	// Extra peripheral (i.e. ADC) is enabled when enabling the corresponding channel with ADC_CHER
	
	// enable pmc
	PMC->PMC_PCR = (1 << 28) + (1 << 12) + 37;
	
	adc_config();
	
	//Enable interrupt in NVIC
	NVIC_EnableIRQ(ID_ADC);
}


void adc_config(){
	
	uint8_t ch_num = 7;
	
	// disable write protection
	ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b0;
	
	// set mode
	/*
	TRGEN: hardware triggers disabled by default
	TRGSEL: external trigger by default
	LOWRES: 12 bit res by default
	SLEEP: awake by default
	FWUP: fast wakeup from sleep, off by default
	FREERUN: never wait for trigger, off by default
	PRESCAL: ADC clock prescaler, set below to 0 (adc_clk = mck / 2(prescal + 1))
	STARTUP: 0 periods by default
	SETTLING: 3 periods by default
	ANACH: no change by default
	TRACKTIM: (Tracking time = (TRACKTIM + 1) * adc_clock_periods)
	TRANSFER: (Transfer period = (TRANSFER*2 + 3) * adc_clock_periods)
	USEQ: Normal mode by default
	*/
	ADC->ADC_MR = 1 << 7; // enable free run
	
	// interrupt on negedge
	if(1){
		// interrupt enable
		ADC->ADC_IER = 1 << 26; // trigger on comparison event
		
		// enable tag in extended mode register for channel numbers in output
		ADC->ADC_EMR = (1 << 24) + (7 << 4) + 0x0; // tri
		
		// set threshold values
		uint16_t hig_th = 0xfff;
		uint16_t low_th = adc_interrupt_lth;
		ADC->ADC_CWR = (hig_th << 16) + low_th;	
	}
	
	// channel enable
	ADC->ADC_CHER = 1 << ch_num;
	//ADC->ADC_CHDR = ~(1 << ch_num); // disable other channels for freerun mode
	
	// enable write protection
	ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b1;
}



uint16_t adc_read_lcdr(uint8_t ch_num){
	// control register: ADC_CR, START bits to begin conversion
	ADC->ADC_CR = 0b10;
	
	// wait?
	// eventually wait for transfer period and conversion period
	
	uint32_t adc_data = ADC->ADC_LCDR;
		
	// channel data check
	uint32_t adc_data_channel = (adc_data & (0b1111 << 12)) >> 12;
	if(adc_data_channel != ch_num){
		printf("\n\rData read from wrong channel (ch_num: %d)", ch_num);
		return 0;
	}
	
	// Last converted data register: ADC_LCDR
	return (adc_data & (0xfff));
}

uint16_t adc_read(uint8_t ch_num) {
	return ADC->ADC_CDR[ch_num];
}

void adc_interrupt_clear(){
	ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b0;
	ADC->ADC_IER = 1 << 26;
	ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b1;
}

void adc_interrupt_disable(){
	ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b0;
	ADC->ADC_IDR = (0x1f << 24) + 0xffff;
	ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b1;
}

void ADC_Handler(void){
	uint16_t adc_data = adc_read_lcdr(7);
	printf("INTERRIPt, adc: %x\n\r", adc_data);
	//printf("is: %x\n\r", ADC->ADC_ISR);
	int dummy_read = ADC->ADC_ISR;
	if(adc_data < adc_interrupt_lth){
		printf("gaol!\n\r");
		adc_interrupt_disable();
		adc_interrupt_flag = 1;
		struct can_message_t msg;
		msg.id = 0x02;
		msg.data_length = 1;
		msg.data[0] = 1;
		int ret = can_send(&msg, 0);
		adc_interrupt_flag = 0;
	}
	
	NVIC_ClearPendingIRQ(ID_ADC);
}