/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINLVLCALIB_H_
#define WINLVLCALIB_H_

#include "Window.h"

//14-Water Level Calibration


const int nWaterLevelButtons = 5;
static char* waterLevelButtonsText[nWaterLevelButtons] = {
	"Set",
	"Set"
};
static int waterLevelButtons[nWaterLevelButtons];


class WinLvlCalib: public Window {
	public:
		WinLvlCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinLvlCalib(const WinLvlCalib &other);
		WinLvlCalib& operator=(const WinLvlCalib &other);
		virtual ~WinLvlCalib();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		uint16_t _waterLvlMax, _waterLvlMin, _rawWaterLvl;
		void print();
};



#endif