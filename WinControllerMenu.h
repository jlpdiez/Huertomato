// #############################################################################
//
// # Name       : WinControllerMenu
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
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
 
const int nControllerButtons = 8;
static char* controllerButtonText[nControllerButtons] = {
	"Time & Date",
	"Sensor Polling",
	"SD Card",
	"Sound:",
	"Serial Debugging:"
};
static int controllerButtons[nControllerButtons];

static char* nameWinControllerMenu = "- Controller Settings -";

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
		static const int _xSpacer = 15;
		static const int _ySpacer = 40;
		
		//Sound toggle
		boolean _soundActive;
		//Serial toggle
		boolean _serialActive;
		void print();
};



#endif