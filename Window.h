// #############################################################################
//
// # Name       : Window
// # Version    : 1.6
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 18.01.2016
//
// # Description: Superclass window. holds types of screens, and common variables.
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


#ifndef WINDOW_H
#define WINDOW_H

#include <Arduino.h>
#include "Settings.h"
#include "Sensors.h"
#include <LiquidCrystal.h>

const char loadingText[] PROGMEM = "Loading system..";

const char spaceChar[] PROGMEM = " ";
const char celsStr[] PROGMEM = "C";
const char timeSeparator[] PROGMEM = ":";
const char dateSeparator[] PROGMEM = "/";
const char percentSign[] PROGMEM = "%";
const char minutesChar[] PROGMEM = "m";
const char zero[] PROGMEM = "0";
const char miliSiemens[] PROGMEM = "mS";

class Window {
	public:
		enum Screen {
			None = 0,
			Splash = 1,
			MainScreen = 2,
			TimeDate = 3,
			WateringCycle = 4,
			NutrientCounter = 5,
		};
		
		//Here for reference - Implemented in GUI
		/*enum Button {
			Non = 0,
			Right = 1,
			Up = 2,
			Down = 3,
			Left = 4,
			Select = 5,
		};*/
				
		Window(LiquidCrystal *lcd, Sensors *sensors, Settings *settings);
		Window(const Window &other);
		Window& operator=(const Window &other);
		virtual ~Window();
		
		virtual Screen getType() const;
		virtual void draw();
		virtual void update();	
		virtual Screen processTouch(int);

	protected:			 	
		virtual void print();
		
		//These are used to read data from PROGMEM and store them into SRAM
		char _stringBuffer[30];
		char* pmChar(const char* pmArray);
	
		LiquidCrystal *_lcd;
		Sensors *_sensors;
		Settings *_settings;
};

#endif