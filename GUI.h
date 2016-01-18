// #############################################################################
// 
// # Name       : GUI
// # Version    : 0.6
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 18.01.2016
// 
// # Description: Library for controlling Huertomato's GUI
// # Contains window and manages its transition and button presses
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
#include "Settings.h"
#include "Sensors.h" 
#include "Window.h"
#include "WinMainScreen.h"
#include "WinTime.h"
#include "WinWater.h"
#include "WinNutrient.h"
#include <LiquidCrystal.h>

extern const uint8_t lcdA0;

//Class in charge of starting and using LCD
//Holds a pointer to a window
class GUI {
	public:		
		enum Button {
			None = 0,
			Right = 1,
			Up = 2,
			Down = 3,
			Left = 4,
			Select = 5,
		};
		//Constructors
		GUI(LiquidCrystal *lcd, Sensors *sensors, Settings *settings);
		GUI(const GUI &other);
		GUI& operator=(const GUI &other);
		//Destructor
		~GUI();
		
		//First function to call. Setups and shows Splash Screen
		void init();
		//Second function. Shows Main Screen. Starts user interaction
		void start();
		void processInput();
		//Refreshes non-static windows.
		void refresh();
		boolean isMainScreen();
		
	private:
		void updateScreen(Window::Screen newScreen);
		
		int _prevADC;
	    LiquidCrystal *_lcd;
	    Sensors *_sensors;
	    Settings *_settings;
		Window *_window;
};
    
#endif
