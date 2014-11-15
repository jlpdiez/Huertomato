// #############################################################################
//
// # Name       : Window
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
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
#include <UTFT_Buttons.h>

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

//Common window buttons text
static char* backText = " Back ";
static char* saveText = " Save ";
static char* savedText = " Saved ";
static char* cancelText = " Exit ";
//Common window toggles
static char* onStr = "ON ";
static char* offStr = "OFF";
//various_symbols font is used for bulletpoints
static char* bulletStr = "T";


class Window {
	public:
		enum Screen {
			None=0,
			Splash=1,
			MainScreen=2,
			MainMenu=3,
			SystemSettings=4,
			ControllerSettings=5,
			TimeDate=6,
			SensorPolling=7,
			SDCard=8,
			WateringCycle=9,
			Alarms=10,
			PhAlarms=11,
			EcAlarms=12,
			LvlAlarms=13,
			SensorCalib=14,
			LvlCalib=15,
			LightCalib=16,
			Pump=17
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
		static const int _xSize = 399;
		static const int _ySize= 239;
		static const int _bigFontSize = 16;
		static const int _smallFontSize = 8;
		static const int _bigIconSize = 126;
		//static const int _smallIconSize = 64;
		static const int _headerHeight = 20;
		static const int _flowButtonY = 215;
		//static const int _xSpacerMenu = 15;
		//static const int _xSpacerConfig = 25;
	 	
		virtual void print();
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