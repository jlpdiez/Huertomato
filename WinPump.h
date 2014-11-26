// #############################################################################
//
// # Name       : WinPump
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 24.11.2014
//
// # Description: Pump protection configuration window
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


#ifndef WINPUMP_H_
#define WINPUMP_H_

#include "Window.h"

const int nPumpProtectionButtons = 5;
static char* pumpProtectionButtonsText[nPumpProtectionButtons] = {
	"=",
	">"
};
static int pumpProtectionButtons[nPumpProtectionButtons];

static char* nameWinPump = "- Pump Protection -";
static char* wPumpLimit = "Min Water Lvl:";
 
class WinPump: public Window {
	public:
		WinPump(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinPump(const WinPump &other);
		WinPump& operator=(const WinPump &other);
		~WinPump();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		uint8_t _pumpProtectionLvl;
		void print();
};

#endif