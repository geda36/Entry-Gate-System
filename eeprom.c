/*
* eeprom.c
*
* Created: 1/2/2018 2:13:57 PM
*  Author: GeDa
*/
#define F_CPU 11059200UL
#include <inttypes.h>
#include <util/delay.h>
#include "I2C_MTSR.h"
#include "eeprom.h"

uint8_t u8EEWriteByte(uint16_t u16addr, uint8_t u8data)
{
	vI2CStart();
	if (u8I2CGetStatus() != START)
	return ERROR;
	//select device(1 EEPROM on bus) and send A2(0) A1(0) A0(0) address bits
	vI2CWrite((EEDEVADR)|(uint8_t)((u16addr & 0x0700)>>7));
	if (u8I2CGetStatus() != SLA_ACK_ADR_W)
	return ERROR;
	//send the rest of address
	vI2CWrite((uint8_t)(u16addr));
	if (u8I2CGetStatus() != SLA_ACK_DATA )
	return ERROR;
	//write byte to eeprom
	vI2CWrite(u8data);
	if (u8I2CGetStatus() != SLA_ACK_DATA)
	return ERROR;
	vI2CStop();
	return SUCCESS;
}

uint8_t u8EEReadByte(uint16_t u16addr, char *u8data)
{
	vI2CStart();
	if (u8I2CGetStatus() != START)
	return ERROR;
	//select device(1 EEPROM on bus) and send A2(0) A1(0) A0(0) address bits
	vI2CWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7)));
	if (u8I2CGetStatus() != SLA_ACK_ADR_W)
	return ERROR;
	//send the rest of address
	vI2CWrite((uint8_t)(u16addr));
	if (u8I2CGetStatus() != SLA_ACK_DATA )
	return ERROR;
	//send start
	vI2CStart();
	if (u8I2CGetStatus() != REPEATED_START)
	return ERROR;
	//select device and send read bit
	vI2CWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7))|1);
	if (u8I2CGetStatus() != SLA_ACK_ADR_R)
	return ERROR;
	*u8data = u8I2CReadNACK();
	if (u8I2CGetStatus() != NACK_DATA)
	return ERROR;
	vI2CStop();
	return SUCCESS;
}

char cEEReadMultiByte(char* ptr_data,uint8_t StartADR,uint8_t EndADR)
{
	uint8_t offset=0;
	char chkR=0;
	for(offset=StartADR;offset<EndADR;offset++,ptr_data++)
	{
		u8EEReadByte(offset,ptr_data);
		chkR^=*ptr_data;
	}
	return chkR;
}

char cEEWriteMultiByte(char* ptr_data,uint8_t StartADR,uint8_t EndADR)
{
	uint8_t offset=0;
	char chkW=0;
	for(offset=StartADR;offset<EndADR;offset++,ptr_data++)
	{
		u8EEWriteByte(offset,*ptr_data);
		_delay_ms(1);
		chkW^=*ptr_data;
	}
	return chkW;
}

char checksum (uint8_t StartADR,uint8_t EndADR)
{
	uint8_t offset=0;
	char chkR=0,Ret_data;
	for(offset=StartADR;offset<EndADR;offset++)
	{
		u8EEReadByte(offset,&Ret_data);
		chkR^=Ret_data;
	}
	return chkR;
}
