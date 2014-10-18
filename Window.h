/*
 * Sensor.h
 *
 * Created: 10/17/2014 8:49:56 PM
 *  Author: eNDeR
 */ 


#ifndef WINDOW_H
#define WINDOW_H

#include <Arduino.h>
#include "Settings.h"
#include "Sensors.h"
#include "Buttons.h"
#include "GUI.h"
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <SD.h>

extern UTFT *_lcd;
//extern UTouch *_touch;
//extern Sensors *_sensors;
//extern Settings *_settings;

//Fonts
extern uint8_t hallfetica_normal[];
extern uint8_t various_symbols[];

//Colours
extern uint8_t lightGreen[3];
extern uint8_t darkGreen[3];
extern uint8_t grey[3];
extern uint8_t white[3];

extern int xSize;

class Window {
	
	public:
		Window();
		~Window();
		
		virtual void printWindow() = 0;
		virtual void drawWindow() = 0;
		virtual void updateWindow() = 0;	
		virtual void processTouch() = 0;
	
	protected:
		static Borderless_Buttons _buttons;
		void printHeaderBackground();
};

//Constructors
Window::Window() {
	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
}

//Destructor
Window::~Window() {}
	


//Prints header background and separator line
void Window::printHeaderBackground() {
	const int headerHeight = 20;
	//Header background
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->fillRect(0,0,xSize,headerHeight);
	//Separator line
	_lcd->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
	_lcd->drawLine(0, headerHeight, xSize, headerHeight);
}

#endif