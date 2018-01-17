/*
* project.h
*
* Created: 1/3/2018 9:45:52 PM
*  Author: GeDa
*/


#ifndef PROJECT_H_
#define PROJECT_H_

#define F_CPU 11059200UL
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "projectconfig.h"
#include "UART.h"
#include "eeprom.h"
#include "I2C_MTSR.h"
#include "Lcd.h"
#include "keypad.h"
#include "RTC.h"
#include "motor.h"

#define FIRST_USE      0xFF
#define USED           1U
#define SA_STARTADR    16U //SuperAdmin
#define SA_ENDADR      31U
#define A_STARTADR     32U //Admin
#define A_ENDADR       47U
#define Hour10_Mask    0x10
#define Hour_Mask      0x0F
#define Min10_Mask     0x70
#define Min_Mask       0x0F
#define Sec10_Mask     0x70
#define Sec_Mask       0x0F
#define Day10_Mask     0x30
#define Day_Mask       0x0F
#define Month10_Mask   0x10
#define Month_Mask     0x0F
#define Year10_Mask    0xF0
#define Year_Mask      0x0F
#define KEYPADOFF      0x70
#define KEYPADON       0x0F


bool bSystemInit (void);
void vUserInput (char* input_buffer, uint8_t max);
void vAddUserData (char* W_data, char* ID, uint8_t freepos);
void vEditUserData (char* W_data, char* ID);
void vAddUser (void);
void vEditUser (void);
bool u8ListUsers (void);
void vDeleteUser(void);
bool bCheckUserStatus (uint8_t IDs);
uint8_t u8Search (char* user_ID, char* user_pass, char* Name_saved);
void Logging(char* data1, char* data2);


#endif /* PROJECT_H_ */