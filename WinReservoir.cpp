#include "WinReservoir.h"

WinReservoir::WinReservoir(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) { }
	
WinReservoir::WinReservoir(const WinReservoir &other) : Window(other) { }
	
WinReservoir& WinReservoir::operator=(const WinReservoir &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinReservoir::~WinReservoir() { }
	
Window::Screen WinReservoir::getType() const {
	return Window::Reservoir;
}

void WinReservoir::print() {
	static const int _yFactor4lines = 2.5;
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Print bulletpoints
	_lcd->setFont(various_symbols);
	//Before the buttons were adding there are the flow buttons
	for (int i = 0; i < nuttons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	//Make menu buttons
	for (int i = 0; i < nSystemButtons - _nFlowButtons; i++) {
		systemButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,systemButtonText[i]);
	}
}
}

void WinReservoir::update() {
	
}

void WinReservoir::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinReservoir);
	addFlowButtons(true,false,true,reservoirButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinReservoir::processTouch(const int x, const int y) {
		int buttonIndex = _buttons.checkButtons(x,y);
		//Back
		if (buttonIndex == reservoirButtons[0])
			return SystemSettings;
		//Exit
		else if (buttonIndex == reservoirButtons[2])
			return MainScreen;
		
		//On/Off toggle
		else if (buttonIndex == reservoirButtons[3]) {
			_reservoirActive = !_reservoirActive;
			update();
		} else if (buttonIndex == reservoirButtons[4])
			return Alarms;
		else if (buttonIndex == reservoirButtons[5])
			return Pump;
		else if (buttonIndex == reservoirButtons[6])
			return SensorCalib;
		return None;
}