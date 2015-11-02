// #############################################################################
//
// # Name       : WinControllerMenuTwo
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.10.2015
//
// # Description: Controller settings menu part two
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


#ifndef WINCONTROLLERMENUTWO_H_
#define WINCONTROLLERMENUTWO_H_

#include "Window.h"
 
const char nameWinControllerMenuTwo[] PROGMEM = "Controller Settings";

const char controller2Str0[] PROGMEM = "SD Card";
const char controller2Str1[] PROGMEM = "Sensor Polling";
const char controller2Str2[] PROGMEM = "Serial Debugging:";
const char* const controllerButtonTextTwo[] PROGMEM = { controller2Str0, controller2Str1, controller2Str2 };

class WinControllerMenuTwo: public Window {
	public:
		WinControllerMenuTwo(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinControllerMenuTwo(const WinControllerMenuTwo &other);
		WinControllerMenuTwo& operator=(const WinControllerMenuTwo &other);
		~WinControllerMenuTwo();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nControllerButtonsTwo = _nFlowButtons + 3;
		int8_t _controllerButtonsTwo[_nControllerButtonsTwo];
		//These are temp variables used for displaying data
		//They are read from _settings in print() funcs. Changed in processTouch()
		//displayed again with update() and saved to eeprom when button save is pressed
		//Serial toggle
		boolean _serialActive;
		void print();
		void printToggles();
};



#endif