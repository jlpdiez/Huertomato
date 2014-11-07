/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINLIGHTCALIB_H_
#define WINLIGHTCALIB_H_

#include "Window.h"

/*
	//15-Light Calibration
*/


const int nLightCalibrationButtons = 4;
static char* lightCalibrationButtonsText[nLightCalibrationButtons] = {
	"Set"
};
static int lightCalibrationButtons[nLightCalibrationButtons];


class WinLightCalib: public Window {
	public:
		WinLightCalib(UTFT *lcd, UTouch *touch);
		WinLightCalib(const WinLightCalib &other);
		WinLightCalib& operator=(const WinLightCalib &other);
		virtual ~WinLightCalib();
		void draw();
		int processTouch(int x, int y);
	
	protected:
		void print();
};



#endif