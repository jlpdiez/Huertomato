// #############################################################################
//
// # Name       : WinSensorPolling
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.01.2015
//
// # Description: Time between sensor readings configuration window
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

#ifndef WINSENSORPOLLING_H_
#define WINSENSORPOLLING_H_

#include "Window.h"

/*const char* sensorPollingButtonText[nSensorPollingButtons] = {
	"=",
	">"
};*/

const char nameWinSensorPolling[] PROGMEM = "- Sensor Polling -";

const char sensorPollingText1[] PROGMEM = "Time between readings:";
const char sensorPollingText2[] PROGMEM = "seconds";

class WinSensorPolling: public Window {
	public:
		WinSensorPolling(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSensorPolling(const WinSensorPolling &other);
		WinSensorPolling& operator=(const WinSensorPolling &other);
		~WinSensorPolling();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nSensorPollingButtons = _nFlowButtons + 2;
		uint8_t _sensorPollingButtons[_nSensorPollingButtons];
		uint8_t _pollSec;
		void print();
};

#endif