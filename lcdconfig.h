/*
 * lcdconfig.h
 *
 * Created: 12/5/2017 5:05:11 AM
 *  Author: GeDa
 */ 


#ifndef LCDCONFIG_H_
#define LCDCONFIG_H_

#include <avr/io.h>

#define  Rs          2
#define  Rs_DIR      DDRB
#define  Rs_DATAOUT  PORTB

#define  EN          3
#define  EN_DIR      DDRB
#define  EN_DATAOUT  PORTB

#define  D4          4
#define  D4_DIR      DDRB
#define  D4_DATAOUT  PORTB

#define  D5          5
#define  D5_DIR      DDRB
#define  D5_DATAOUT  PORTB

#define  D6          6
#define  D6_DIR      DDRB
#define  D6_DATAOUT  PORTB

#define  D7          7 
#define  D7_DIR      DDRB
#define  D7_DATAOUT  PORTB


#endif /* LCDCONFIG_H_ */