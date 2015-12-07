// #############################################################################
//
// # Name       : WinTime
// # Version    : 0.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 07.12.2015
//
// # Description: Time configuration screen
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

class WinTime: public Window {
	public:
		WinTime(LiquidCrystal *lcd, Sensors *sensors, Settings *settings);
		WinTime(const WinTime &other);
		WinTime& operator=(const WinTime &other);
		~WinTime();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(int);
	
	protected:
		//Tracks position of cursor
		//Line 0 is time, line 1 is date
		//Line 0 Columns: 5-hour; 8-minute; 11-second
		//Line 1 Columns: 4-day; 7-month; 12-year
		uint8_t  _line, _column;
		//Internal time variables
		uint8_t _sysHour, _sysMin, _sysSec, _sysDay, _sysMonth;
		int _sysYear;
		//Tracks date change to update RTC or not
		boolean _modified;
};

#endif