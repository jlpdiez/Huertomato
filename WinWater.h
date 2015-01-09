// #############################################################################
//
// # Name       : WinWater
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.01.2015
//
// # Description: Water cycle configuration window
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


#ifndef WINWATER_H_
#define WINWATER_H_

#include "Window.h"

/*const char* waterCycleButtonsText[_nWaterCycleButtons] = {
	"Mode:",
	"=",
	">",
	"=",
	">",
	"=",
	">",
}; */

const char nameWinWater[] PROGMEM = "- Watering Cycle -";

const char modeStr[] PROGMEM = "Mode:";
const char modeTimedS[] PROGMEM = "Timed     ";
const char modeContS[] PROGMEM = "Continuous";
const char waterTwo[] PROGMEM = "Water every:";
const char waterThree[] PROGMEM = "Active for:";

class WinWater: public Window {
	public:
		WinWater(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinWater(const WinWater &other);
		WinWater& operator=(const WinWater &other);
		~WinWater();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nWaterCycleButtons = _nFlowButtons + 7;
		uint8_t _waterCycleButtons[_nWaterCycleButtons];
		boolean _waterTimed;
		uint8_t _waterHour, _waterMin, _floodMin;
		void print();
};

#endif