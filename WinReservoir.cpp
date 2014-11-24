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