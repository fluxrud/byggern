/*
 * uart_driver.h
 *
 * Created: 31.08.2022 13:55:58
 *  Author: andbreis
 */ 


#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <avr/io.h>
#include <stdint.h>

#include "utilities.h"
#include <stdio.h>
#include <string.h>


#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init( unsigned int ubrr );
void USART_Transmit (unsigned char data, FILE*);
unsigned char USART_Receive ( FILE* );
void test_usart();


#endif /* UART_DRIVER_H_ */