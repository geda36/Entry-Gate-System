/*
* projectconfig.h
*
* Created: 1/10/2018 8:57:06 AM
*  Author: GeDa
*/


#ifndef PROJECTCONFIG_H_
#define PROJECTCONFIG_H_

#include <avr/io.h>

#define BUZZER                2
#define BUZZER_DIR            DDRD	
#define BUZZER_DATAOUT        PORTD

#define RED_LED               3
#define RED_LED_DIR           DDRD
#define RED_LED_DATAOUT       PORTD

#define GREEN_LED             4
#define GREEN_LED_DIR         DDRD
#define GREEN_LED_DATAOUT     PORTD


#endif /* PROJECTCONFIG_H_ */