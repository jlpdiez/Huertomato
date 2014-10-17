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
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <SD.h>

class Window {
	
	public:
		//Constructors
		//GUI(int lcdRS,int lcdWR,int lcdCS,int lcdRST,int lcdTCLK,int lcdTCS,int lcdTDIN,int lcdTDOUT,int lcdIRQ);
		Window();
		Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		Window(const Window &other);
		Window& operator=(const Window &other);
		//Destructor
		~Window();
		//Getters
		int getActScreen() const;
		
		virtual void printWindow() = 0;
		virtual void drawWindow() = 0;
		virtual void updateWindow() = 0;	
		virtual void processTouchWindow() = 0;
	
	private:	
		static uint8_t _actScreen;
		
		static UTFT *_lcd;
		static UTouch *_touch;
		static Sensors *_sensors;
		static Settings *_settings;
		static Borderless_Buttons _buttons;
		
		void drawSplashScreen();
		void printHeaderBackground();
};

//Constructors
Window::Window() {
}

/*Window::Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: _lcd(lcd), _touch(touch), _buttons(lcd,touch), _sensors(sensors), _settings(settings) {
	_actScreen = 0;

	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
	
}*/

Window::Window(const Window &other) /*: _buttons(other._buttons)*/ {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	
	_actScreen = 0;

	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
}

Window& Window::operator=(const Window &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_buttons = other._buttons;
	_sensors = other._sensors;
	_settings = other._settings;
	
	return *this;
}

//Destructor
Window::~Window() {}

//Getter
int Window::getActScreen() const {	return _actScreen; }

//Draw splash Screen
//TODO: warn when no RTC or SD present- would be cool if we asked for a touchScreen
void Window::drawSplashScreen() {
	const int iconSize = 126;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Shows centered icon
	_lcd->drawBitmap(xSize/2-(iconSize/2),10,iconSize,iconSize,logo126);
	//Shows centered text
	char* message = "Huertomato is loading...";
	_lcd->print(message,xSize/2-(bigFontSize*(strlen(message)/2)),50+iconSize);
}

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