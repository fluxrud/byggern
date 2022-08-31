/*
 * pin_util.h
 *
 * Created: 31.08.2022 11:51:53
 *  Author: andbreis
 */ 


#ifndef PIN_UTIL_H_
#define PIN_UTIL_H_

#include <stdint.h>
#include <avr/io.h>

#include "utilities.h"


void toggle_pin(char port, uint8_t pin);


#endif /* PIN_UTIL_H_ */