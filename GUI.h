// #############################################################################
// 
// # Name       : GUI
// # Version    : 2.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.11.2014
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
#include "WinAlarms.h"
#include "WinControllerMenu.h"
#include "WinEcAlarms.h"
#include "WinLightCalib.h"
#include "WinLvlAlarms.h"
#include "WinLvlCalib.h"
#include "WinMainMenu.h"
#include "WinMainScreen.h"
#include "WinPhAlarms.h"
#include "WinPump.h"
#include "WinSD.h"
#include "WinSensorCalib.h"
#include "WinSensorPolling.h"
#include "WinSystemMenu.h"
#include "WinTime.h"
#include "WinWater.h"

#include <UTFT.h>
#include <UTouch.h>

//Class in charge of starting and using LCD and Touchscreen
//Holds a pointer to a window
class GUI {
	public:		
		//Constructors
		GUI(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		GUI(const GUI &other);
		GUI& operator=(const GUI &other);
		//Destructor
		~GUI();
		
		//First function to call. Setups and shows Splash Screen
		void init();
		//Second function. Shows Main Screen
		void start();
		void processTouch();
		void refresh();
		boolean isMainScreen();
		
	private:
		void updateScreen();
			
	    UTFT *_lcd;
	    UTouch *_touch;
	    Sensors *_sensors;
	    Settings *_settings;
		Window *_window;
		Window::Screen _actScreen;
};
    
#endif
