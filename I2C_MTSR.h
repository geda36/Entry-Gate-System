/*
 * I2C_MTSR.h
 *
 * Created: 12/26/2017 2:57:50 AM
 *  Author: GeDa
 */ 


#ifndef I2C_MTSR_H_
#define I2C_MTSR_H_ 

#include <inttypes.h>
 
void vI2CInit (unsigned long scl_freq);
void vI2CStart(void);
void vI2CWrite (char data);
void vI2CStop (void);
unsigned char u8I2CReadACK (unsigned char ack);
uint8_t u8I2CReadNACK (void);
unsigned char u8I2CGetStatus (void);

#endif /* I2C_H_ */