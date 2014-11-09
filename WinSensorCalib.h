/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINSENSORCALIB_H_
#define WINSENSORCALIB_H_

#include "Window.h"

//13-Sensor Calibration


const int nSensorCalibrationButtons = 5;
static char* sensorCalibrationButtonsText[nSensorCalibrationButtons] = {
	"Water Levels",
	"Night Threshold"
};
static int sensorCalibrationButtons[nSensorCalibrationButtons];



class WinSensorCalib: public Window {
	public:
		WinSensorCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSensorCalib(const WinSensorCalib &other);
		WinSensorCalib& operator=(const WinSensorCalib &other);
		virtual ~WinSensorCalib();
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		void print();
};

#endif