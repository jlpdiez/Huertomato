// #############################################################################
//
// # Name       : WinPhAlarms
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 19.11.2014
//
// # Description: Ph alarms configuration window
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


#ifndef WINPHALARMS_H_
#define WINPHALARMS_H_

#include "Window.h"

const int nPHalarmsButtons = 7;
static char* phAlarmsButtonsText[nPHalarmsButtons] = {
	"=",
	">",
	"=",
	">"
}; 

static int phAlarmsButtons[nPHalarmsButtons];

static char* nameWinPhAlarms = "- pH Alarms -";
static char* uPhLimit = "Upper Limit:";
static char* dPhLimit = "Lower Limit:";

class WinPhAlarms: public Window {
	public:
		WinPhAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinPhAlarms(const WinPhAlarms &other);
		WinPhAlarms& operator=(const WinPhAlarms &other);
		~WinPhAlarms();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		float _phAlarmMax, _phAlarmMin;
		void print();
};

#endif