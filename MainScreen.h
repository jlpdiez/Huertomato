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
		MainScreen();
		MainScreen(const MainScreen &other);
		MainScreen& operator=(const MainScreen &other);
		~MainScreen();
		void printWindow();
		void drawWindow();
		void updateWindow();
		int processTouch();
		int getActScreen() const;
		
	protected:		
		void printMainHeader();
		void updateMainHeader();
};

#endif 