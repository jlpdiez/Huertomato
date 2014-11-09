/*
 * mainScreen.h
 *
 * Created: 10/17/2014 9:12:39 PM
 *  Author: eNDeR
 */ 


#ifndef WINMAINSCREEN_H_
#define WINMAINSCREEN_H_

#include "Sensors.h"
#include "Settings.h"
#include "Window.h"
#include <SD.h>   
#include <Time.h>

//Holds text strings for each sensor
const int nSensorText = 6;
static char* sensorText[nSensorText] = {
	"Humidity:",
	"Temp:",
	"Light:",
	"pH:",
	"EC:",
	"Deposit:"
};

class WinMainScreen: public Window {
	public:	
		WinMainScreen(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinMainScreen(const WinMainScreen &other);
		WinMainScreen& operator=(const WinMainScreen &other);
		virtual ~WinMainScreen();
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
		
	protected:		
		void print();	
		void printMainHeader();
		void printSensorInfo();
		void printIconAndStatus();
		void updateMainHeader();
		void updateIconAndStatus();
		void updateSensorInfo();
};

#endif 