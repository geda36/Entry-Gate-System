/*
 * eepromconfig.h
 *
 * Created: 1/3/2018 3:55:06 AM
 *  Author: GeDa
 */ 


#ifndef EEPROMCONFIG_H_
#define EEPROMCONFIG_H_

#define SUCCESS           0U
#define ERROR             1U
#define EEDEVADR          0xA0
#define START             0x08
#define REPEATED_START    0x10
#define SLA_ACK_ADR_W     0x18
#define SLA_ACK_DATA      0x28
#define SLA_ACK_ADR_R     0x40
#define NACK_DATA         0x58


#endif /* EEPROMCONFIG_H_ */