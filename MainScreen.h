/*
 * mainScreen.h
 *
 * Created: 10/17/2014 9:12:39 PM
 *  Author: eNDeR
 */ 


#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include "Window.h"
#include <Arduino.h>
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

class MainScreen: public Window {
	public:	
		MainScreen();
		~MainScreen();
		void printWindow();
		void drawWindow();
		void updateWindow();
		void processTouch();
		
	protected:
		void printMainHeader();
		void updateMainHeader();
};


int MainScreen::getActScreen() const {
	return _actScreen;
}


#endif 