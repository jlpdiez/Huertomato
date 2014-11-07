/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINPUMP_H_
#define WINPUMP_H_

#include "Window.h"

/*
	//16-Pump Protection
	*/

const int nPumpProtectionButtons = 5;
static char* pumpProtectionButtonsText[nPumpProtectionButtons] = {
	"=",
	">"
};
static int pumpProtectionButtons[nPumpProtectionButtons];


class WinPump: public Window {
	public:
		WinPump(UTFT *lcd, UTouch *touch);
		WinPump(const WinPump &other);
		WinPump& operator=(const WinPump &other);
		virtual ~WinPump();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		void print();
};

#endif