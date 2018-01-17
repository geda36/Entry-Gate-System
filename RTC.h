/*
 * RTC.h
 *
 * Created: 12/28/2017 12:51:32 AM
 *  Author: GeDa
 */ 


#ifndef RTC_H_
#define RTC_H_

#include <inttypes.h>

void vRTCGetTime (char* hour, char* min, char* sec);
void vRTCGetDate (char* day, char* month, char* year);


#endif /* RTC_H_ */