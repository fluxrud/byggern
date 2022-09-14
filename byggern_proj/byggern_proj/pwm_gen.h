/*
 * pwm_gen.h
 *
 * Created: 14.09.2022 09:58:34
 *  Author: andrea7
 */ 


#ifndef PWM_GEN_H_
#define PWM_GEN_H_


/*
	COMn1 and COMn0 => waveform generator chosen on OCn pin
	
	wavegen has Normal, CTC and PWM mode
	
	PD5 is connected (OC1A/TOSC2) => Timer/Counter1 register A
*/

void set_fast_pwm_mode()
{
	// fast PWM
	set_bit(TCCR1A, WGM10);
	set_bit(TCCR1A, WGM11);
}

void set_ctc_mode()
{
	// CTC mode PD4
	clear_bit(TCCR3A, WGM30);
	clear_bit(TCCR3A, WGM31);
	set_bit(TCCR3B, WGM32);
	clear_bit(TCCR3B, WGM33);
}

void set_selected_clock()
{
	// clock prescaler selection clk_io / 1
	set_bit(TCCR3B, CS30);
	clear_bit(TCCR3B, CS31);
	clear_bit(TCCR3B, CS32);
}

void set_compare_match_mode()
{
	// toggle output on compare
	set_bit(TCCR3A, COM3A0);
	clear_bit(TCCR3A, COM3A1);
}

void init_pwm()
{	
	// set PD4 as output
	set_bit(DDRD, DDD4);	
	
	set_ctc_mode();
	set_selected_clock();
	set_compare_match_mode();
	
	// set max freq, f_clk / 2
	OCR3A = 0x00;
}

#endif /* PWM_GEN_H_ */