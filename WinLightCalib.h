// #############################################################################
//
// # Name       : WinLightCalib
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 19.11.2014
//
// # Description: Superclass window. holds types of screens, and common variables.
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


#ifndef WINLIGHTCALIB_H_
#define WINLIGHTCALIB_H_

#include "Window.h"

const int nLightCalibrationButtons = 4;
static char* lightCalibrationButtonsText[nLightCalibrationButtons] = {
	"Set"
};
static int lightCalibrationButtons[nLightCalibrationButtons];

static char* nameWinLightCalib = "- Night Calibration -";
//static char* rawLight = "Current Reading:";
//static char* lightThreshold = "Threshold:";

class WinLightCalib: public Window {
	public:
		WinLightCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinLightCalib(const WinLightCalib &other);
		WinLightCalib& operator=(const WinLightCalib &other);
		~WinLightCalib();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		uint16_t _lightThreshold, _rawLightLvl;
		void print();
};



#endif