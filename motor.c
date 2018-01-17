/*
 * motor.c
 *
 * Created: 1/10/2018 8:50:52 AM
 *  Author: GeDa
 */ 
#include "motorconfig.h"

void vMotorInit(void)
{
	//timer configurations
	//timer mode : Phase-Correct-PWM
	TCCR2 |=  (1U<<WGM20);
	TCCR2 &=~ (1U<<WGM21);
	//duty = OCR/TOP1
	OCR2 = 128; //50%
	//output mode : Non-Inverting
	TCCR2 &=~ (1U<<COM20);
	TCCR2 |= (1U<<COM21);
	//set CW, ACW, OC2 as output
	CW_DIR  |= (1U<<CW);
	ACW_DIR |= (1U<<ACW);
	OC2_DIR |= (1U<<OC2);
	//prescaler : \64
	//PWM Freq. = F-Clock / 64
	TCCR2 &=~ (1U<<CS20);
	TCCR2 &=~ (1U<<CS21);
	TCCR2 |=  (1U<<CS22);	
}

void vMotorOnCW (void)
{
	CW_DATAOUT  &=~ (1U<<CW);
	ACW_DATAOUT &=~ (1U<<ACW);
	
	CW_DATAOUT  |=  (1U<<CW);
	ACW_DATAOUT &=~ (1U<<ACW);
	OC2_DATAOUT |= (1U<<OC2);
}

void vMotorOnACW (void)
{
	CW_DATAOUT  &=~ (1U<<CW);
	ACW_DATAOUT &=~ (1U<<ACW);
	
	CW_DATAOUT  &=~ (1U<<CW);
	ACW_DATAOUT |=  (1U<<ACW);
	OC2_DATAOUT |=  (1U<<OC2);
}

void vMotorOff (void)
{
   CW_DATAOUT  &=~ (1U<<CW);
   ACW_DATAOUT &=~ (1U<<ACW);
   OC2_DATAOUT &=~  (1U<<OC2);
}