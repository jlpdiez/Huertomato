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
		void createAndRenderWindow();
			
	    UTFT *_lcd;
	    UTouch *_touch;
	    Sensors *_sensors;
	    Settings *_settings;
		Window *_window;
	
		//Screen currently active
		//1-Main Screen, 2-Main Menu, 3-System Settings, 4-Controller Settings,
		//5-Time & Date, 6-Sensor Polling, 7-SD Card, 8-Watering Cycle
		//9-Sensor Alarms, 10-pH Alarms, 11-EC Alarms, 12-Nutrient Level Alarms,
		//13-Sensor Calibration, 14-Water Level Calibration
		//15-Light Calibration, 16-Pump Protection
		uint8_t _actScreen;
};
    
#endif
