/*
 * pwm.h
 *
 * Created: 19.10.2022 15:38:55
 *  Author: andrea7
 */ 


#ifndef PWM_H_
#define PWM_H_

/*

name:		PWMH5 
pin:		PC19 
peripheral: B
(pin 44 on shield)

*/

#include "sam.h"

uint32_t CPRD = 1680000;

void init_pwm(){
	
	int ch_num = 5;
	
	// disable write protect for all register groups to enable configuration of pwm
	PWM->PWM_WPCR = ('P' << 24) + ('W' << 16) + ('M' << 8) + (0b1111111 << 2) + 0b00;
	
	// clk gen?
	
	// select clock
	//PWM->PWM_CH_NUM[ch_num]->PWM_CMR = ()
	
	// select period
	PWM->PWM_CH_NUM[ch_num].PWM_CPRD = CPRD;
	
	// set duty cycle
	PWM->PWM_CH_NUM[ch_num].PWM_CDTY = CPRD / 2;
	
	// set cmp mode
	//PWM->PWM_CH_NUM[ch_num].PWM_CMPM = 
	
	// enable pwm channel
	PWM->PWM_ENA = 1 << 5;
	
	// lock write-protect
	PWM->PWM_WPCR = ('P' << 24) + ('W' << 16) + ('M' << 8) + (0b1111111 << 2) + 0b10;
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