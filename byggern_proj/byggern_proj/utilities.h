/*
 * utilities.h
 *
 * Created: 31.08.2022 11:52:47
 *  Author: andbreis
 */ 


#ifndef UTILITIES_H_
#define UTILITIES_H_

// bit-macros
#define set_bit(reg, bit)					(reg |= (1 << bit))
#define clear_bit(reg, bit)					(reg &= ~(1 << bit))
#define test_bit(reg, bit)					(reg & (1 << bit))
#define loop_until_bit_is_set( reg, bit )	while (!test_bit( reg, bit ))
#define loop_until_bit_is_clear( reg, bit ) while (test_bit( reg, bit ))



#endif /* UTILITIES_H_ */