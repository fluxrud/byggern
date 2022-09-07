/*
 * address_decoder.c
 *
 * Created: 31.08.2022 16:17:24
 *  Author: andbreis
 */ 


/*
recommended memory mapping

address space
0b XXXX aaaa aaaa aaaa	(XXXX is occupied by JTAG)

Starting on > 0x0500 since internam memory uses ~0.5kB of the first address space

OLED CMD	0x1000 - 0x11FF		(0.5kB)
OLED_DATA	0x1200 - 0x13FF		(0.5kB)
ADC			0x1400 - 0x17FF		(1kB)
SRAM		0x1800 - 0x1FFF		(2kB)

Unique bits [8...11]
OLED_CMD:	0b000X => CS = not A and not B	Wolfram to NAND => CS = not (not A nand not B)
OLED_DATA:	0b001X
ADC:		0b01XX => CS = not A and B		Wolfram to NAND => CS = not (not A nand B)
SRAM:		0b1XXX => CS = A and B			Wolfram to NAND => CS = A

SRAM base address 0x1800

Microcontroller
	PA0 - PA7	memory_address [0...7]
	PC0 - PC7	memory_address [8...15]
	PD6			WR
		Falling edge -> Processor has valid data on databus
		Rising edge  -> Data should be latched on the external device
	PD7			RD
		Falling edge -> External device should output valid data
		Rising edge  -> Processor will latch the data
	PE1			ALE
		Rising edge	 -> Start of bus cycle, open latch for low-byte
		Falling edge -> Lock latch and indicate that valid 16-bit is on address bus


*/