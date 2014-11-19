// #############################################################################
//
// # Name       : WinWaterNight
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 19.11.2014
//
// # Description: Night Watering Settings. Toggle on/off and set threshold
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


#ifndef WINWATERNIGHT_H_
#define WINWATERNIGHT_H_

#include "Window.h"

const int nWaterNightButtons = 4;
static char* waterNightButtonsText[nWaterNightButtons] = {
	"Set"
};
static int waterNightButtons[nWaterNightButtons];

static char* nameWinWaterNight = "- Night Calibration -";
static char* rawLight = "Current Reading:";
static char* lightThreshold = "Threshold:";

//TODO: Should contain LightCalib and on/off toggle with 3 lines, with toggle first like inSD
class WinWaterNight: public Window {
	public:
	WinWaterNight(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
	WinWaterNight(const WinWaterNight &other);
	WinWaterNight& operator=(const WinWaterNight &other);
	~WinWaterNight();
	Screen getType() const;
	void draw();
	void update();
	Window::Screen processTouch(const int x, const int y);
	
	protected:
	uint16_t _lightThreshold, _rawLightLvl;
	void print();
};

#endif