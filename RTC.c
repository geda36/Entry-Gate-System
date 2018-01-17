/*
 * RTC.c
 *
 * Created: 12/27/2017 10:29:18 PM
 *  Author: GeDa
 */ 

#include "RTC.h"
#include "I2C_MTSR.h"

void vRTCGetTime (char* hour, char* min, char* sec)
{
	vI2CStart();
	vI2CWrite(0xD0);
	vI2CWrite(0);
	vI2CStart();
	vI2CWrite(0xD1);
	*sec=u8I2CReadACK(1);
	*min=u8I2CReadACK(1);
	*hour=u8I2CReadNACK();
	vI2CStop();
}

void vRTCGetDate (char* day, char* month, char* year)
{
	 vI2CStart();
	 vI2CWrite(0xD0);
	 vI2CWrite(4);
	 vI2CStart();
	 vI2CWrite(0xD1);
	 *day=u8I2CReadACK(1);
	 *month=u8I2CReadACK(1);
	 *year=u8I2CReadNACK();
	 vI2CStop();
}
