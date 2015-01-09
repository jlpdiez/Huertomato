// #############################################################################
//
// # Name       : WinTime
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.01.2015
//
// # Description: Time configuration window
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


#ifndef WINTIME_H_
#define WINTIME_H_

#include "Window.h"
#include <Time.h>
/*const char* timeButtonText[_nTimeButtons] = {
	"=",
	"=",
	"=",
	">",
	">",
	">",
	"=",
	"=",
	"=",
	">",
	">",
	">"
};*/
 
const char nameWinTime[] PROGMEM = "- Time & Date -";
const char timeS[] PROGMEM = "Time";
const char timeFormatS[] PROGMEM = "(HH:MM:SS)";
const char timeSeparator[] PROGMEM = ":";
const char dateS[] PROGMEM = "Date";
const char dateFormatS[] PROGMEM = "(DD/MM/YYYY)";
const char dateSeparator[] PROGMEM = "/";

class WinTime: public Window {
	public:
		WinTime(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinTime(const WinTime &other);
		WinTime& operator=(const WinTime &other);
		~WinTime();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nTimeButtons = _nFlowButtons + 12;
		uint8_t _timeButtons[_nTimeButtons];
		uint8_t _sysHour, _sysMin, _sysSec, _sysDay, _sysMonth;
		int _sysYear;
		void print();
};

#endif