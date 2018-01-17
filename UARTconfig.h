/*
 * UARTconfig.h
 *
 * Created: 12/25/2017 7:04:31 PM
 *  Author: GeDa
 */ 


#ifndef UARTCONFIG_H_
#define UARTCONFIG_H_

#include <avr/io.h>

#define RXD          0
#define RXD_DIR      DDRD
#define RXD_DATAIN   PIND
#define RXD_PULLUP   PORTD

#define TXD          1
#define TXD_DIR      DDRD
#define TXD_DATAOUT  PORTD






#endif /* UARTCONFIG_H_ */