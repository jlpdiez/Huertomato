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
//#include <Time.h>
//#include <TimeAlarms.h>
//#include <SD.h>

//Main Screen Icons
extern prog_uint16_t plant126[0x3E04];
extern prog_uint16_t alarm126[0x3E04];
extern prog_uint16_t logo126[0x3E04];
extern prog_uint16_t moon126[0x3E04];

//Menu Icons
extern prog_uint16_t plant64[0x1000];
extern prog_uint16_t plantDark64[0x1000];
extern prog_uint16_t settings64[0x1000];
extern prog_uint16_t settingsDark64[0x1000];

//Fonts
extern uint8_t hallfetica_normal[];
extern uint8_t various_symbols[];
//extern uint8_t Sinclair_S[];

//Colours
/*uint8_t lightGreen[3] = {184, 210, 60};
uint8_t darkGreen[3] = {141, 170, 39};
uint8_t grey[3] = {100,100,100};
uint8_t white[3] = {255,255,255};*/


//Colours
//uint8_t lightGreen[3] = {184, 210, 60};
//uint8_t darkGreen[3] = {141, 170, 39};
//uint8_t grey[3] = {100,100,100};
//extern uint8_t red[3] = {200,0,0};
//extern uint8_t blue[3] = {0,135,199};
//extern uint8_t yellow[3] = {255,242,32};
//uint8_t white[3] = {255,255,255};

extern int xSize;

class Window {
	
	public:
		Window(UTFT *lcd, UTouch *touch);
		Window(const Window &other);
		Window& operator=(const Window &other);
		virtual ~Window();
		
		virtual void draw();
		virtual void update();	
		virtual int processTouch(int x, int y);

	protected:
		virtual void printWindow();
		void addFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]);
		
		static const int xSize = 399;
		static const int ySize = 239;
		static const int bigFontSize = 16;
		static const int smallFontSize = 8;
		
		//Colours
		/*static const uint8_t lightGreen[3];
		static const uint8_t darkGreen[3];
		static const uint8_t grey[3];
		static const uint8_t white[3];*/
		UTFT *_lcd;
		UTouch *_touch;
		//array de ventanas indexado por _actScreen
		Borderless_Buttons _buttons;
		
		friend class MainScreen;
};

#endif