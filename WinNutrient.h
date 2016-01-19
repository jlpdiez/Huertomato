// #############################################################################
//
// # Name       : WinWater
// # Version    : 0.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 18.01.2016
//
// # Description: Nutrient change counter window
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


#ifndef WINNUTRIENT_H_
#define WINNUTRIENT_H_

#include "Window.h"

const char cambioTxt[] PROGMEM = "Cambio deposito:";
const char cambioTxt1[] PROGMEM = "dias";
const char pasadoTxT[] PROGMEM = "necesario";

class WinNutrient: public Window {
	public:
		WinNutrient(LiquidCrystal *lcd, Sensors *sensors, Settings *settings);
		WinNutrient(const WinNutrient &other);
		WinNutrient& operator=(const WinNutrient &other);
		~WinNutrient();
		Screen getType() const;
		void draw();
		void update();
		Window::Screen processTouch(int);
};

#endif