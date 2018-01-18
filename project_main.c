/*
project_main.c

Created: 1/3/2018 9:45:37 PM
Author: GeDa
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "project.h"
volatile static bool isReset = false;
int main(void)
{
	RED_LED_DIR |= (1U<<RED_LED);
	GREEN_LED_DIR |= (1U<<GREEN_LED);
	BUZZER_DIR |= (1U<<BUZZER);
	vUartInit();
	vLcdInit();
	vKeypadInit();
	vI2CInit(100000UL);
	vMotorInit();

	if(bSystemInit() == false)
	return 0;
	

	sei();
	while (1)
	{
		isReset=false;
		RED_LED_DATAOUT &=~ (1U<<RED_LED);
		char Name_saved[9]={0};
		char user_ID[3]={0};
		char user_pass[5]={0};
		unsigned char pressed_key = NO_VALID_INPUT;
		uint8_t counter=0;
		vLcdClear();
		vSendString("Welcome");
		while(!((pressed_key != NO_VALID_INPUT)&&(pressed_key == '#')))
		{
			pressed_key = ucKeypadScan() ;
		}
		bool isCorrect=false;
		while((!isCorrect) && (!isReset))
		{
			uint8_t k=0,j=0,num_char=0;
			vLcdClear();
			if (counter > 0)
			{
				vLcdgo_xy(0,15);
				vSendChar((counter+'0'));
			}
			vLcdgo_xy(0,0);
			vSendString("ID:");
			num_char+=3;
			while(1 && (!isReset))
			{
				pressed_key = ucKeypadScan() ;
				if (pressed_key != NO_VALID_INPUT)
				{
					if ((pressed_key == ' ')&&(num_char>3))
					{
						--j;
						vLcdgo_xy(0,--num_char);
						vSendChar(' ');
						vLcdgo_xy(0,num_char);
						_delay_ms(275);
					}
					else if ((pressed_key == '#')&&(num_char==5))
					{
						user_ID[j]='\0';
						break;
					}
					else if ((pressed_key != ' ')&&(pressed_key != '#'))
					{
						num_char++;
						vSendChar(pressed_key);
						user_ID[j++]=pressed_key;
					}
				}
			}
			vLcdString_xy(1,0,"Pass:");
			num_char=0;
			num_char+=5;
			while(1 && (!isReset))
			{
				
				pressed_key = ucKeypadScan() ;
				if (pressed_key != NO_VALID_INPUT)
				{
					if ((pressed_key == ' ')&&(num_char>5))
					{
						--k;
						vLcdgo_xy(1,--num_char);
						vSendChar(' ');
						vLcdgo_xy(1,num_char);
						_delay_ms(275);
					}
					else if ((pressed_key == '#')&&(num_char==9))
					{
						user_pass[k]='\0';
						break;
					}
					else if ((pressed_key != ' ')&&(pressed_key != '#'))
					{
						num_char++;
						vSendChar(pressed_key);
						_delay_ms(200);
						vLcdString_xy(1,num_char-1,"*");
						user_pass[k++]=pressed_key;
					}
				}
			}
			if(!isReset)
			{
				uint8_t status=0;
				status=u8Search(user_ID,user_pass,Name_saved);
				if (status == 2)
				{
					if (counter < 2)
					{
						counter++;
						vLcdClear();
						vSendString("Wrong Entry");
						_delay_ms(500);
						vUartTransmitString("Pass: ");
						vLogging(user_pass,"not found");
					}
					else
					{
						vUartTransmitString("Pass: ");
						vLogging(user_pass,"not found");
						BUZZER_DATAOUT |= (1U<<BUZZER);
						PORTA = KEYPADOFF;
						bool isAllowed=false;
						while ((!isAllowed) && (!isReset))
						{
							RED_LED_DATAOUT &=~ (1U<<RED_LED);
							vLcdClear();
							_delay_ms(500);
							vSendString("Access Denied");
							RED_LED_DATAOUT |=(1U<<RED_LED);
							_delay_ms(500);
						}
					}
				}
				else if (status == 1)
				{
					vLcdClear();
					vSendString("User Doesn't");
					vLcdString_xy(1,0,"Exist");
					isCorrect=true;
					_delay_ms(1000);
				}
				else
				{
					vLcdClear();
					vSendString("Welcome");
					vLcdString_xy(1,6,Name_saved);
					vLogging(Name_saved,user_ID);
					GREEN_LED_DATAOUT |= (1U<<GREEN_LED);
					vMotorOnACW();
					_delay_ms(1000);
					vMotorOff();
					_delay_ms(1000);
					vMotorOnCW();
					_delay_ms(1000);
					vMotorOff();
					GREEN_LED_DATAOUT &=~ (1U<<GREEN_LED);
					isCorrect=true;
				}
			}
		}
	}
	return 0;
}

ISR(USART_RXC_vect)
{
	if (UDR == '@')
	{
		char input_buffer[4], Pass_saved[4], choice[2];
		uint8_t i = 0;
		bool isDone = false, isDone1 = false, isfound=false;

		vUartTransmitString("\rEnter SuperAdmin/Admin Password: ");

		vUserInput(input_buffer,5);

		cEEReadMultiByte(Pass_saved, A_STARTADR + 11, A_ENDADR);
		if (!(memcmp(Pass_saved, input_buffer, 4)))
		{

			while (!isDone)
			{
				vUartTransmitString("\rAdmin Privileges: \r1- Reset System\r2- Open Door\r3- Close Door\r4- Add/Remove User (SuperAdmin Pass Required)\r5- Exit");
				vUartTransmitString("\r\rYour Choice: ");
				
				i=0;
				do
				{
					choice[i] = ucUartReceive();
				} while ( choice[i++] != 0x0D );
				choice[--i] = '\0';

				switch (choice[0])
				{
					case '1' :
					isReset=true;
					PORTA = KEYPADON;
					break;
					
					case '2' :
					vMotorOnACW();
					_delay_ms(1000);
					vMotorOff();
					break;
					
					case '3' :
					vMotorOnCW();
					_delay_ms(1000);
					vMotorOff();
					break;
					
					case '4' :
					vUartTransmitString("\rEnter SuperAdmin Password: ");

					vUserInput(input_buffer,5);

					cEEReadMultiByte(Pass_saved, SA_STARTADR + 11, SA_ENDADR);
					if (!(memcmp(Pass_saved, input_buffer, 4)))
					{
						while (!isDone1)
						{
							vUartTransmitString("\rSuperAdmin Privileges: \r1- Add User\r2- Modify User\r3- List Users\r4- Remove User\r5- Return To Previous Menu");
							vUartTransmitString("\r\rYour Choice: ");

							i = 0;
							do
							{
								choice[i] = ucUartReceive();
							} while ( choice[i++] != 0x0D );
							choice[--i] = '\0';

							switch (choice[0])
							{
								case '1':
								vAddUser();
								break;
								
								case '2' :
								vEditUser();
								break;
								
								case '3' :
								isfound=u8ListUsers();
								if(!isfound)
								{
									vUartTransmitString("\rNo Users Found\r");
								}
								break;
								
								case '4' :
								vDeleteUser();
								break;
								
								case '5' :
								isDone1 = true;
								break;
								
								default:
								vUartTransmitString("\rInvalid Choice\r");
								break;
							}
						}
					}
					else
					{
						vUartTransmitString("\rWrong SuperAdmin Password");
					}
					break;
					
					case '5':
					isDone = true;
					break;
					
					default:
					vUartTransmitString("\rInvalid Choice\r");
					break;
				}
			}
		}
		else
		{
			cEEReadMultiByte(Pass_saved, SA_STARTADR + 11, SA_ENDADR);
			if (!(memcmp(Pass_saved, input_buffer, 4)))
			{
				while (!isDone)
				{
					vUartTransmitString("\rSuperAdmin Privileges:\r1- Reset System\r2- Open Door\r3- Close Door\r4- Add/Remove User (SuperAdmin Pass Required)\r5- Exit");
					vUartTransmitString("\r\rYour Choice: ");
					
					i=0;
					do
					{
						choice[i] = ucUartReceive();
					} while ( choice[i++] != 0x0D );
					choice[--i] = '\0';

					switch (choice[0])
					{
						case '1' :
						isReset=true;
						PORTA = KEYPADON;
						break;
						
						case '2' :
						vMotorOnACW();
						_delay_ms(1000);
						vMotorOff();
						break;
						
						case '3' :
						vMotorOnCW();
						_delay_ms(1000);
						vMotorOff();
						break;
						
						case '4' :
						while (!isDone1)
						{
							vUartTransmitString("\rSuperAdmin Privileges: \r1- Add User\r2- Modify User\r3- List Users\r4- Remove User\r5- Return To Previous Menu");
							vUartTransmitString("\r\rYour Choice: ");

							i = 0;
							do
							{
								choice[i] = ucUartReceive();
							} while ( choice[i++] != 0x0D );
							choice[--i] = '\0';

							switch (choice[0])
							{
								case '1':
								vAddUser();
								break;
								
								case '2' :
								vEditUser();
								break;
								
								case '3' :
								isfound=u8ListUsers();
								if(!isfound)
								{
									vUartTransmitString("\rNo Users Found\r");
								}
								break;
								
								case '4' :
								vDeleteUser();
								break;
								
								case '5' :
								isDone1 = true;
								break;
								
								default:
								vUartTransmitString("\rInvalid Choice\r");
								break;
							}
						}
						break;
						
						case '5' :
						isDone = true;
						break;
						
						default:
						vUartTransmitString("\rInvalid Choice\r");
						break;
					}
				}
			}
			else
			{
				vUartTransmitString("\rInvalid Password , Access Denied");
			}
		}
	}
}

