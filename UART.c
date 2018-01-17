/*
* UART.c
*
* Created: 12/25/2017 4:16:43 PM
*  Author: GeDa
*/



#include "UART.h"

#define BAUDRATE 9600                                       //define baud rate
#define F_CPU 11059200UL
#define UBRRVALUE ((F_CPU)/(BAUDRATE*16UL)-1)               //set baud rate value for UBRR

//UART Initialization
void vUartInit(void)
{
	TXD_DIR |= (1U << TXD);
	RXD_DIR &= ~(1U << RXD);
	RXD_PULLUP |= (1U << RXD);
	
	UBRRH = (UBRRVALUE >> 8);
	UBRRL = UBRRVALUE;
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);            // 8bit data format
	UCSRB |= (1<<TXEN)|(1<<RXEN)|(1<<RXCIE);              // enable transmiter, receiver and receiver interrupt
}

//UART Transmitter
void vUartTransmit (char data)
{
	while((UCSRA & (1<<UDRE))==0);
	UDR=data;
}

//UART RECEVIER
unsigned char ucUartReceive (void)
{
	while(!((UCSRA) & (1<<RXC)));
	return UDR;
}

void vUartTransmitString(char *strData)
{
	while(*strData!='\0')
	{
		vUartTransmit(*strData);
		strData++;
	}
	vUartTransmit('\0');
	return;

}