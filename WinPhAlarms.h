/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINPHALARMS_H_
#define WINPHALARMS_H_

#include "Window.h"

/*
	//9-pH Alarms
	*/


const int nPHalarmsButtons = 7;
static char* phAlarmsButtonsText[nPHalarmsButtons] = {
	"=",
	">",
	"=",
	">"
};
static int phAlarmsButtons[nPHalarmsButtons];


class WinPhAlarms: public Window {
	public:
		WinPhAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinPhAlarms(const WinPhAlarms &other);
		WinPhAlarms& operator=(const WinPhAlarms &other);
		virtual ~WinPhAlarms();
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		float _phAlarmMax, _phAlarmMin;
		void print();
};

#endif