/*
 * Other.cpp
 *
 * Created: 22/12/2013 17:21:52
 *  Author: eNDeR
 */ 

#include "Other.h"

void setRTCtime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, int y) {
	tmElements_t t;
	t.Hour = h;
	t.Minute = m;
	t.Second = s;
	t.Day = d;
	t.Month = mo;
	//year argument is offset from 1970
	t.Year = y - 1970;
	time_t time = makeTime(t);
	setTime(time);
	RTC.set(time);
}