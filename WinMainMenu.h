// #############################################################################
//
// # Name       : WinMainMenu
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
//
// # Description: Main menu window
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

#ifndef WINMAINMENU_H_
#define WINMAINMENU_H_

#include "Sensors.h"
#include "Settings.h"
#include "Window.h"

const int nMainMenuButtons = 7;
static char* mainMenuButtonText[nMainMenuButtons] = {
	"System Settings",
	"Controller Settings"
};
static int mainMenuButtons[nMainMenuButtons];
  
static char* nameWinMainMenu = "- Main Menu -";

class WinMainMenu: public Window {
	public:
		WinMainMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinMainMenu(const WinMainMenu &other);
		WinMainMenu& operator=(const WinMainMenu &other);
		~WinMainMenu();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		void print();
};

#endif