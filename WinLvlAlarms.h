// #############################################################################
//
// # Name       : WinLvlAlarms
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
//
// # Description: Nutrient level alarms menu window
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


#ifndef WINLVLALARMS_H_
#define WINLVLALARMS_H_
 
#include "Window.h"

const char nameWinLvlAlarms[] PROGMEM = "Nutrient Alarms";

const char wLimitLvlS[] PROGMEM = "Lower Limit:";

class WinLvlAlarms: public Window {
	public:
		WinLvlAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinLvlAlarms(const WinLvlAlarms &other);
		WinLvlAlarms& operator=(const WinLvlAlarms &other);
		~WinLvlAlarms();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nWaterAlarmsButtons = _nFlowButtons + 2;
		int8_t _waterAlarmsButtons[_nWaterAlarmsButtons];
		uint8_t _waterAlarmMin;
		void print();
};



#endif