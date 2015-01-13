// #############################################################################
//
// # Name       : WinMainScreen
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
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

const char sensorTextStr0[] PROGMEM = "Humidity:";
const char sensorTextStr1[] PROGMEM = "Temp:";
const char sensorTextStr2[] PROGMEM = "Light:";
const char sensorTextStr3[] PROGMEM = "pH:";
const char sensorTextStr4[] PROGMEM = "EC:";
const char sensorTextStr5[] PROGMEM = "Deposit:";
const char* const sensorText[] = { sensorTextStr0, sensorTextStr1, sensorTextStr2,
	sensorTextStr3, sensorTextStr4, sensorTextStr5 };

//Paths for finding icons in SD card
const char plantPath[] PROGMEM = "/PICTURE/plant126.RAW";
const char nightPath[] PROGMEM  = "/PICTURE/moon126.RAW";
const char logoPath[] PROGMEM = "/PICTURE/logo126.RAW";
const char alarmPath[] PROGMEM = "/PICTURE/alarm126.RAW";
//Misc texts
const char versionName[] PROGMEM = "Huertomato 1.4";
const char tempUnit[] PROGMEM = "C";
const char lightUnit[] PROGMEM = "Lux";
const char ecUnit[] PROGMEM = "uS";
//Spaces at the end are used to have all strings of same size and no thrash on screen
const char nextWater[] PROGMEM = "Next Watering @ ";
const char noNight[] PROGMEM = "No Watering @ Night   ";
const char htmtWatering[] PROGMEM = "Huertomato Watering   ";
const char alarmCont[] PROGMEM = "Alarm - Check Solution";
const char normalCont[] PROGMEM = "System working fine  ";

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
		void printMainHeader();
		void updateMainHeader();
				
		void printInfoReservoir();
		void updateInfoReservoir();
		void printInfoNoModule();
		void updateInfoNoModule();
	
		void printIconAndStatus();
		void updateIconAndStatus();
};

#endif 