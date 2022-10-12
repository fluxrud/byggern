/*
 * can.h
 *
 * Created: 05.10.2022 09:53:31
 *  Author: andrea7
 */ 


#ifndef CAN_H_
#define CAN_H_

#include "mcp2515.h"
#include <stdlib.h>

struct can_msg_t
{
	uint8_t id;
	uint8_t data_l;
	uint8_t* data;
};

struct can_msg_t can_read_rx_buf0();
void display_can_frame(struct can_msg_t msg);

ISR(INT0_vect){
	char canstat = mcp2515_read(0x0e);
	char ICOD = ((((1 << 3) - 1) << 1) & canstat) >> 1;
	if( ICOD == 6 ){
		struct can_msg_t msg = can_read_rx_buf0();
		display_can_frame(msg);
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
	
	t_bit = t_sync + t_propseg + t_ps1 + t_ps2
	
	t_sync		= 1 T
	t_propseg	= 2 T
	t_ps1		= 8 T
	t_ps2		= 8 T
	
	t_bit = 19 T
	
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
	can_set_config_mode(MODE_NORMAL);
	
	// enable receive interrupt
	mcp2515_bit_mod(MCP_CANINTE, MCP_RX_INT, MCP_RX_INT);
}

void can_transmit_tx_buf0(struct can_msg_t msg)
{
	uint8_t tx_buf0_base = 0b00110000;
	uint8_t idh_adr = 0b0001;
	uint8_t idl_adr = 0b0010;
	uint8_t dlc_adr = 0b0101;
	uint8_t dat_adr = 0b0110;
	
	mcp2515_write(tx_buf0_base + idh_adr, msg.id >> 3);
	mcp2515_write(tx_buf0_base + idl_adr, msg.id << 5);
	mcp2515_write(tx_buf0_base + dlc_adr, msg.data_l);
	for (uint8_t i = 0; i < msg.data_l && i < 8; i++){
		mcp2515_write(tx_buf0_base + dat_adr + i, msg.data[i]);
	}

	mcp2515_send_req(0b1);
}

struct can_msg_t can_read_rx_buf0()
{
	uint8_t rx_buf0_base = 0b01100000;
	uint8_t idh_adr = 0b0001;
	uint8_t idl_adr = 0b0010;
	uint8_t dlc_adr = 0b0101;
	uint8_t dat_adr = 0b0110;
	
	struct can_msg_t msg;
	msg.id =  mcp2515_read(rx_buf0_base + idh_adr) << 3;
	msg.id += mcp2515_read(rx_buf0_base + idl_adr) >> 5;
	msg.data_l = mcp2515_read(rx_buf0_base + dlc_adr);
	msg.data = (uint8_t*)malloc(msg.data_l);
	
	for (uint8_t i = 0; i < msg.data_l && i < 8; i++){
		msg.data[i] = mcp2515_read(rx_buf0_base + dat_adr + i);
	}
	
	return msg;
}

void display_can_frame(struct can_msg_t msg){
	printf("\n\rCANFRAME");
	printf("\n\r id: %02x",msg.id);
	printf("\n\r data_l: %d\n\r data: ",msg.data_l);
	for (uint8_t i = 0; i < msg.data_l; i++){
		printf("%02x", msg.data[i]);
	}
}


#endif /* CAN_H_ */