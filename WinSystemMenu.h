// #############################################################################
//
// # Name       : WinSystemMenu
// # Version    : 1.3
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.01.2015
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
 
const char nameWinSystemMenu[] PROGMEM = "- System Settings -";

const char sysMenuStr0[] PROGMEM = "Watering Cycle";
const char sysMenuStr1[] PROGMEM = "Night Watering";
const char sysMenuStr2[] PROGMEM = "Reservoir Module";
const char* const systemButtonText[] PROGMEM = { sysMenuStr0, sysMenuStr1, sysMenuStr2 };

class WinSystemMenu: public Window {
	public:
		WinSystemMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSystemMenu(const WinSystemMenu &other);
		WinSystemMenu& operator=(const WinSystemMenu &other);
		~WinSystemMenu();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nSystemButtons = _nFlowButtons + 3;
		uint8_t _systemButtons[_nSystemButtons];
		boolean _nightWater;
		void print();
};



#endif