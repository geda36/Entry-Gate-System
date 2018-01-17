/*
 * motorconfig.h
 *
 * Created: 1/11/2018 2:20:47 PM
 *  Author: GeDa
 */ 


#ifndef MOTORCONFIG_H_
#define MOTORCONFIG_H_

#include <avr/io.h>

#define CW                    5
#define CW_DIR                DDRD
#define CW_DATAOUT            PORTD

#define ACW                   6
#define ACW_DIR               DDRD
#define ACW_DATAOUT           PORTD

#define OC2                   7
#define OC2_DIR               DDRB
#define OC2_DATAOUT           PORTB


#endif /* MOTORCONFIG_H_ */