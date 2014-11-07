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
		WinPhAlarms(UTFT *lcd, UTouch *touch);
		WinPhAlarms(const WinPhAlarms &other);
		WinPhAlarms& operator=(const WinPhAlarms &other);
		virtual ~WinPhAlarms();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		void print();
};

#endif