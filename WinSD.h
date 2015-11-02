// #############################################################################
//
// # Name       : WinSD
// # Version    : 1.3
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.10.2015
//
// # Description: SD configuration window
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
 

#ifndef WINSD_H_
#define WINSD_H_

#include "Window.h"
#include "Settings.h"
#include <SD.h>                                                       

const char nameWinSD[] PROGMEM = "SD Card";

const char sdCardText0[] PROGMEM = "SD Card Log:";
const char sdCardText1[] PROGMEM = "Save every";

class WinSD: public Window {
	public:
		WinSD(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSD(const WinSD &other);
		WinSD& operator=(const WinSD &other);
		~WinSD();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nSDcardButtons = _nFlowButtons + 5;
		int8_t _sdCardButtons[_nSDcardButtons];
		boolean _sdActive;
		uint8_t _sdHour, _sdMin;
		void print();
};

#endif