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
extern UTouch *_touch;
extern Sensors *_sensors;
extern Settings *_settings;

//Main Screen Icons
/*extern prog_uint16_t plant126[0x3E04];
extern prog_uint16_t alarm126[0x3E04];

extern prog_uint16_t moon126[0x3E04];

//Menu Icons
extern prog_uint16_t plant64[0x1000];
extern prog_uint16_t plantDark64[0x1000];
extern prog_uint16_t settings64[0x1000];
extern prog_uint16_t settingsDark64[0x1000];*/

extern prog_uint16_t logo126[0x3E04];

//Fonts
//extern uint8_t Sinclair_S[];
extern uint8_t hallfetica_normal[];
extern uint8_t various_symbols[];

//Colours
extern uint8_t lightGreen[3];
extern uint8_t grey[3];
extern uint8_t white[3];
uint8_t darkGreen[3] = {141, 170, 39};

const int xSize = 399;
const int ySize = 239;
const int bigFontSize = 16;
const int smallFontSize = 8;

class Window {
	
	public:
		//Constructors
		//GUI(int lcdRS,int lcdWR,int lcdCS,int lcdRST,int lcdTCLK,int lcdTCS,int lcdTDIN,int lcdTDOUT,int lcdIRQ);
		Window();
		//Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		//Window(const Window &other);
		//Window& operator=(const Window &other);
		//Destructor
		virtual ~Window();
		//Getters
		virtual int getActScreen() const = 0;
		
		virtual void printWindow() = 0;
		virtual void drawWindow() = 0;
		virtual void updateWindow() = 0;	
		virtual void processTouch() = 0;
	
	protected:
		static uint8_t _actScreen;
		
		static Borderless_Buttons _buttons;
		
		void drawSplashScreen();
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