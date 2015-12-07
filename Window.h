// #############################################################################
//
// # Name       : Window
// # Version    : 1.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 7.12.2015
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

/*const char loadingText[] PROGMEM = "Huertomato is loading...";
//Common window buttons text
const char backText[] PROGMEM = " Back ";
const char saveText[] PROGMEM = " Save ";
const char savedText[] PROGMEM = " Saved ";
const char cancelText[] PROGMEM = " Exit ";
//Common window toggles
//Has one extra char to cover last F of OFF
const char onStr[] PROGMEM = "ON ";
const char offStr[] PROGMEM = "OFF";
const char celsStr[] PROGMEM = "Celsius   ";
const char farenhStr[] PROGMEM = "Fahrenheit";
//various_symbols font is used for these
const char bulletStr[] PROGMEM = "T";
const char plusStr[] PROGMEM = "=";
const char minusStr[] PROGMEM = ">";
//Other common texts
const char headerDecoration[] PROGMEM = "-";
const char spaceChar[] PROGMEM = " ";*/
const char timeSeparator[] PROGMEM = ":";
const char dateSeparator[] PROGMEM = "/";
//const char percentSign[] PROGMEM = "%";
//const char hoursChar[] PROGMEM = "h";
//const char minutesChar[] PROGMEM = "m";
/*//Calibration texts
const char startCalibStr1[] PROGMEM = "Start calibration";
const char startCalibStr2[] PROGMEM = "procedure now?";
const char yesStr[] PROGMEM = "Yes";
const char continueStr[] PROGMEM = "Continue";
const char endStr[] PROGMEM = "End";*/

class Window {
	public:
		enum Screen {
			None = 0,
			Splash = 1,
			MainScreen = 2,
			TimeDate = 3,
			WateringCycle = 4,
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
		//int centerX(const char* c);
		
		//These are used to read data from PROGMEM and store them into SRAM
		char _stringBuffer[30];
		char* pmChar(const char* pmArray);
	
		LiquidCrystal *_lcd;
		Sensors *_sensors;
		Settings *_settings;
};

#endif