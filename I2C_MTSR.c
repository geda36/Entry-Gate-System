/*
* I2C_MTSR.c
*
* Created: 12/26/2017 2:55:02 AM
*  Author: GeDa
*/

#include <avr/io.h>
#include <avr/iom32.h>
#define F_CPU 11059200UL
#include "I2C_MTSR.h"

void vI2CInit (unsigned long scl_freq)
{
	unsigned long twbr;
	//No Prescaler
	TWSR &=~ (1U<<TWPS0);
	TWSR &=~ (1U<<TWPS1);
	//for XTAL=11.0592MHz
	twbr = (F_CPU/scl_freq-16)/(2*1);
	TWBR = (uint8_t)twbr;
	//enable TW
	TWCR |= (1U<<TWEN);
}

void vI2CStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) != (1<<TWINT));			//wait to finish
}

void vI2CStop (void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void vI2CWrite (char data)
{
	TWDR=data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) != (1<<TWINT));
}

uint8_t u8I2CReadACK (unsigned char ack)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(ack<<TWEA);
	while((TWCR & (1<<TWINT)) != (1<<TWINT));
	return TWDR;
}

uint8_t u8I2CReadNACK (void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) != (1<<TWINT));
	return TWDR;
}

uint8_t u8I2CGetStatus (void)
{
	uint8_t status;
	status = TWSR & 0xF8;
	return status;
}
