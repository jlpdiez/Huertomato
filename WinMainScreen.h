// #############################################################################
//
// # Name       : WinMainScreen
// # Version    : 0.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 21.12.2015
//
// # Description: Main idle screen window
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


#ifndef WINMAINSCREEN_H_
#define WINMAINSCREEN_H_

#include "Sensors.h"
#include "Settings.h"
#include "Window.h"  
#include <Time.h>

const char phAlarmUp[] PROGMEM = "pH alto";
const char phAlarmUp1[] PROGMEM = "agregue pH Down";
const char phAlarmDown[] PROGMEM = "pH bajo";
const char phAlarmDown1[] PROGMEM = "agregue pH Up";

const char ecAlarmUp[] PROGMEM = "EC alta";
const char ecAlarmUp1[] PROGMEM = "agregue agua";
const char ecAlarmDown[] PROGMEM = "EC baja";
const char ecAlarmDown1[] PROGMEM = "mas nutrientes";

const char lvlAlarm[] PROGMEM = "Nivel bajo";
const char lvlAlarm1[] PROGMEM = "Rellene depo";
const char pumpAlarm[] PROGMEM = "Nivel critico!";
const char pumpAlarm1[] PROGMEM = "NO se regara!";

class WinMainScreen: public Window {
	public:	
		WinMainScreen(LiquidCrystal *lcd, Sensors *sensors, Settings *settings);
		WinMainScreen(const WinMainScreen &other);
		WinMainScreen& operator=(const WinMainScreen &other);
		~WinMainScreen();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(int);
	protected:
		bool _alarm;
		float _temp;
		uint8_t _waterLvl;
		float _ph;
		float _ec;
};

#endif 