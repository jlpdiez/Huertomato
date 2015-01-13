// #############################################################################
//
// # Name       : WinWaterNight
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
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

const char nameWinWaterNight[] PROGMEM = "Night Calibration";

const char rawLight[] PROGMEM = "Actual Light:";
const char lightThreshold[] PROGMEM = "Threshold:";
const char newLightUnit[] PROGMEM = "Lux";

const char waterNightStr0[] PROGMEM = "Water at night:";
const char waterNightStr1[] PROGMEM = "Set";

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
		static const uint8_t _nWaterNightButtons = _nFlowButtons + 2;
		int8_t _waterNightButtons[_nWaterNightButtons];
		boolean _nightWater;
		float _lightThreshold, _rawLightLvl;
		void print();
};

#endif