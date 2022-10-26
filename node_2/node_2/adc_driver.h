/*
 * adc_driver.h
 *
 * Created: 26.10.2022 13:52:48
 *  Author: andrea7
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

/*

11 available analog inputs

shield pin:	A0
adc:		AD7
channel:	CH0
IO line:	PA16
Peripheral:	X1

expected value [0, 3.3]V

interrupt:
	trigger on negedge
	clear with sw


*/

void adc_config();

void init_adc(){
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
		ADC->ADC_EMR = (1 << 24) + (7 << 4) + 0x3; // tri
		
		// set threshold values
		uint16_t hig_th = 0xffff;
		uint16_t low_th = 0x0050;
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

void ADC_Handler(void){
	uint16_t adc_data = adc_read_lcdr(7);
	printf("INTERRIPt, adc: %x\n\r", adc_data);
	
	if(adc_data < 0x100 && 0){
		// disable adc interrupt
		ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b0;
		ADC->ADC_IDR = (0x1f << 24) + 0xffff;
		ADC->ADC_WPMR = ('A' << 24) + ('D' << 16) + ('C' << 8) + 0b1;	
	}
	NVIC_ClearPendingIRQ(ID_ADC);
}


#endif /* ADC_DRIVER_H_ */