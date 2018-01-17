/*
 * keypadconfig.h
 *
 * Created: 12/4/2017 9:01:21 PM
 *  Author: GeDa
 */ 


#ifndef KEYPADCONFIG_H_
#define KEYPADCONFIG_H_

#include <avr/io.h>

#define ROW0              0
#define ROW0_DIR          DDRA
#define ROW0_DATAOUT      PORTA

#define ROW1              1
#define ROW1_DIR          DDRA
#define ROW1_DATAOUT      PORTA

#define ROW2              2
#define ROW2_DIR          DDRA
#define ROW2_DATAOUT      PORTA

#define ROW3              3
#define ROW3_DIR          DDRA
#define ROW3_DATAOUT      PORTA

#define LINE0             4
#define LINE0_DIR         DDRA
#define LINE0_DATAIN      PINA
#define LINE0_PULLUP      PORTA

#define LINE1             5
#define LINE1_DIR         DDRA
#define LINE1_DATAIN      PINA
#define LINE1_PULLUP      PORTA

#define LINE2             6
#define LINE2_DIR         DDRA
#define LINE2_DATAIN      PINA
#define LINE2_PULLUP      PORTA


#endif /* KEYPADCONFIG_H_ */