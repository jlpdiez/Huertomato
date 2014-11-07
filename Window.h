/*
 * Window.h
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
extern uint8_t Sinclair_S[];

//Colours
static uint8_t lightGreen[3] = {184, 210, 60};
static uint8_t darkGreen[3] = {141, 170, 39};
static uint8_t grey[3] = {100,100,100};
static uint8_t white[3] = {255,255,255};
static uint8_t red[3] = {200,0,0};
static uint8_t blue[3] = {0,135,199};
//static uint8_t yellow[3] = {255,242,32};

class Window {
	
	public:
		Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		Window(const Window &other);
		Window& operator=(const Window &other);
		virtual ~Window();
		
		virtual void draw();
		virtual void update();	
		virtual int processTouch(int x, int y);

	protected:
		//1-Main Screen, 2-Main Menu, 3-System Settings, 4-Controller Settings,
		//5-Time & Date, 6-Sensor Polling, 7-SD Card, 8-Watering Cycle
		//9-Sensor Alarms, 10-pH Alarms, 11-EC Alarms, 12-Nutrient Level Alarms,
		//13-Sensor Calibration, 14-Water Level Calibration
		//15-Light Calibration, 16-Pump Protection
		enum Screen {
			Splash=0,
			MainScreen=1,
			MainMenu=2,
			SystemSettings=3,
			ControllerSettings=4,
			TimeDate=5,
			SensorPolling=6,
			SDCard=7,
			WateringCycle=8,
			Alarms=9,
			PhAlarms=10,
			EcAlarms=11,
			LvlAlarms=12,
			SensorCalib=13,
			LvlCalib=14,
			LightCalib=15,
			Pump=16
		};
		
		static const int xSize = 399;
		static const int ySize = 239;
		static const int bigFontSize = 16;
		static const int smallFontSize = 8;
		
		virtual void printWindow();
		void addFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]);
		void printHeaderBackground();
		void printMenuHeader(char* c);
		void printSavedButton();

		UTFT *_lcd;
		UTouch *_touch;
		Sensors *_sensors;
		Settings *_settings;
		Borderless_Buttons _buttons;
};

#endif