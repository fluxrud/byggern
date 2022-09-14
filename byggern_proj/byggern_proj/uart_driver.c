/*
 * uart_driver.c
 *
 * Created: 31.08.2022 13:55:36
 *  Author: andbreis
 */ 

#include "uart_driver.h"

// Following functions are taken from "Atmel AVR ATmega162 datasheet"

void USART_Init(unsigned int ubrr)
{
	/* set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

int USART_Transmit (char data, FILE* file)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0) ));
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}

int USART_Receive ( FILE* file )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	
	/* Get and return received data from buffer */
	return UDR0;
}

/*
This function reads USART characters and responds with received character.
*/
void test_usart()
{
	unsigned char c = USART_Receive( NULL );
	if(c < 32) return;
	char* response = "Received: ~";
	response[10] = c;
	for(int i = 0; i < strlen(response); i++) USART_Transmit(response[i], NULL);
}