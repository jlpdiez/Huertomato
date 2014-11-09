/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINWATER_H_
#define WINWATER_H_

#include "Window.h"

//7-Watering Cycle

//Here lies the button info for each window
//We add 3 to all nButtons to account for back/save/exit (These are always 0,1,2)
//static in this context means vars will only be accesible in this file
//http://www.cprogramming.com/tutorial/statickeyword.html
const int nWaterCycleButtons = 10;
static char* waterCycleButtonsText[nWaterCycleButtons] = {
	"Mode:",
	"=",
	">",
	"=",
	">",
	"=",
	">",
};
static int  waterCycleButtons[nWaterCycleButtons];


class WinWater: public Window {
	public:
		WinWater(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinWater(const WinWater &other);
		WinWater& operator=(const WinWater &other);
		virtual ~WinWater();
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		//These are temp variables used for displaying data
		//They are read from _settings in print() funcs. Changed in processTouch()
		//displayed again with update() and saved to eeprom when button save is pressed
		boolean _waterTimed;
		uint8_t _waterHour, _waterMin, _floodMin;
		void print();
};

#endif