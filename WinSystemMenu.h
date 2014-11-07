/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINSYSTEMMENU_H_
#define WINSYSTEMMENU_H_

#include "Window.h"

//2-System Settings

const int nSystemButtons = 8;
static char* systemButtonText[nSystemButtons] = {
	"Watering Cycle",
	"Sensor Alarms",
	"Calibration",
	"Pump protection",
	"Watering at Night:"
};
static int systemButtons[nSystemButtons];


class WinSystemMenu: public Window {
	public:
		WinSystemMenu(UTFT *lcd, UTouch *touch);
		WinSystemMenu(const WinSystemMenu &other);
		WinSystemMenu& operator=(const WinSystemMenu &other);
		virtual ~WinSystemMenu();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		void print();
};



#endif