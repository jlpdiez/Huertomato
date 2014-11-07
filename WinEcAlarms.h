/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINECALARMS_H_
#define WINECALARMS_H_

#include "Window.h"
/*
	//10-EC Alarms
	*/

const int nECalarmsButtons = 7;
static char* ecAlarmsButtonsText[nECalarmsButtons] = {
	"=",
	">",
	"=",
	">"
};
static int ecAlarmsButtons[nECalarmsButtons];


class WinEcAlarms: public Window {
	public:
		WinEcAlarms(UTFT *lcd, UTouch *touch);
		WinEcAlarms(const WinEcAlarms &other);
		WinEcAlarms& operator=(const WinEcAlarms &other);
		virtual ~WinEcAlarms();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		void print();
};



#endif