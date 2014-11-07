/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINSD_H_
#define WINSD_H_

#include "Window.h"

//6-SD Card                                                                  

const int nSDcardButtons = 8;
static char* sdCardButtonsText[nSDcardButtons] = {
	"SD Card Log:",
	"=",
	"=",
	">",
	">"
};
static int sdCardButtons[nSDcardButtons];


class WinSD: public Window {
	public:
		WinSD(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSD(const WinSD &other);
		WinSD& operator=(const WinSD &other);
		virtual ~WinSD();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		static boolean _sdActive;
		static uint8_t _sdHour, _sdMin;
		void print();
};



#endif