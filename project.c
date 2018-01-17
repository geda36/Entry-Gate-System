/*
* project.c
*
* Created: 1/3/2018 9:44:46 PM
*  Author: GeDa
*/
#include "project.h"

bool bSystemInit (void)
{
	char input_buffer[4], R_data[16], R_eeprommap[13], Default_Pass[4];
	char chkW = 0, chkR = 0, initial, semsem;
	uint8_t StartADR = 0, EndADR = 13;


	// Check on Previous Cofiguration
	u8EEReadByte(StartADR + 15, &initial);
	if (initial == FIRST_USE)
	{
		// During Configuration
		//LCD
		vSendString("Un-initialized");
		vLcdString_xy(1, 0, "No Member List");
		_delay_ms(500);
		vLcdClear();
		vSendString("Wait For Config");

		//UART
		vUartTransmitString("Initialization EEPROM Memory");

		char eeprommap[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		chkW = cEEWriteMultiByte(eeprommap, StartADR, EndADR);
		chkR = cEEReadMultiByte(R_eeprommap, StartADR, EndADR);
		if (!(memcmp(&chkW, &chkR, 1)))
		{
			vUartTransmitString("\rSearch Array -> Success");
			char superadmin[16] = {'S', '.', 'A', 'd', 'm', 'i', 'n', 0, 0, '0', '1', '0', '0', '0', '0'};
			chkW = cEEWriteMultiByte(superadmin, SA_STARTADR, SA_ENDADR);
			chkR = cEEReadMultiByte(R_data, SA_STARTADR, SA_ENDADR);
			memset(R_data, 0, sizeof R_data);
			if (!(memcmp(&chkW, &chkR, 1)))
			{
				vUartTransmitString("\rDefault SuperAdmin -> Success");

				//Check Usage oF Default Pass
				u8EEReadByte(EndADR + 1, &semsem);
				if (semsem != USED )
				{
					vUartTransmitString("\rEnter Default Pass(0000): ");

					vUserInput(input_buffer,5);
					cEEReadMultiByte(Default_Pass, 27, SA_ENDADR);
					if (!(memcmp(Default_Pass, input_buffer, 4)))
					{
						u8EEWriteByte(EndADR + 1, 1);
						_delay_ms(1);

						vUartTransmitString("\rPlease Enter New Pass: ");

						vUserInput(input_buffer,5);

						vUartTransmitString("\rSaving...");

						chkW = cEEWriteMultiByte(input_buffer, 27, SA_ENDADR);
						chkR = cEEReadMultiByte(R_data, 27, SA_ENDADR);
						memset(R_data, 0, sizeof R_data);
						if (!(memcmp(&chkW, &chkR, 1)))
						{
							vUartTransmitString("\rSaved");
							vUartTransmitString("\rSuperAdmin ID is 01");

							//checksum after new superadmin password
							chkR = checksum(SA_STARTADR, SA_ENDADR);
							u8EEWriteByte(31, chkR);
							_delay_ms(1);

							bool isDone = false;
							while (!isDone)
							{
								vUartTransmitString("\rEnter Admin Data -->");
								char admin[16] = {'A', 'd', 'm', 'i', 'n', 0, 0, 0, 0, '0', '2'};
								uint8_t i = 11;

								vUartTransmitString("\rEnter Admin Pass: ");

								do
								{
									admin[i] = ucUartReceive();
								} while ( admin[i++] != 0x0D );
								admin[--i] = '\0';

								vUartTransmitString("\rSaving...");

								chkW = cEEWriteMultiByte(admin, A_STARTADR, A_ENDADR);
								u8EEWriteByte(47, chkW);
								_delay_ms(1);

								chkR = cEEReadMultiByte(R_data, 32, 47);
								memset(R_data, 0, 16);
								if (!(memcmp(&chkW, &chkR, 1)))
								{

									vUartTransmitString("\rSaved");
									vUartTransmitString("\rAdmin ID is 02");
									//Complete Configuration Flag
									u8EEWriteByte(EndADR + 2, 1);
									_delay_ms(1);
									isDone = true;
								}
								else
								{
									vUartTransmitString("\rAdmin Failed,checksum mismatched");
								}
							}
						}
						else
						{
							vUartTransmitString("\rSuperAdmin New Pass Failed,checksum mismatched");
						}
					}
					else
					{
						vUartTransmitString("\rWrong Default Pass");
					}
				}
				else
				{
					vUartTransmitString("\rSystem Locked , Please Contact GeDa To Unlock System");
					return false;
				}
			}
			else
			{
				vUartTransmitString("\rDefault SuperAdmin Failed,checksum mismatched");
				return false;
			}
		}
		else
		{
			vUartTransmitString("\rSearch Array Failed,checksum mismatched");
			return false;
		}
	}

	return true;
}

void vUserInput (char* input_buffer, uint8_t max)
{
	uint8_t byte=0;
	do
	{
		*input_buffer = ucUartReceive();
		byte++;
	} while ((*input_buffer++ != 0x0D) && (byte < max));
	if (byte <= max)
	{
	  *(--input_buffer) = '\0';
	}
}

void vAddUserData (char* W_data, char* ID, uint8_t freepos)
{
	char user_info[16] = {0};
	vUartTransmitString("\rEnter Name: ");

	vUserInput(&user_info[0],10);

	if (freepos < 10)
	{
		user_info[9] = *ID++ = '0';
		user_info[10] = *ID++ = (freepos + '0');
		*ID = '\0';
	}
	else
	{
		user_info[9] = *ID++ = '1';
		user_info[10] = *ID++ = ((freepos - 10) + '0');
		*ID = '\0';
	}
	vUartTransmitString("\rEnter Pass: ");
	
	vUserInput(&user_info[11],5);
	
	memcpy(W_data,user_info,16);
}

void vEditUserData (char* W_data, char* ID)
{
	char user_info[16] = {0};
	vUartTransmitString("\rEnter Name: ");

	vUserInput(&user_info[0],10);

	user_info[9] = *ID++;
	user_info[10] = *ID++;

	vUartTransmitString("\rEnter Pass: ");

	vUserInput(&user_info[11],5);

	memcpy(W_data,user_info,16);

}

void vAddUser (void)
{
	char W_data[16], R_data[16], R_eeprommap[13], ID[3];
	char chkW = 0, chkR = 0;
	uint8_t freepos = 0, StartADR = 0, EndADR = 13;

	cEEReadMultiByte(R_eeprommap, StartADR, EndADR);

	while ((R_eeprommap[freepos] != 0) && (freepos < 13))
	{
		freepos++;
	}
	if (freepos >= 13)
	{
		vUartTransmitString("\rEEPROM is full");
	}
	else
	{
		freepos = freepos + 3;
		StartADR = (freepos * 16);
		EndADR = (((freepos + 1) * 16) - 1);

		vAddUserData (W_data, ID, freepos);

		vUartTransmitString("\rSaving");

		//Write
		chkW = cEEWriteMultiByte(W_data, StartADR, EndADR);
		u8EEWriteByte(StartADR + 15, chkW);
		_delay_ms(1);
		
		chkR = cEEReadMultiByte(R_data, StartADR, EndADR);

		if (!(memcmp(&chkW, &chkR, 1)))
		{

			vUartTransmitString("\rSaved");
			u8EEWriteByte(freepos - 3, 1);
			_delay_ms(1);
			vUartTransmitString("\rYour ID is ");
			vUartTransmitString(&ID[0]);
		}
		else
		{
			vUartTransmitString("\rSaving Failed, checksum mismatched");
		}
	}
}

void vEditUser (void)
{
	char W_data[16], R_data[16], ID[3], choice[2];
	char chkW = 0, chkR = 0;
	uint8_t i = 0, IDs=0, StartADR = 0, EndADR = 13;
	bool isfound=false;
	
	vUartTransmitString("\r1-Using Specific ID\r2-List All Users");
	vUartTransmitString("\r\rYour Choice: ");
	
    i=0;
	do
	{
		choice[i] = ucUartReceive();
	} while ( choice[i++] != 0x0D );
	choice[--i] = '\0';
	switch (choice[0])
	{
		case '1':
		vUartTransmitString("\rEnter ID: ");
		vUserInput(ID,3);
		if (ID[0] == '0')
		{
			IDs = (ID[1] - '0');
			StartADR = (IDs * 16);
			EndADR = (((IDs + 1) * 16) - 1);
		}
		else if (ID[0] == '1')
		{
			IDs = (10 + (ID[1] - '0'));
			StartADR = (IDs * 16);
			EndADR = (((IDs + 1) * 16) - 1);
		}
		if (!(bCheckUserStatus(IDs)))
		{
			vUartTransmitString("User Doesn't Exist");
		}
		else
		{
			vEditUserData (W_data, ID);
			vUartTransmitString("\rSaving");

			//Write
			chkW = cEEWriteMultiByte(W_data, StartADR, EndADR);
			u8EEWriteByte(StartADR + 15, chkW);
			_delay_ms(1);

			chkR = cEEReadMultiByte(R_data, StartADR, EndADR);

			if (!(memcmp(&chkW, &chkR, 1)))
			{
				vUartTransmitString("\rSaved");
			}
			else
			{
				vUartTransmitString("\rSaving Failed, checksum mismatched");
			}
		}
		break;

		case '2':
		isfound = u8ListUsers();
		if (!isfound)
		{
			vUartTransmitString("\rNo Users Found\r");
		}
		else
		{
			vUartTransmitString("\rChoose an ID: ");
		    vUserInput(ID,3);
			if (ID[0] == '0')
			{
				IDs = (ID[1] - '0');
				StartADR = (IDs * 16);
				EndADR = (((IDs + 1) * 16) - 1);
			}
			else if (ID[0] == '1')
			{
				IDs = (10 + (ID[1] - '0'));
				StartADR = (IDs * 16);
				EndADR = (((IDs + 1) * 16) - 1);
			}
			
			if (!(bCheckUserStatus(IDs)))
			{
				vUartTransmitString("User Doesn't Exist");
			}
			else
			{
			    vEditUserData (W_data, ID);
			    vUartTransmitString("\rSaving");

			    //Write
			    chkW = cEEWriteMultiByte(W_data, StartADR, EndADR);
				u8EEWriteByte(StartADR + 15, chkW);
				_delay_ms(1);

				chkR = cEEReadMultiByte(R_data, StartADR, EndADR);

				if (!(memcmp(&chkW, &chkR, 1)))
				{
					vUartTransmitString("\rSaved");
				}
				else
				{
					vUartTransmitString("\rSaving Failed, checksum mismatched");
				}
			}
		}
		default:
		vUartTransmitString("\rInvalid Choice\r");
		break;
	}
}

bool u8ListUsers (void)
{
	char R_data[16], R_eeprommap[13];
	char chkR = 0, chkS = 0;
	uint8_t i = 0, StartADR = 0, EndADR = 13;
	signed char usedpos = -1;
	bool isfound=false;

	vUartTransmit('\r');
	cEEReadMultiByte(R_eeprommap, StartADR, EndADR);
	while ((R_eeprommap[++usedpos] == 1) && (usedpos < 13))
	{
		isfound=true;
		StartADR = ((usedpos+3) * 16);
		EndADR = ((((usedpos+3) + 1) * 16) - 1);
		//Read
		chkR = cEEReadMultiByte(R_data, StartADR, EndADR);
		u8EEReadByte(EndADR, &chkS);
		if (!(memcmp(&chkR, &chkS, 1)))
		{
			for (i = 9; i < 11; i++)
			{
				vUartTransmit(R_data[i]);
			}
			vUartTransmitString(". ");
			for (i = 0; i < 9; i++)
			{
				vUartTransmit(R_data[i]);
			}
		}
		else
		{
			vUartTransmitString("\rReading Failed, checksum mismatched");
		}
		vUartTransmit('\r');
	}
	return isfound;
}

void vDeleteUser(void)
{
	char ID[3], choice[2], chkdel = 2;
	uint8_t i = 0, IDs = 0;
	bool isfound=false;

	vUartTransmitString("\r1-Using Specific ID\r2-List All Users");
	vUartTransmitString("\r\rYour Choice: ");
	
    i=0;
	do
	{
		choice[i] = ucUartReceive();
	} while ( choice[i++] != 0x0D );
	choice[--i] = '\0';
	
	switch (choice[0])
	{
		case '1':
		vUartTransmitString("\rEnter ID: ");
		vUserInput(ID,3);
		if (ID[0] == '0')
		{
			IDs = (ID[1] - '0');
		}
		else if (ID[0] == '1')
		{
			IDs = (10 + (ID[1] - '0'));
		}
		if (!(bCheckUserStatus(IDs)))
		{
			vUartTransmitString("User Doesn't Exist");
		}
		else
		{
			u8EEWriteByte(IDs - 3, 0);
			_delay_ms(1);
			u8EEReadByte(IDs - 3, &chkdel);
			
			if (chkdel == 0)
			vUartTransmitString("Deleted");
			else if (chkdel == 1)
			vUartTransmitString("Not Deleted");
			else
			vUartTransmitString("ERROR");
		}
		break;
		
		case '2':
		isfound = u8ListUsers();
		if (!isfound)
		{
			vUartTransmitString("\rNo Users Found\r");
		}
		else
		{
			vUartTransmitString("\rChoose an ID: ");
            vUserInput(ID,3);  
			if (ID[0] == '0')
			{
				IDs = (ID[1] - '0');
			}
			else if (ID[0] == '1')
			{
				IDs = (10 + (ID[1] - '0'));
			}
			if (!(bCheckUserStatus(IDs)))
			{
				vUartTransmitString("User Doesn't Exist");
			}
			else
			{
				u8EEWriteByte(IDs - 3, 0);
				_delay_ms(1);
				u8EEReadByte(IDs - 3, &chkdel);

				if (chkdel == 0)
				vUartTransmitString("Deleted");
				else if (chkdel == 1)
				vUartTransmitString("Not Deleted");
				else
				vUartTransmitString("ERROR");
			}
		}
		break;
		
		default :
		vUartTransmitString("\rInvalid Choice\r");
		break;
	}
}

bool bCheckUserStatus (uint8_t IDs)
{
	char R_eeprommap[13];
	uint8_t StartADR = 0, EndADR = 13;
	cEEReadMultiByte(R_eeprommap, StartADR, EndADR);
	if (R_eeprommap[IDs-3] == 0)
	return false;
	else
	return true;
}

uint8_t u8Search (char* user_ID, char* user_pass, char* Name_saved)
{
	uint8_t IDs=0, StartADR, EndADR;
	char Pass_saved[4];
	
	if (*user_ID++ == '0')
	{
		IDs = (*user_ID - '0');
	}
	else if (*user_ID++ == '1')
	{
		IDs = (10 + (*user_ID - '0'));
	}
	if (!(bCheckUserStatus(IDs)))
	{
		return 1;
	}
	else
	{
		StartADR = (IDs * 16);
		EndADR = (((IDs + 1) * 16) -1);
		cEEReadMultiByte(Pass_saved, StartADR + 11, EndADR);
		if (!(memcmp(Pass_saved, user_pass, 4)))
		{
			cEEReadMultiByte(Name_saved, StartADR, StartADR+9);
			return 0;
		}
		else
		{
			return 2;
		}
	}
}

void Logging(char* data1, char* data2)
{
	char hour=0 ,min=0 ,sec=0 ,day=0 ,month=0 ,year=0;
	char time[9], Date[9];
	
	vRTCGetTime(&hour,&min,&sec);
	vRTCGetDate(&day,&month,&year);
	
	time [0] = (((hour & Hour10_Mask)>>4) + '0');
	time [1] = ((hour & Hour_Mask) + '0');
	time [2] = ':';
	time [3] = (((min & Min10_Mask)>>4) + '0');
	time [4] = ((min & Min_Mask) + '0');
	time [5] = ':';
	time [6] = (((sec & Sec10_Mask)>>4) + '0');
	time [7] = ((sec & Sec_Mask) + '0');
	time [8] = '\0';
	
	Date [0] = (((day & Day10_Mask)>>4) + '0');
	Date [1] = ((day & Day_Mask) + '0');
	Date [2] = ':';
	Date [3] = (((month & Month10_Mask)>>4) + '0');
	Date [4] = ((month & Month_Mask) + '0');
	Date [5] = ':';
	Date [6] = (((year & Year10_Mask)>>4) + '0');
	Date [7] = ((year & Year_Mask) + '0');
	Date [8] = '\0';
	

	vUartTransmitString(data1);
	vUartTransmitString(" (ID:");
	vUartTransmitString(data2);
	vUartTransmitString(") logged ");
	vUartTransmitString(time);
	vUartTransmitString(" | ");
	vUartTransmitString(Date);
	vUartTransmit('\r');
}







