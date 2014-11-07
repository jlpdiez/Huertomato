// #############################################################################
// 
// # Name       : GUI
// # Version    : 2.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 04.05.2014
// 
// # Description: Library for controlling Huertomato's GUI
// # Contains all windows and manages its transition and button presses
// 
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
//#include "Settings.h"
//#include "Sensors.h" 
#include "Window.h"
#include "WinMainScreen.h"
#include <UTFT.h>
#include <UTouch.h>
//#include <UTFT_Buttons.h>
//#include <Time.h> 
//#include <TimeAlarms.h>
//#include <SD.h>    

//Here lies the button info for each window
//We add 3 to all nButtons to account for back/save/exit (These are always 0,1,2)
//static in this context means vars will only be accesible in this file
//http://www.cprogramming.com/tutorial/statickeyword.html



/*const int nAutoConfigButtons = 13;
static char* autoConfigButtonsText[nAutoConfigButtons] = {
  
};
static int autoConfigButtons[nAutoConfigButtons];*/


//These are temp variables used for displaying data
//They are read from _settings in print() funcs. Changed in processTouch()
//displayed again with update() and saved to eeprom when button save is pressed
//System Settings
//Watering cycle
static boolean waterTimed;
static uint8_t waterHour, waterMin, floodMin;
//pH alarms
static float phAlarmMax, phAlarmMin;
//EC alarms
static uint16_t ecAlarmMax, ecAlarmMin;
//Water alarms
static uint8_t waterAlarmMin;
//Water levels calibration
static uint16_t waterLvlMax;
static uint16_t waterLvlMin;
static uint16_t rawWaterLvl;
//Light calibration
static uint16_t lightThreshold;
static uint16_t rawLightLvl;
//Pump protection
static uint8_t pumpProtectionLvl;

//Night toggle
static boolean nightWater;
//Water pump toggle
static boolean waterPumpState;
//Controller Settings
//Time & Date
static uint8_t sysHour, sysMin, sysSec, sysDay, sysMonth;
static int sysYear;
//Sensor Polling
static uint8_t pollMin, pollSec;
//SD Card
static boolean sdActive;
static uint8_t sdHour, sdMin;
//Sound toggle
static boolean soundActive;
//Serial toggle
static boolean serialActive;

class GUI {
	public:		
		//Constructors
		GUI(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		GUI(const GUI &other);
		GUI& operator=(const GUI &other);
		//Destructor
		~GUI();
		
		void init();
		void processTouch();
		void refresh();
		boolean isMainScreen();
		
	private:
		void draw();
			
	    UTFT *_lcd;
	    UTouch *_touch;
	    Sensors *_sensors;
	    Settings *_settings;
		
		Window *_window;
				
		//Screen currently active
		//ALL NUMBERS +1 Screen 0 is loading screen
		//0-Main Screen, 1-Main Menu, 2-System Settings, 3-Controller Settings,
		//4-Time & Date, 5-Sensor Polling, 6-SD Card, 7-Watering Cycle
		//8-Sensor Alarms, 9-pH Alarms, 10-EC Alarms, 11-Nutrient Level Alarms,
		//12-Auto Config Alarms, 13-Sensor Calibration, 14-Water Level Calibration
		//15-Light Calibration, 16-Pump Protection
		uint8_t _actScreen;
};
    
#endif
