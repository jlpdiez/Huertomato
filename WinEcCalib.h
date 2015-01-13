// #############################################################################
//
// # Name       : WinEcCalib
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
//
// # Description: EC circuit calibration menu window
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


#ifndef WINECCALIB_H_
#define WINECCALIB_H_

#include "Window.h"

/*const char ecCalibrationButtonsText[nECcalibrationButtons] = {
	"1. Set probe type",
	"2. Dry calibration",
	"3. Calibrate for 40000 uS",
	"4. Calibrate for 10500 uS"
};*/

const char nameWinEcCalib[] PROGMEM = "EC Circuit";

class WinEcCalib: public Window {
	public:
		WinEcCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinEcCalib(const WinEcCalib &other);
		WinEcCalib& operator=(const WinEcCalib &other);
		~WinEcCalib();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nECcalibrationButtons = _nFlowButtons + 4;		
		int8_t _ecCalibrationButtons[_nECcalibrationButtons];
		void print();
};

#endif