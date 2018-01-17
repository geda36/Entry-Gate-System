/*
 * UART.h
 *
 * Created: 12/25/2017 5:15:10 PM
 *  Author: GeDa
 */ 


#ifndef UART_H_
#define UART_H_

#include "UARTconfig.h"
//function prototypes
void vUartInit(void);
void vUartTransmit (char data);
unsigned char ucUartReceive (void);
void vUartTransmitString(char *strData);



#endif /* UART_H_ */