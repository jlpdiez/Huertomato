/*
 * mainScreen.h
 *
 * Created: 10/17/2014 9:12:39 PM
 *  Author: eNDeR
 */ 


#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include "Window.h"

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

class MainScreen: public Window {
	public:	
		MainScreen(UTFT *lcd, UTouch *touch);
		MainScreen(const MainScreen &other);
		MainScreen& operator=(const MainScreen &other);
		virtual ~MainScreen();
		void draw();
		void update();
		int processTouch(int x, int y);
		
	protected:			
		void printHeaderBackground();
		void printMainHeader();
		void printSensorInfo();
		void printIconAndStatus();
		void updateMainHeader();
		void updateIconAndStatus();
		void updateSensorInfo();
};

#endif 