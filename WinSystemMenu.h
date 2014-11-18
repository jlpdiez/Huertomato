// #############################################################################
//
// # Name       : WinSystemMenu
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
//
// # Description: System menu window
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


#ifndef WINSYSTEMMENU_H_
#define WINSYSTEMMENU_H_

#include "Window.h"

const int nSystemButtons = 6;
static char* systemButtonText[nSystemButtons] = {
	"Watering Cycle",
	"Night Watering",
	"Reservoir Module"
};
static int systemButtons[nSystemButtons];
 
static char* nameWinSystemMenu = "- System Settings -";

class WinSystemMenu: public Window {
	public:
		WinSystemMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSystemMenu(const WinSystemMenu &other);
		WinSystemMenu& operator=(const WinSystemMenu &other);
		~WinSystemMenu();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		boolean _nightWater;
		void print();
};



#endif