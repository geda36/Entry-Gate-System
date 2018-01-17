/*
 * Lcd.h
 *
 * Created: 12/5/2017 5:04:39 AM
 *  Author: GeDa
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "lcdconfig.h"
//function prototypes
void vLcdInit (void);
void vSendCommand(unsigned char cmd) ;
void vSend4Bits(unsigned char cmd_init);
void vSendChar (unsigned char data);
void vSendString (char *str);
void vLcdClear (void);
void vLcdgo_xy (unsigned char row,unsigned char pos);	
void vLcdString_xy (unsigned char row,unsigned char pos,char *str);


#endif /* LCD_H_ */