/*
 * mainScreen.h
 *
 * Created: 10/17/2014 9:12:39 PM
 *  Author: eNDeR
 */ 


#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include "Window.h"

class MainScreen: public Window {
	public:
		MainScreen();
		MainScreen(const MainScreen &other);
		MainScreen& operator=(const MainScreen &other);
		~MainScreen();
		
		void printWindow();
		void drawWindow();
		void updateWindow();
		void processTouchWindow();
		
	private:
		void printMainHeader();
		void updateMainHeader();
};





#endif 