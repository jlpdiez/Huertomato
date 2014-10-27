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

//extern UTFT *_lcd;
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
		Window(UTFT *lcd, UTouch *touch);
		Window(const Window &other);
		Window& operator=(const Window &other);
		~Window();
		
		virtual void printWindow();
		virtual void drawWindow();
		virtual void updateWindow();	
		virtual void processTouch();
		virtual int getActScreen() const;
	
	protected:
		//Screen currently active
		//0-Main Screen, 1-Main Menu, 2-System Settings, 3-Controller Settings,
		//4-Time & Date, 5-Sensor Polling, 6-SD Card, 7-Watering Cycle
		//8-Sensor Alarms, 9-pH Alarms, 10-EC Alarms, 11-Nutrient Level Alarms,
		//12-Auto Config Alarms, 13-Sensor Calibration, 14-Water Level Calibration
		//15-Light Calibration, 16-Pump Protection
		static uint8_t _actScreen;

		UTFT *_lcd;
		UTouch *_touch;
		//array de ventanas indexado por _actScreen!
		Borderless_Buttons _buttons;
		
		void printHeaderBackground();
};

#endif