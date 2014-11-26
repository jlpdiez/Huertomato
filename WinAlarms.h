// #############################################################################
//
// # Name       : WinAlarms
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 24.11.2014
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

const int nSensorAlarmsButtons = 6;
static char* sensorAlarmsButtonsText[nSensorAlarmsButtons] = {
	"pH Thresholds",
	"EC Thresholds",
	"Nutrient Level"
};
static int sensorAlarmsButtons[nSensorAlarmsButtons];

static char* nameWinAlarms = "- Sensor Alarms -";
 
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
		void print();
};



#endif