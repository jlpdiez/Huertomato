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
		WinWater(UTFT *lcd, UTouch *touch);
		WinWater(const WinWater &other);
		WinWater& operator=(const WinWater &other);
		virtual ~WinWater();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		void print();
};

#endif