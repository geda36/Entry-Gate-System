/*
 * keypad.c
 *
 * Created: 12/4/2017 8:05:05 PM
 *  Author: GeDa
 */ 
#define F_CPU 11059200UL
#include <util/delay.h>
#include "keypad.h"

static const unsigned char g_Keypad_Mapping[4][3] = {{'1','2','3'},
                                                     {'4','5','6'},
													 {'7','8','9'},
													 {' ','0','#'}};

void vKeypadInit(void)
{
	//input 0 >> output 0

	//set rows as output
	ROW0_DIR |=(1U<<ROW0);
	ROW1_DIR |=(1U<<ROW1);
	ROW2_DIR |=(1U<<ROW2);
	ROW3_DIR |=(1U<<ROW3);
	//set lines as input
	LINE0_DIR &=~(1U<<LINE0);
	LINE1_DIR &=~(1U<<LINE1);
	LINE2_DIR &=~(1U<<LINE2);
	//set lines pull up
	LINE0_PULLUP |=(1U<<LINE0);
	LINE1_PULLUP |=(1U<<LINE1);
	LINE2_PULLUP |=(1U<<LINE2);
	//set rows to no output state(1)
	ROW0_DATAOUT |=(NO_OUTPUT_STATE<<ROW0);
	ROW1_DATAOUT |=(NO_OUTPUT_STATE<<ROW1);
	ROW2_DATAOUT |=(NO_OUTPUT_STATE<<ROW2);
	ROW3_DATAOUT |=(NO_OUTPUT_STATE<<ROW3);
}
unsigned char ucKeypadScan(void)
{
	uint8_t row=0;
	uint8_t line= NO_VALID_INPUT;
	unsigned char result = NO_VALID_INPUT;
	//loop on rows
	for (row=0;row<4;row++)
	{
		//set rows to no output state(1)
		ROW0_DATAOUT |=(NO_OUTPUT_STATE<<ROW0);
		ROW1_DATAOUT |=(NO_OUTPUT_STATE<<ROW1);
		ROW2_DATAOUT |=(NO_OUTPUT_STATE<<ROW2);
		ROW3_DATAOUT |=(NO_OUTPUT_STATE<<ROW3);
		//set the numbered row to output state (0)
		switch (row)
		{
			case 0:ROW0_DATAOUT &=~(OUTPUT_STATE<<ROW0);break;
			case 1:ROW1_DATAOUT &=~(OUTPUT_STATE<<ROW1);break;
			case 2:ROW3_DATAOUT &=~(OUTPUT_STATE<<ROW2);break;
			case 3:ROW3_DATAOUT &=~(OUTPUT_STATE<<ROW3);break;
			default:break;
		}
		//check inputs
		if ((LINE0_DATAIN&(1U<<LINE0))== INPUT_STATE){line = 0;}
		else if ((LINE1_DATAIN&(1U<<LINE1))== INPUT_STATE){line = 1;}
		else if ((LINE2_DATAIN&(1U<<LINE2))== INPUT_STATE){line = 2;}
		else {; /*MISRA*/}
		//if there was a valid input
		if (line != NO_VALID_INPUT)
		{
			if ((row==3)&&(line==0))
			{
				result = g_Keypad_Mapping[row][line];
			}
			else
			{
				//wait until the input is gone
				while (INPUT_IS_STILL_VALID == INPUT_STATE);
				result = g_Keypad_Mapping[row][line];
			}
			break;
		}
	}
	return result ;
}