// #############################################################################
//
// # Name       : WinPhCalib
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 09.01.2015
//
// # Description: pH circuit calibration menu window
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


#ifndef WINPHCALIB_H_
#define WINPHCALIB_H_

#include "Window.h"

/*const char phCalibrationButtonsText[nPHcalibrationButtons] = {
	"1. Calibrate for pH7",
	"2. Calibrate for pH4",
	"3. Calibrate for pH10"
};*/

const char nameWinPhCalib[] PROGMEM = "- pH Circuit -";

const char startCalibStr1[] PROGMEM = "Start calibration";
const char startCalibStr2[] PROGMEM = "procedure now?";
const char yesStr[] PROGMEM = "Yes";

const char phText1[] PROGMEM = "Rinse & dry probe,";
const char phText2[] PROGMEM = "submerge in pH";
const char phText3[] PROGMEM = "and";
const char phText4[] PROGMEM = "wait 5 mins.";
const char continueStr[] PROGMEM = "Continue";

class WinPhCalib: public Window {
	public:
		WinPhCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinPhCalib(const WinPhCalib &other);
		WinPhCalib& operator=(const WinPhCalib &other);
		~WinPhCalib();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		static const uint8_t _nPHcalibrationButtons = _nFlowButtons + 1;
		uint8_t _phCalibrationButtons[_nPHcalibrationButtons];
		//Stores screen state:
		//0: start screen, 1: pH7, 2: pH4, 3: pH10
		uint8_t _actScreen;
		void print();
};


#endif