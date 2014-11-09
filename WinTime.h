/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINTIME_H_
#define WINTIME_H_

#include "Window.h"
#include <Time.h>

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
		WinTime(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinTime(const WinTime &other);
		WinTime& operator=(const WinTime &other);
		virtual ~WinTime();
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		uint8_t _sysHour, _sysMin, _sysSec, _sysDay, _sysMonth;
		int _sysYear;
		void print();
};

#endif