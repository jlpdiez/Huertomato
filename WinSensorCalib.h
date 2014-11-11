// #############################################################################
//
// # Name       : WinSensorCalib
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.11.2014
//
// # Description: Sensor calibration menu window
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


#ifndef WINSENSORCALIB_H_
#define WINSENSORCALIB_H_

#include "Window.h"

const int nSensorCalibrationButtons = 5;
static char* sensorCalibrationButtonsText[nSensorCalibrationButtons] = {
	"Water Levels",
	"Night Threshold"
};
static int sensorCalibrationButtons[nSensorCalibrationButtons];

static char* nameWinSensorCalib = "- Calibration -";

class WinSensorCalib: public Window {
	public:
		WinSensorCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSensorCalib(const WinSensorCalib &other);
		WinSensorCalib& operator=(const WinSensorCalib &other);
		~WinSensorCalib();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const int _xSpacer = 15;
		static const int _ySpacer = 45;
		static const int _yFirst = 60;
		static const int _ySecond = 135;
		void print();
};

#endif