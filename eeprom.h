/*
* eeprom.h
*
* Created: 1/3/2018 3:50:39 AM
*  Author: GeDa
*/


#ifndef EEPROM_H_
#define EEPROM_H_

#include "eepromconfig.h"

uint8_t u8EEWriteByte(uint16_t u16addr, uint8_t u8data);
uint8_t u8EEReadByte(uint16_t u16addr, char *u8data);
char cEEReadMultiByte(char* ptr_data,uint8_t StartADR,uint8_t EndADR);
char cEEWriteMultiByte(char* ptr_data,uint8_t StartADR,uint8_t EndADR);
char checksum (uint8_t StartADR,uint8_t EndADR);

#endif /* EEPROM_H_ */