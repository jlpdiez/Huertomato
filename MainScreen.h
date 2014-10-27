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
		MainScreen(UTFT *lcd, UTouch *touch);
		MainScreen(const MainScreen &other);
		MainScreen& operator=(const MainScreen &other);
		~MainScreen();
		void printWindow();
		void drawWindow();
		void updateWindow();
		void processTouch();
		int getActScreen() const;
		
	protected:		
		void printMainHeader();
		void updateMainHeader();
};

MainScreen::MainScreen(UTFT *lcd, UTouch *touch) : Window(_lcd,_touch) {
	_actScreen = 0;
}

MainScreen::MainScreen(const MainScreen &other) : Window(_lcd,_touch) {
	
}

MainScreen& MainScreen::operator=(const MainScreen &other) {
	
}

int MainScreen::getActScreen() const {
	return _actScreen;
}



#endif 