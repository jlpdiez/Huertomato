// #############################################################################
//
// # Name       : WinEcAlarms
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
//
// # Description: Ec alarms configuration window.
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


#ifndef WINECALARMS_H_
#define WINECALARMS_H_

#include "Window.h"

const char nameWinEcAlarms[] PROGMEM = "EC Alarms";

const char unitEcS[] PROGMEM = "uS";
const char uEcLimit[] PROGMEM = "Upper Limit:";
const char dEcLimit[] PROGMEM = "Lower Limit:";
 
class WinEcAlarms: public Window {
	public:
		WinEcAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinEcAlarms(const WinEcAlarms &other);
		WinEcAlarms& operator=(const WinEcAlarms &other);
		~WinEcAlarms();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nECalarmsButtons = _nFlowButtons + 4;
		int8_t _ecAlarmsButtons[_nECalarmsButtons]; 
		uint16_t _ecAlarmMax, _ecAlarmMin;
		void print();
};

#endif