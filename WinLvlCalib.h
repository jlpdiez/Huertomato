// #############################################################################
//
// # Name       : WinLvlCalib
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
//
// # Description: Nutrient level max/min calibration window
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


#ifndef WINLVLCALIB_H_
#define WINLVLCALIB_H_

#include "Window.h" 

const int nWaterLevelButtons = 5;
static char* waterLevelButtonsText[nWaterLevelButtons] = {
	"Set",
	"Set"
};
static int waterLevelButtons[nWaterLevelButtons];

static char* nameWinLvlCalib = "- Nutrient Levels -";
static char* firstLvlCalib = "Current Reading:";
static char* secondLvlCalib = "Current Top:";
static char* thirdLvlCalib = " and Bottom:";
static char* unitLvl = "cm";

class WinLvlCalib: public Window {
	public:
		WinLvlCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinLvlCalib(const WinLvlCalib &other);
		WinLvlCalib& operator=(const WinLvlCalib &other);
		~WinLvlCalib();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const int _yFirstLine = 50;
		static const int _ySecondLine = 100;
		static const int _yThirdLine = 150;
		static const int _xSpacer = 25;
		uint16_t _waterLvlMax, _waterLvlMin, _rawWaterLvl;
		void print();
};



#endif