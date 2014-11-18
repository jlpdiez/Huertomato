#include "WinSystemMenu.h"

WinSystemMenu::WinSystemMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSystemMenu::WinSystemMenu(const WinSystemMenu &other) : Window(other) { }
	
WinSystemMenu& WinSystemMenu::operator=(const WinSystemMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinSystemMenu::~WinSystemMenu() {}
	
Window::Screen WinSystemMenu::getType() const {
	return Window::SystemSettings;
}

void WinSystemMenu::print() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Print bulletpoints
	_lcd->setFont(various_symbols);
	//Before the buttons were adding there are the flow buttons
	for (int i = 0; i < nSystemButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	//Make menu buttons
	for (int i = 0; i < nSystemButtons - _nFlowButtons; i++) {
		systemButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,systemButtonText[i]);
	}
}

//Draws entire screen System Settings
void WinSystemMenu::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinSystemMenu);
	addFlowButtons(true,false,true,systemButtons);
	print();
	_buttons.drawButtons();
}
 
//Redraws only system settings text from inner temp vars
//Used when +- signs are pressed
void WinSystemMenu::update() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setFont(hallfetica_normal);
	//Watering at night ON/OFF
	if (_nightWater)
		_lcd->print(onStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(systemButtonText[_nFlowButtons+1]),_yThreeLnsFirst+_bigFontSize*_yFactor3lines*4);
	else
		_lcd->print(offStr,_xMenu+_bigFontSize*2+_bigFontSize*strlen(systemButtonText[_nFlowButtons+1]),_yThreeLnsFirst+_bigFontSize*_yFactor3lines*4);

}

Window::Screen WinSystemMenu::processTouch(const int x,const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == systemButtons[0]) { return MainMenu; }
	//Exit
	else if (buttonIndex == systemButtons[2]) { return MainScreen; }
	//Watering Cycle
	else if (buttonIndex == systemButtons[3]) { return WateringCycle; }
	//Sensor Alarms
	else if (buttonIndex == systemButtons[4]) { return Alarms; }
	//Sensor Calibration
	else if (buttonIndex == systemButtons[5]) { return SensorCalib; }
	//Pump Protection
	else if (buttonIndex == systemButtons[6]) { return Pump; }
	//Water at night Toggle
	else if (buttonIndex == systemButtons[7]) {
		_nightWater = !_nightWater;
		_settings->setNightWatering(_nightWater);
		update();
	}
	return None;
}