// #############################################################################
//
// # Name       : WinWater
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
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

//Here lies the button info for each window
//We add 3 to all nButtons to account for back/save/exit (These are always 0,1,2)
//static in this context means vars will only be accesible in this file
//http://www.cprogramming.com/tutorial/statickeyword.html
const int nWaterCycleButtons = 10;
static char* waterCycleButtonsText[nWaterCycleButtons] = {
	"Mode:",
	"=",
	">",
	"=",
	">",
	"=",
	">",
}; 
static int  waterCycleButtons[nWaterCycleButtons];

static char* nameWinWater = "- Watering Cycle -";
static char* modeTimedS = "Timed     ";
static char* modeContS = "Continuous";
static char* waterTwo = "Water every:";
static char* waterThree = "Active for:";

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
		static const int _yFirstLine = 50;
		static const int _ySecondLine = 100;
		static const int _yThirdLine = 160;
		static const int _xSpacer = 25;
		//These are temp variables used for displaying data
		//They are read from _settings in print() funcs. Changed in processTouch()
		//displayed again with update() and saved to eeprom when button save is pressed
		boolean _waterTimed;
		uint8_t _waterHour, _waterMin, _floodMin;
		void print();
};

#endif