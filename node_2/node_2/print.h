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
   File name    : print.h
---------------------------------------------------------------------------------------------------
   Created      : 11 May 2013                Author(s) : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Simple print functions for Atmel SAM microcontrollers using DMA
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_

void setup_print_uart(void);
void txbytes(uint8_t *data, uint8_t len);
void printi(int32_t x, uint8_t end);
void printx(uint32_t x, uint8_t end);
#define prints(s)	txbytes((uint8_t *)s, sizeof(s)-1)

#endif //_DEBUG_H_
