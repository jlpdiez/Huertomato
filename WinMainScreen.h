// #############################################################################
//
// # Name       : WinMainScreen
// # Version    : 1.3
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 16.01.2015
//
// # Description: Main idle screen window
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


#ifndef WINMAINSCREEN_H_
#define WINMAINSCREEN_H_

#include "Sensors.h"
#include "Settings.h"
#include "Window.h"
#include <SD.h>   
#include <Time.h>

//Sensor names
const char sensorTextStr0[] PROGMEM = "Humidity:";
const char sensorTextStr1[] PROGMEM = "Temp:";
const char sensorTextStr2[] PROGMEM = "Light:";
const char sensorTextStr3[] PROGMEM = "pH:";
const char sensorTextStr4[] PROGMEM = "EC:";
const char sensorTextStr5[] PROGMEM = "Deposit:";
//Paths for finding icons in SD card
const char plantPath[] PROGMEM = "/PICTURE/plant126.RAW";
const char nightPath[] PROGMEM  = "/PICTURE/moon126.RAW";
const char logoPath[] PROGMEM = "/PICTURE/logo126.RAW";
const char alarmPath[] PROGMEM = "/PICTURE/alarm126.RAW";
//Misc texts
const char htmtTxt[] PROGMEM = "Huertomato";
const char tempUnitC[] PROGMEM = "C";
const char tempUnitF[] PROGMEM = "F";
const char lightUnit[] PROGMEM = "Lux";
const char ecUnit[] PROGMEM = "uS";
//Status strings
const char nextWater[] PROGMEM = "Next Watering @";
const char noNight[] PROGMEM = "No Watering @ Night";
const char htmtWatering[] PROGMEM = "Huertomato Watering";
const char alarmCont[] PROGMEM = "Alarm - Check Solution";
const char normalCont[] PROGMEM = "System working fine";
const char pumpCont[] PROGMEM = "Watering OFF for pump";

//Contains two main screens. One when reservoir module is on, another when off
class WinMainScreen: public Window {
	public:	
		WinMainScreen(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinMainScreen(const WinMainScreen &other);
		WinMainScreen& operator=(const WinMainScreen &other);
		~WinMainScreen();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
		 
	protected:	
		static const uint8_t _headerTextY = 2;
		static const uint8_t _statusTextY = 200;
		static const uint8_t _statusTextX = 10;
		static const uint8_t _statusLength = 22;
		void printMainHeader();
		void updateMainHeader();
				
		void printInfoReservoir();
		void updateInfoReservoir();
		void printInfoNoModule();
		void updateInfoNoModule();
		
		void printStatus(const char* msg);
		void printNextWaterHour();
		void printIconAndStatus();
		void updateIconAndStatus();
};

#endif 