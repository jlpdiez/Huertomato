// #############################################################################
//
// # Name       : WinAlarms
// # Version    : 1.3
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.01.2015
//
// # Description: Alarms menu window
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


#ifndef WINALARMS_H_
#define WINALARMS_H_

#include "Window.h"

//Window name
const char nameWinAlarms[] PROGMEM = "- Sensor Alarms -";

//Window buttons texts
const char alarmsStr0[] PROGMEM = "pH Thresholds";
const char alarmsStr1[] PROGMEM = "EC Thresholds";
const char alarmsStr2[] PROGMEM = "Nutrient Level";
const char* const sensorAlarmsButtonsText[] PROGMEM = { alarmsStr0, alarmsStr1, alarmsStr2 };
	 
class WinAlarms: public Window {
	public:
		WinAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinAlarms(const WinAlarms &other);
		WinAlarms& operator=(const WinAlarms &other);
		~WinAlarms();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		//Here lies the button info for each window
		//We add 3 to all nButtons to account for back/save/exit (These are always 0,1,2)
		static const uint8_t _nSensorAlarmsButtons = _nFlowButtons + 3;
		//Array that will contain the buttons
		uint8_t _sensorAlarmsButtons[_nSensorAlarmsButtons];
		void print();
};



#endif