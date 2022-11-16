/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "printf-stdarg.h"

#include "can_controller.h"

//#include "pwm_driver.h"

#define DEBUG_INTERRUPT 0

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);
		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}
		pwm_joystick_move(message.data[0]);
		motor_set_position(255 - message.data[1]);
		if (message.data[2] > 0x10) 
		{
			PIOC->PIO_CODR = (1u << 22);
			adc_interrupt_clear();
		} else {
			PIOC->PIO_SODR = (1u << 22);
		}
		//printf("%x, %x, %x\n\r", message.data[0], message.data[1], message.data[2]);
		/*
		switch (message.data[0]){
			case 0x1f: {
				//printf("can: %x", message.data[1]);
				// joystick data
				pwm_joystick_move(message.data[0]);
				if (0){
					if (message.data[0] == 3){
						// move right
						printf("motor right \n\r");
						motor_set_speed(255);
						motor_set_direction(1);
						motor_set_enable(1);
						} else if (message.data[1] == 4){
						// move left
						printf("motor left \n\r");
						motor_set_speed(255);
						motor_set_direction(0);
						motor_set_enable(1);
						} else {
						printf("stop \n\r");
						motor_set_speed(0);
						motor_set_direction(0);
						motor_set_enable(0);
					}
				}
				break;
			}
			case 0x2f: {
				// right slider data
				//printf("slider pos: %d\n\r", message.data[1]);
				motor_set_position(255 - message.data[1]);
				break;
			}
			case 0x3f: {
				// left slider data
				//printf("slider pos: %d\n\r", message.data[1]);
				pin_util_toggle('C', 22);
				break;
			}
		}
		*/
		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;
	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();
}
