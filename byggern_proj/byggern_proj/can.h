/*
 * can.h
 *
 * Created: 05.10.2022 09:53:31
 *  Author: andrea7
 */ 


#ifndef CAN_H_
#define CAN_H_

#include "mcp2515.h"

/*
000 = Set Normal Operation mode
001 = Set Sleep mode
010 = Set Loopback mode
011 = Set Listen-Only mode
100 = Set Configuration mode
*/

ISR(INT0_vect){
	//Interrupten skal triggre denne funksjonen
	printf("Interrupt, ");
	// can_receive call
	
	
	 /*
	 0b1000
	 0b0111
	 0b1110
	 */
	char canstat = mcp2515_read(0x0e);
	char ICOD = ((((1 << 3) - 1) << 1) & canstat) >> 1;
	if( ICOD == 6 ){
		printf("rx buffer 0: %c, canintf = %x\n", mcp2515_read(0b01100110), mcp2515_read(MCP_CANINTF));
	}
	
	// clear int flag
	mcp2515_bit_mod(MCP_CANINTF, 1 + (1 << 2), 0);
	set_bit(GIFR, 6);
}

void can_interrupt_enable()
{
	set_bit(GIFR, 6);
	set_bit(GICR, 6);
	
	// negedge interrupt
	set_bit(MCUCR, 1);
	clear_bit(MCUCR, 0);
	sei();
}


void can_set_config_mode(char c)
{
	mcp2515_bit_mod(MCP_CANCTRL, 0b111 << 5, c);
}

void init_can()
{
	// set up interrupt on PD2
	can_interrupt_enable();
	
	/*
	conf registers
	
	PropSeg + PS1 >= PS2
	PropSeg + PS1 >= TDELAY
	PS2 > SJW
	
	*/
	
	char SJW = 0xc0;
	char BRP = 0x01; // set baud-rate T = 2 * (brp + 1) / f_osc
	
	char BTLMOD = 0x00;
	char SAM = 0x40;
	char PHSEG1 = 0x38;
	char PRSEG = 0x01;
	
	char SOF = 0x80;
	char WAKFIL = 0x00;
	char PHSEG2 = 0x07;
	
	mcp2515_bit_mod(MCP_CNF1, 0xff, SJW + BRP);
	mcp2515_bit_mod(MCP_CNF2, 0xff, BTLMOD + SAM + PHSEG1 + PRSEG);
	mcp2515_bit_mod(MCP_CNF3, 0xff, SOF + WAKFIL + PHSEG2);
	
	// loopback
	can_set_config_mode(MODE_LOOPBACK);
	
	// enable receive interrupt
	mcp2515_bit_mod(MCP_CANINTE, MCP_RX_INT, MCP_RX_INT);
	
	
}


void can_transmit()
{
	char upper_adr = 0b00110000;
	
	char IDH = 0b0001;
	mcp2515_write(upper_adr + IDH, 0xFF);
	char IDL = 0b0010;
	mcp2515_write(upper_adr + IDL, 0x00);
	
	
	char DLC = 0b0101;
	mcp2515_write(upper_adr + DLC, 1);
	
	char D_0 = 0b0110;
	mcp2515_write(upper_adr + D_0, 'x');
	
	
	// request to send tx buffer 0
	mcp2515_send_req(0b1);
}

void can_receive()
{
	// CANSTAT to read interrupt flags
}


#endif /* CAN_H_ */