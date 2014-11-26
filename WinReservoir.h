// #############################################################################
//
// # Name       : WinReservoir
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 24.11.2014
//
// # Description: Reservoir module submenu. On/off toggle, alarms, calibration and pump protection
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

#ifndef WINRESERVOIR_H_
#define WINRESERVOIR_H_

#include "Window.h"

//TODO: Should have: ON/OFF toggle ¿first? (then  grey out or not remaining options)
//Should be cool to autodetect if sensors are unresponsive and change setting.reservoirModule
//And read hgere and grey out automatically
//Alarms menu
//Sensor Calibration
//Pump protection

const int nReservoirButtons = 7;
static char* reservoirButtonsText[nReservoirButtons] = {
	"Reservoir Module:",
	"Sensor Alarms",
	"Pump Protection",
	"Sensor Calibration"
};
static int reservoirButtons[nReservoirButtons];

static char* nameWinReservoir = "- Reservoir Module -";

class WinReservoir: public Window {
	public:
		WinReservoir(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinReservoir(const WinReservoir &other);
		WinReservoir& operator=(const WinReservoir &other);
		~WinReservoir();
		Screen getType() const;
		void draw();
		Window::Screen processTouch(const int x, const int y);
		
	protected:	
		boolean _reservoirActive;
		void print();
};

#endif 