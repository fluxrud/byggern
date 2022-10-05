/*
 * spi.h
 *
 * Created: 28.09.2022 11:56:48
 *  Author: andrea7
 */ 


#ifndef SPI_H_
#define SPI_H_

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_SCK  DDB7
#define DD_MISO DDB6
#define DD_SS	DDB4
void SPI_clear_slave_select();
 // from datasheet ATmega162
 
void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	//DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	set_bit(DDR_SPI, DD_MOSI);
	set_bit(DDR_SPI, DD_SCK);
	set_bit(DDR_SPI, DD_SS);
	clear_bit(DDR_SPI, DD_MISO);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPI_clear_slave_select();
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0); // |(1<<SPR1)
	
	SPI_clear_slave_select();
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}


char SPI_MasterTrancieve(char cData)
{
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)))
		;
	
	return SPDR;
}

void SPI_set_slave_select()
{
	char reg = PINB;
	clear_bit(reg, DD_SS);
	PORTB = reg;
}

void SPI_clear_slave_select()
{
	char reg = PINB;
	set_bit(reg, DD_SS);
	PORTB = reg;
}

#endif /* SPI_H_ */