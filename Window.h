// #############################################################################
//
// # Name       : Window
// # Version    : 1.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
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
#include "Buttons.h"
#include <UTFT.h>
#include <UTouch.h>

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

const char loadingText[] PROGMEM = "Huertomato is loading...";
//Common window buttons text
const char backText[] PROGMEM = " Back ";
const char saveText[] PROGMEM = " Save ";
const char savedText[] PROGMEM = " Saved ";
const char cancelText[] PROGMEM = " Exit ";
//Common window toggles
//Has one extra char to cover last F of OFF
const char onStr[] PROGMEM = "ON ";
const char offStr[] PROGMEM = "OFF";
//various_symbols font is used for these
const char bulletStr[] PROGMEM = "T";
const char plusStr[] PROGMEM = "=";
const char minusStr[] PROGMEM = ">";
//Other common texts
const char headerDecoration[] PROGMEM = "-";
const char spaceChar[] PROGMEM = " ";
const char timeSeparator[] PROGMEM = ":";
const char dateSeparator[] PROGMEM = "/";
const char percentSign[] PROGMEM = "%";
const char hoursChar[] PROGMEM = "h";
const char minutesChar[] PROGMEM = "m";

class Window {
	public:
		enum Screen {
			None = 0,
			Splash = 1,
			MainScreen = 2,
			MainMenu = 3,
			SystemSettings = 4,
			ControllerSettings = 5,
			TimeDate = 6,
			SensorPolling = 7,
			SDCard = 8,
			WateringCycle = 9,
			Alarms = 10,
			PhAlarms = 11,
			EcAlarms = 12,
			LvlAlarms = 13,
			SensorCalib = 14,
			LvlCalib = 15,
			PhCalib = 16,
			EcCalib = 17,
			NightWater = 18,
			Pump = 19,
			Reservoir = 20
		};
				
		Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		Window(const Window &other);
		Window& operator=(const Window &other);
		virtual ~Window();
		
		virtual Screen getType() const;
		virtual void draw();
		virtual void update();	
		virtual Screen processTouch(const int x, const int y);

	protected:		
		//Screen size
		static const int _xSize = 399;
		static const int _ySize= 239;
		//Font sizes
		static const uint8_t _bigFontSize = 16;
		static const uint8_t _smallFontSize = 8;
		//Icon sizes
		static const uint8_t _bigIconSize = 126;
		static const uint8_t _smallIconSize = 64;
		//Height of header
		static const uint8_t _headerHeight = 20;
		//The Y coord of the flow buttons bar
		static const uint8_t _flowButtonY = 215;
		//The max number of flow buttons
		static const uint8_t _nFlowButtons = 3;
		//First coords of windows
		static const uint8_t _xMenu = 15;
		static const uint8_t _xConfig = 25;
		static const uint8_t _signSpacer = 22;
		//The Y coord where the first text row will start
		static const uint8_t _yOneLine = 100;
		static const uint8_t _yTwoLnsFirst = 65;
		static const uint8_t _yTwoLnsSecond = 140;
		static const uint8_t _yThreeLnsFirst = 50;
		static const uint8_t _yThreeLnsSecond = 100;
		static const uint8_t _yThreeLnsThird = 150;
		static const uint8_t _yFourLines = 40;
		static const uint8_t _yFiveLines = 40;
		//These are used for menus that are drawn using a for
		//Will be multiplied with (_bigFontSize * number of row) to get Y coord in menus
		static const uint8_t _yFactor3lines = 3;
		static const float _yFactor4lines = 2.5;
		static const uint8_t _yFactor5lines = 2;
	 	
		virtual void print();
		void addFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int8_t buttonArray[]);
		void printHeaderBackground();
		void printMenuHeader(const char* c);
		void printSavedButton();
		int centerX(const char* c);
		
		//These are used to read data from PROGMEM and store them into SRAM
		char _stringBuffer[30];
		char* pmChar(const char* pmArray);
	
		UTFT *_lcd;
		UTouch *_touch;
		Sensors *_sensors;
		Settings *_settings;
		Borderless_Buttons _buttons;
};

#endif