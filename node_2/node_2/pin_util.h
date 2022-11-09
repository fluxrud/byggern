/*
 * pin_util.h
 *
 * Created: 12.10.2022 13:32:01
 *  Author: andrea7
 */ 


#ifndef PIN_UTIL_H_
#define PIN_UTIL_H_


enum PIN_DIR_TYPE{OUTPUT_ENABLE = 1, PIO_ENABLE, PIO_DISABLE};
	
void pin_util_set_dir(char port, uint8_t pin, enum PIN_DIR_TYPE type)
{
	switch (port) {
		case 'A':
		{
			//PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
			if (type == 1){
				PIOA->PIO_OER |= 1u << pin;	
			} else if (type == 2){
				PIOA->PIO_PER |= 1u << pin;
			}
			//PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 1;
			break;
		}
		case 'B':
		{
			//PIOB->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
			if (type == 1){
				PIOB->PIO_OER |= 1u << pin;
			} else if (type == 2){
				PIOB->PIO_PER |= 1u << pin;
			}
			//PIOB->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 1;
			break;
		}
		case 'C':
		{
			//PIOC->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
			if (type == 1){
				PIOC->PIO_OER |= 1u << pin;
				} else if (type == 2){
				PIOC->PIO_PER |= 1u << pin;
			}
			//PIOC->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 1;
			break;
		}
		case 'D':
		{
			//PIOD->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 0;
			if (type == 1){
				PIOD->PIO_OER |= 1u << pin;
				} else if (type == 2){
				PIOD->PIO_PER |= 1u << pin;
			}
			//PIOD->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8) + 1;
			break;
		}
		default: break;
	}
}

void pin_util_toggle(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			uint32_t port_data = PIOA->PIO_ODSR;
			if (port_data & (1u << pin))	PIOA->PIO_CODR = (1u << pin);
			else							PIOA->PIO_SODR = port_data | (1u << pin);
			//PIOA->PIO_SODR = port_data ^ (1u << pin);
			//PIOA->PIO_SODR = port_data & ~(1 << pin);
			break;
		}
		case 'D':
		{
			uint32_t port_data = PIOD->PIO_ODSR;
			if (port_data & (1u << pin))	PIOD->PIO_CODR = (1u << pin);
			else							PIOD->PIO_SODR = (1u << pin);
			//PIOA->PIO_SODR = port_data ^ (1u << pin);
			//PIOA->PIO_SODR = port_data & ~(1 << pin);
			break;
		}
		default: break;
	}
}

void pin_util_set(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			uint16_t port_data = PIOA->PIO_ODSR;
			PIOA->PIO_SODR = port_data | (1 << pin);
			break;
		}
		default: break;
	}
}

void pin_util_clear(char port, uint8_t pin)
{
	switch (port) {
		case 'A':
		{
			PIOA->PIO_CODR = (1 << pin);
			break;
		}
		default: break;
	}
}



#endif /* PIN_UTIL_H_ */