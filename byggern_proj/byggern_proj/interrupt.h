/*
 * interrupt.h
 *
 * Created: 05.10.2022 12:35:59
 *  Author: andrea7
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

ISR(INT0_vect){
	//Interrupten skal triggre denne funksjonen
	printf("received can interrupt");
}

void interrupt_enable()
{
	set_bit(GICR, 6);
	set_bit(MCUCR, 1);
	clear_bit(MCUCR, 0);
	sei();
}


#endif /* INTERRUPT_H_ */