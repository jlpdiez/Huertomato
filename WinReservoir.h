// #############################################################################
//
// # Name       : WinReservoir
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
//
// # Description: Reservoir module submenu. On/off toggle, alarms, calibration and pump protection
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

#ifndef WINRESERVOIR_H_
#define WINRESERVOIR_H_

#include "Window.h"

const char nameWinReservoir[] PROGMEM = "Reservoir Module";

const char reservoirStr0[] PROGMEM = "Reservoir Module:";
const char reservoirStr1[] PROGMEM = "Sensor Alarms";
const char reservoirStr2[] PROGMEM = "Pump Protection";
const char reservoirStr3[] PROGMEM = "Sensor Calibration";
const char* const reservoirButtonsText[] PROGMEM = { reservoirStr0, reservoirStr1, reservoirStr2, reservoirStr3};

class WinReservoir: public Window {
	public:
		WinReservoir(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinReservoir(const WinReservoir &other);
		WinReservoir& operator=(const WinReservoir &other);
		~WinReservoir();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
		
	protected:	
		static const uint8_t _nReservoirButtons = _nFlowButtons + 4;
		int8_t _reservoirButtons[_nReservoirButtons];
		boolean _reservoirActive;
		void print();
};

#endif 