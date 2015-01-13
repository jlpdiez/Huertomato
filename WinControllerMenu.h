// #############################################################################
//
// # Name       : WinControllerMenu
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
//
// # Description: Controller settings menu
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


#ifndef WINCONTROLLERMENU_H_
#define WINCONTROLLERMENU_H_

#include "Window.h"
 
const char nameWinControllerMenu[] PROGMEM = "Controller Settings";

const char controllerStr0[] PROGMEM = "Time & Date";
const char controllerStr1[] PROGMEM = "Sensor Polling";
const char controllerStr2[] PROGMEM = "SD Card";
const char controllerStr3[] PROGMEM = "Sound:";
const char controllerStr4[] PROGMEM = "Serial Debugging:";
const char* const controllerButtonText[] PROGMEM = { controllerStr0, controllerStr1, controllerStr2,
	controllerStr3, controllerStr4 };

class WinControllerMenu: public Window {
	public:
		WinControllerMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinControllerMenu(const WinControllerMenu &other);
		WinControllerMenu& operator=(const WinControllerMenu &other);
		~WinControllerMenu();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nControllerButtons = _nFlowButtons + 5;
		int8_t _controllerButtons[_nControllerButtons];
		//These are temp variables used for displaying data
		//They are read from _settings in print() funcs. Changed in processTouch()
		//displayed again with update() and saved to eeprom when button save is pressed
		//Sound toggle
		boolean _soundActive;
		//Serial toggle
		boolean _serialActive;
		void print();
};



#endif