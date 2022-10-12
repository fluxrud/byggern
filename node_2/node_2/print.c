/**************************************************************************************************
---------------------------------------------------------------------------------------------------
	Copyright (c) 2013, Jonathan Bagg
	All rights reserved.

	 Redistribution and use in source and binary forms, with or without modification, are permitted 
	 provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of 
	  conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of 
	  conditions and the following disclaimer in the documentation and/or other materials provided 
	  with the distribution.
    * Neither the name of Jonathan Bagg nor the names of its contributors may be used to 
	  endorse or promote products derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
  POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------------------------------
   Processor	: Sam3/4
   File name    : print.c
---------------------------------------------------------------------------------------------------
   Created      : 11 May 2013                Author(s) : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Simple print functions for Atmel SAM microcontrollers using DMA
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <inttypes.h>
#include "sam.h"
#include "print.h"

#define	PF_BLOCK		0
#define	PF_RETURN		1
#define	PF_REMAINING		2

/**************************************************************************************************
*   Choose UART and BAUD rate
**************************************************************************************************/
#define	PF_DEV		USART2
#define PF_ID_UART	ID_UART
#define BAUD		9600
#define MCLK		CHIP_FREQ_CPU_MAX

/**************************************************************************************************
*   Print options - uncomment to enable
**************************************************************************************************/
//#define	PF_INT					//printi() int32 - adds 132 bytes on Cortex-M3, -Os
//#define	PF_HEX					//printx() uint32 as hex - adds 148 bytes on Cortex-M3, -Os

/**************************************************************************************************
*   Buffer full behaviour
*   	PF_BLOCK		-> blocks until the buffer has enough room.
*   	PF_RETURN		-> returns if there is no room on the buffer.
*   	PF_REMAINING		-> puts just what it can fit on the buffer.
**************************************************************************************************/
#define HANDLE_FULL		PF_BLOCK

/**************************************************************************************************
*   PREPROCESSOR ONLY - Do not edit
**************************************************************************************************/
#define	PF_TX_SIZE	256
#define	PF_UART		((Uart*)PF_DEV)
#define	remaining	(PF_TX_SIZE - PF_UART->UART_TCR - PF_UART->UART_TNCR)
#define out		(PF_UART->UART_TPR - (uint32_t) tx_buff)

static uint8_t tx_buff[PF_TX_SIZE];


/**************************************************************************************************
*   setup_print_uart() - initialises U(S)ART
**************************************************************************************************/
void setup_print_uart(void)
{
	PMC->PMC_WPMR = 0x504D4300;
	PMC->PMC_PCER0 = (1 << PF_ID_UART);
	PMC->PMC_WPMR = 0x504D4301;
	PF_UART->UART_CR = UART_CR_RXDIS | UART_CR_TXDIS | UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RSTSTA;
	PF_UART->UART_MR = 0xC0 | UART_MR_PAR_NO;
	PF_UART->UART_BRGR = (MCLK / BAUD / 16);
	PF_UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

/**************************************************************************************************
*   txbytes() - Copies the bytes in *data to the UART DMA buffer, then sets up the PDC transfer. If
*   	called during a PDC transfer, the transfer is stopped while the buffer is loaded. Buffer
*   	full behaviour is controlled by how HANDLE_FULL is defined.
**************************************************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void txbytes(uint8_t *data, uint8_t length)
{
	volatile uint32_t junk;
	static uint8_t in;
	uint8_t i = 0;

	#if (HANDLE_FULL == PF_BLOCK)
	do {
		PF_UART->UART_PTCR = UART_PTCR_TXTEN;
		while (remaining < length);
		PF_UART->UART_PTCR = UART_PTCR_TXTDIS;
		junk = PF_UART->UART_PTSR;		// before computing remaining, wait a couple of cycles 
	} while (remaining < length);			//  for PDC registers to stabilise after disable 

	#elif (HANDLE_FULL == PF_RETURN)
	PF_UART->UART_PTCR = UART_PTCR_TXTDIS;
	junk = PF_UART->UART_PTSR;			// before computing remaining, wait a couple of cycles 
	if (remaining < length) {			//  for PDC registers to stabilise after disable 
		PF_UART->UART_PTCR = UART_PTCR_TXTEN;
		return;
	}

	#elif (HANDLE_FULL == PF_REMAINING)
	PF_UART->UART_PTCR = UART_PTCR_TXTDIS;
	junk = PF_UART->UART_PTSR;			// before computing remaining, wait a couple of cycles 
	if (remaining < length)				//  for PDC registers to stabilise after disable 
		length = remaining;

	#endif

	if (!PF_UART->UART_TCR) {
		in = 0;
		PF_UART->UART_TPR = (uint32_t) tx_buff;
	}
	while (length--)
		tx_buff[in++] = data[i++];

	if (in < out || !(remaining-i)) {		//if in has wrapped
		PF_UART->UART_TCR = PF_TX_SIZE - out;
		PF_UART->UART_TNCR = in;
	}
	else
		PF_UART->UART_TCR+= i;

	PF_UART->UART_TNPR = (uint32_t) tx_buff;	//TNPR never decrements like TPR does

	PF_UART->UART_PTCR = UART_PTCR_TXTEN;
}
#pragma GCC diagnostic pop

/**************************************************************************************************
*   printi() - print integer.  Strips off the leading zeros.  Set end = '\n' or '\t' to get
*   	sent after the print or leave end = 0 to append nothing.
**************************************************************************************************/
void printi(int32_t x, uint8_t end)
{
#ifdef PF_INT
	uint8_t length = 0, digit, buff[12], leeding = 1;
	uint32_t d;

	if (x < 0) {
		buff[length++] = '-';
		x*= -1;
	}

/*	if (x < 999)				//speed up leeding zero strip, adds ~40 bytes
		d = 100;
	else if (x < 99999)
		d = 10000;
	else */
		d = 1000000000;

	while (d) {
		digit = (x / d);
		if (digit || !leeding) {
			buff[length++] = digit + 0x30;
			leeding = 0;
		}
		x%= d;
		d/= 10;
	}
	if (leeding)
		buff[length++] = 0x30;

	if (end)
		buff[length++] = end;

	txbytes(buff, length);
#endif
}

/**************************************************************************************************
*   printx() - print as hexidecimal.  Set end = '\n' or '\t' to get sent after the print or leave
*   	end = 0 to append nothing.  If x < 0x100, six leeding zeros are stripped.  If x < 0x10000,
*   	four leeding zeros are stripped. (so you see 8, 16, or 32 bits)
**************************************************************************************************/
void printx(uint32_t x, uint8_t end)
{
#ifdef PF_HEX
	const uint8_t hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	uint8_t buff[11], i, pos, length;

	if (x < 0x100) {
		pos = 3;
		length = 2;
	}
	else if (x < 0x10000) {
		pos = 5;
		length = 4;
	}
	else {
		pos = 9;
		length = 8;
	}

	buff[0] = '0';
	buff[1] = 'x';

	for (i=0;i<length;i++) {
		buff[pos--] = hex[x & 0xF];
		x>>=4;
	}
	length+= 2;

	if (end)
		buff[length++] = end;

	txbytes(buff, length);
#endif
}
