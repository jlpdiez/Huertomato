// #############################################################################
//
// # Name       : WinMainScreen
// # Version    : 0.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 17.12.2015
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
#include <SD.h>   
#include <Time.h>

//Contains two main screens. One when reservoir module is on, another when off
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
};

#endif 