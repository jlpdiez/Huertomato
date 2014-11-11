// #############################################################################
//
// # Name       : WinMainScreen
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
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

//Holds text strings for each sensor
const int nSensorText = 6;
static char* sensorText[nSensorText] = {
	"Humidity:",
	"Temp:",
	"Light:",
	"pH:",
	"EC:",
	"Deposit:"
};
//Name is not used really. Its here because its neccesary for other windows
static char* nameWinMainScreen = "- Main Screen -";
//Paths for finding icons in SD card
static char* plantPath = "/PICTURE/plant126.RAW";
static char* nightPath  = "/PICTURE/moon126.RAW";
static char* logoPath = "/PICTURE/logo126.RAW";
static char* alarmPath = "/PICTURE/alarm126.RAW";
//Misc texts
static char* versionName = "Huertomato 1.3";
static char* humidUnit = "%";
static char* tempUnit = "C";
static char* lightUnit = "%";
static char* ecUnit = "uS";
static char* lvlUnit = "%";
//Spaces at the end are used to have all strings of same size and no thrash on screen
static char* nextWater = "Next Watering @ ";
static char* noNight = "No Watering @ Night   ";
static char* htmtWatering = "Huertomato Watering   ";
static char* alarmCont = "Alarm - Check Solution";
static char* normalCont = "System working fine  ";

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
		void print();	
		void printMainHeader();
		void printSensorInfo();
		void printIconAndStatus();
		void updateMainHeader();
		void updateIconAndStatus();
		void updateSensorInfo();
};

#endif 