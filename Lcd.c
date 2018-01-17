/*
* Lcd.c
*
* Created: 12/5/2017 5:04:08 AM
*  Author: GeDa
*/

#include "Lcd.h"
#define F_CPU 11059200UL
#include <util/delay.h>

// LCD Initialization
void vLcdInit(void)
{
	// set all pins as output
	Rs_DIR |= (1U<<Rs);
	EN_DIR |= (1U<<EN);
	D4_DIR |= (1U<<D4);
	D5_DIR |= (1U<<D5);
	D6_DIR |= (1U<<D6);
	D7_DIR |= (1U<<D7);
	// set all output pins to zeros
	Rs_DATAOUT &=~(1U<<Rs);
	EN_DATAOUT &=~(1U<<EN);
	D4_DATAOUT &=~(1U<<D4);
	D5_DATAOUT &=~(1U<<D5);
	D6_DATAOUT &=~(1U<<D6);
	D7_DATAOUT &=~(1U<<D7);
	
	_delay_ms(20);
	// reset lcd
	vSendCommand(0x03);
	_delay_ms(5);
	vSendCommand(0x03);
	_delay_ms(1);
	vSendCommand(0x03);
	vSendCommand(0x02);
	// set 4 bit mode , 2 lines , 5*8 font size
	vSendCommand(0x28);
	// display off
	vSendCommand(0x08);
	// display clear
	vSendCommand(0x01);
	//entry mode set(increment & without text)
	vSendCommand(0x06);
	//Display on , cursor on, blinking cursor on
	vSendCommand(0x0F);
	
	
}

void vSendCommand(unsigned char cmd)
{

	D4_DATAOUT &=~(1U<<D4);
	D5_DATAOUT &=~(1U<<D5);
	D6_DATAOUT &=~(1U<<D6);
	D7_DATAOUT &=~(1U<<D7);

	D4_DATAOUT |= ((1U<<D4) & cmd);
	D5_DATAOUT |= ((1U<<D5) & cmd);
	D6_DATAOUT |= ((1U<<D6) & cmd);
	D7_DATAOUT |= ((1U<<D7) & cmd);
	
	Rs_DATAOUT &=~(1U<<Rs);
	EN_DATAOUT  |= (1U<<EN);
	_delay_ms(1);
	EN_DATAOUT &=~ (1U<<EN);
	_delay_ms(1);


	D4_DATAOUT &=~(1U<<D4);
	D5_DATAOUT &=~(1U<<D5);
	D6_DATAOUT &=~(1U<<D6);
	D7_DATAOUT &=~(1U<<D7);
	
	D4_DATAOUT |= ((1U<<D4) & (cmd<<4));
	D5_DATAOUT |= ((1U<<D5) & (cmd<<4));
	D6_DATAOUT |= ((1U<<D6) & (cmd<<4));
	D7_DATAOUT |= ((1U<<D7) & (cmd<<4));
	
	EN_DATAOUT  |= (1U<<EN);
	_delay_ms(1);
	EN_DATAOUT &=~ (1U<<EN);
	_delay_ms(1);

}

// send data to the lcdvoid vSendChar (unsigned char data)
{
	
	D4_DATAOUT &=~(1U<<D4);
	D5_DATAOUT &=~(1U<<D5);
	D6_DATAOUT &=~(1U<<D6);
	D7_DATAOUT &=~(1U<<D7);
	
	D4_DATAOUT |= ((1U<<D4) & data);
	D5_DATAOUT |= ((1U<<D5) & data);
	D6_DATAOUT |= ((1U<<D6) & data);
	D7_DATAOUT |= ((1U<<D7) & data);
	
	Rs_DATAOUT |= (1U<<Rs);
	EN_DATAOUT  |= (1U<<EN);
	_delay_ms(1);
	EN_DATAOUT &=~ (1U<<EN);
	_delay_ms(1);


	D4_DATAOUT &=~(1U<<D4);
	D5_DATAOUT &=~(1U<<D5);
	D6_DATAOUT &=~(1U<<D6);
	D7_DATAOUT &=~(1U<<D7);
	
	D4_DATAOUT |= ((1U<<D4) & (data<<4));
	D5_DATAOUT |= ((1U<<D5) & (data<<4));
	D6_DATAOUT |= ((1U<<D6) & (data<<4));
	D7_DATAOUT |= ((1U<<D7) & (data<<4));
	
	EN_DATAOUT  |= (1U<<EN);
	_delay_ms(1);
	EN_DATAOUT &=~ (1U<<EN);
	_delay_ms(1);
}

// send string to the lcd
void vSendString (char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)		// Send each char of string till the NULL
	{
		vSendChar(str[i]);
	}
}

// clear lcd
void vLcdClear (void)
{
	vSendCommand (0x01);		// Clear display
	_delay_ms(2);
	vSendCommand (0x80);		// Cursor at home position
}

// Send string to LCD with xy position
void vLcdgo_xy (unsigned char row,unsigned char pos)
{
	if (row == 0 && pos<16)
	vSendCommand((pos & 0x0F)|0x80);	// Command of first row and required position<16
	else if (row == 1 && pos<16)
	vSendCommand((pos & 0x0F)|0xC0);	// Command of second row and required position<16
}

void vLcdString_xy (unsigned char row,unsigned char pos,char *str)
{
	if (row == 0 && pos<16)
	vSendCommand((pos & 0x0F)|0x80);	// Command of first row and required position<16
	else if (row == 1 && pos<16)
	vSendCommand((pos & 0x0F)|0xC0);	// Command of second row and required position<16
	vSendString(str);
}