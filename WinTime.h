/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINTIME_H_
#define WINTIME_H_

#include "Window.h"

//4-Time & Date

//These buttons use symbol font hence the strange texts
const int nTimeButtons = 15;
static char* timeButtonText[nTimeButtons] = {
	"=",
	"=",
	"=",
	">",
	">",
	">",
	"=",
	"=",
	"=",
	">",
	">",
	">"
};
static int timeButtons[nTimeButtons];


class WinTime: public Window {
	public:
		WinTime(UTFT *lcd, UTouch *touch);
		WinTime(const WinTime &other);
		WinTime& operator=(const WinTime &other);
		virtual ~WinTime();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		void print();
};

#endif