/*
 * Other.h
 *
 * Created: 22/12/2013 17:03:23
 *  Author: eNDeR
 */ 

//TODO: Send to GUI Class?


#ifndef OTHER_H_
#define OTHER_H_

#include <DS1307RTC.h>
#include <Time.h>

void setRTCtime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, int y);


#endif /


// Dew point temp
// delta max = 0.6544 wrt dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
/*double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity/100);
  double Td = (b * temp) / (a - temp);
  return Td;
}*/