/*
 * keypad.h
 *
 * Created: 12/4/2017 8:06:50 PM
 *  Author: GeDa
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "keypadconfig.h"
//function prototypes
void vKeypadInit(void);
unsigned char ucKeypadScan(void);

//macros
#define INPUT_STATE  0U
#define OUTPUT_STATE 1U
#define NO_OUTPUT_STATE 1U
#define NO_VALID_INPUT 0xFF

#define INPUT_IS_STILL_VALID    ((LINE0_DATAIN&(1U<<LINE0))\
                              &&(LINE1_DATAIN&(1U<<LINE1))\
							  &&(LINE2_DATAIN&(1U<<LINE2)))
#define LONG_PRESS_KEY       (LINE2_DATAIN&(1U<<LINE2))

#endif /* KEYPAD_H_ */