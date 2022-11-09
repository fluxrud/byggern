/*
 * pwm.h
 *
 * Created: 19.10.2022 15:38:55
 *  Author: andrea7
 */ 


#ifndef PWM_H_
#define PWM_H_

/*

5v protected pins

name:		PWMH5 
pin:		PC19 
peripheral: B
(pin 44 on shield)

name:		PWMH6
pin:		PC18
peripheral: B
(pin 45 on shield)

*/

#include "sam.h"

#include "motor_driver.h"

uint32_t CPRD = 52500;//1680000; // 
static uint32_t servo_pos_us = 1500;

void pwm_config(uint8_t ch_num);

void init_pwm(){
	
	// pin disable PIO
	PIOC->PIO_PDR |= PIO_PDR_P19;
	PIOC->PIO_PDR |= PIO_PDR_P18;
	
	// pin peripheral b select
	PIOC->PIO_ABSR |= PIO_ABSR_P19;
	PIOC->PIO_ABSR |= PIO_ABSR_P18;
	
	PMC->PMC_PCR = (1 << 28) + (1 << 12) + 36;
	
	
	pwm_config(5);
	pwm_config(6);
}

void pwm_config(uint8_t ch_num){
	
	PWM->PWM_ENA &= ~(1 << ch_num);
	// disable write protect for all register groups to enable configuration of pwm
	PWM->PWM_WPCR = ('P' << 24) + ('W' << 16) + ('M' << 8) + (0b1111111 << 2) + 0b00;
	
	// clk gen?
	//PWM->PWM_CLK = PWM_CLK_DIVA(1) + PWM_CLK_DIVB(1) + PWM_CLK_PREA(1) + PWM_CLK_PREB(1);
	
	// select clock
	PWM->PWM_CH_NUM[ch_num].PWM_CMR = 0b0101; // select CLKA : 0b1011, select MCLK : 0b0000;
	
	// select period
	PWM->PWM_CH_NUM[ch_num].PWM_CPRD = CPRD;
	
	// set duty cycle
	PWM->PWM_CH_NUM[ch_num].PWM_CDTY = CPRD - 1500 * 84 / 32;
	
	// set cmp mode
	//PWM->PWM_CH_NUM[ch_num].PWM_CMPM =
	
	// enable pwm channel
	PWM->PWM_ENA |= 1 << ch_num;
	
	// lock write-protect
	PWM->PWM_WPCR = ('P' << 24) + ('W' << 16) + ('M' << 8) + (0b1111111 << 2) + 0b10;
}


/*
error 1 on invalid us value
*/
uint8_t pwm_set_dc(uint8_t ch_num, uint32_t us_high){
	if (us_high >= 2000 || us_high <= 1000){
		return 1;	
	}
	uint32_t period_us = CPRD * 32 / 84; // us
	uint32_t duty_cycle_divisor = period_us / us_high;
	
	PWM->PWM_WPCR = ('P' << 24) + ('W' << 16) + ('M' << 8) + (0b1111111 << 2) + 0b00;
	PWM->PWM_CH_NUM[ch_num].PWM_CDTY = CPRD - CPRD / duty_cycle_divisor;
	printf("dc: %d\n\r", CPRD - CPRD / duty_cycle_divisor);
	PWM->PWM_WPCR = ('P' << 24) + ('W' << 16) + ('M' << 8) + (0b1111111 << 2) + 0b10;
	
	//PWM->PWM_CH_NUM[ch_num].PWM_CDTYUPD = CPRD - CPRD / duty_cycle_divisor;
	
	return 0;
}

void pwm_joystick_move(uint8_t dir){
	uint8_t joystick_delta = 50;
	
	if (dir == 3){
		// right
		servo_pos_us -= joystick_delta;
		if(pwm_set_dc(5, servo_pos_us) != 0) servo_pos_us += joystick_delta; // out of bounds correction
		
		//motor_set_enable(1);
		//motor_set_direction(1);
		//motor_set_speed(0xcf);
		
	} else if (dir == 4){
		// left
		servo_pos_us += joystick_delta;
		if(pwm_set_dc(5, servo_pos_us) != 0) servo_pos_us -= joystick_delta; // out of bounds correction
		
		//motor_set_enable(1);
		//motor_set_direction(0);
		//motor_set_speed(0xcf);
	}
}
	

#endif /* PWM_H_ */



/*

INIT protocol from datasheet

? Unlock User Interface by writing the WPCMD field in the PWM_WPCR Register.

? Configuration of the clock generator (DIVA, PREA, DIVB, PREB in the PWM_CLK register if required).

? Selection of the clock for each channel (CPRE field in the PWM_CMRx register)

? Configuration of the waveform alignment for each channel (CALG field in the PWM_CMRx register)

? Selection of the counter event selection (if CALG = 1) for each channel (CES field in the PWM_CMRx
register)

? Configuration of the output waveform polarity for each channel (CPOL in the PWM_CMRx register)

? Configuration of the period for each channel (CPRD in the PWM_CPRDx register). Writing in PWM_CPRDx
register is possible while the channel is disabled. After validation of the channel, the user must use
PWM_CPRDUPDx register to update PWM_CPRDx as explained below.

? Configuration of the duty-cycle for each channel (CDTY in the PWM_CDTYx register). Writing in
PWM_CDTYx register is possible while the channel is disabled. After validation of the channel, the user
must use PWM_CDTYUPDx register to update PWM_CDTYx as explained below.

? Configuration of the dead-time generator for each channel (DTH and DTL in PWM_DTx) if enabled (DTE bit
in the PWM_CMRx register). Writing in the PWM_DTx register is possible while the channel is disabled.
After validation of the channel, the user must use PWM_DTUPDx register to update PWM_DTx

? Selection of the synchronous channels (SYNCx in the PWM_SCM register)

? Selection of the moment when the WRDY flag and the corresponding PDC transfer request are set (PTRM
and PTRCS in the PWM_SCM register)

? Configuration of the update mode (UPDM in the PWM_SCM register)

? Configuration of the update period (UPR in the PWM_SCUP register) if needed.

? Configuration of the comparisons (PWM_CMPVx and PWM_CMPMx).

? Configuration of the event lines (PWM_ELMRx).

? Configuration of the fault inputs polarity (FPOL in PWM_FMR)

? Configuration of the fault protection (FMOD and FFIL in PWM_FMR, PWM_FPV and PWM_FPE/2)

? Enable of the Interrupts (writing CHIDx and FCHIDx in PWM_IER1 register, and writing WRDYE, ENDTXE,
TXBUFE, UNRE, CMPMx and CMPUx in PWM_IER2 register)

? Enable of the PWM channels (writing CHIDx in the PWM_ENA register)

*/