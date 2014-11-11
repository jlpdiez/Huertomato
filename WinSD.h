// #############################################################################
//
// # Name       : WinSD
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
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

const int nSDcardButtons = 8;
static char* sdCardButtonsText[nSDcardButtons] = {
	"SD Card Log:",
	"=",
	"=",
	">",
	">"
};
static int sdCardButtons[nSDcardButtons];

static char* nameWinSD = "- SD Card -";
static char* sdCardText1 = "Save every";

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
		static const int _yFirstLine = 60;
		static const int _ySecondLine = 135;
		static const int _xSpacer = 25;
		boolean _sdActive;
		uint8_t _sdHour, _sdMin;
		void print();
};



#endif