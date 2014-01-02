/* 
* UTFT_SDimg.h
*
* Created: 02/01/2014 12:19:36
* Author: eNDeR
*/

//Based on the work of Stanley Huang
//https://github.com/stanleyhuangyc/ArduinoTFT

#include <Arduino.h>
#include <UTFT.h>
#include "memorysaver.h"

#if defined(__AVR__)
	#include <Arduino.h>
	#include <HW_AVR_defines.h>
#elif defined(__PIC32MX__)
	#include "WProgram.h"
	#include "HW_PIC32_defines.h"
#elif defined(__arm__)
	#include "Arduino.h"
#include "HW_ARM_defines.h"
#endif

#ifndef UTFT_SDIMG_H
#define UTFT_SDIMG_H


class UTFT_SDimg : public UTFT {

public:
	//Constructors
	UTFT_SDimg();
	UTFT_SDimg(byte model, int RS, int WR, int CS, int RST, int SER);
	UTFT_SDimg(const UTFT_SDimg &other);
	UTFT_SDimg& operator=(const UTFT_SDimg &other);
	
	//Draws a pixel line. Ideal from reading data from SD and displaying
	void drawPixelLine(int x, int y, int sx, uint16_t* data);
};

#endif
