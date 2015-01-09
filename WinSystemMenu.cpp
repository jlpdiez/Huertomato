#include "WinSystemMenu.h"

WinSystemMenu::WinSystemMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSystemMenu::WinSystemMenu(const WinSystemMenu &other) : Window(other) {
	for (int i = 0; i < _nSystemButtons; i++) {
		_systemButtons[i] = other._systemButtons[i];
	}
}
	
WinSystemMenu& WinSystemMenu::operator=(const WinSystemMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nSystemButtons; i++) {
		_systemButtons[i] = other._systemButtons[i];
	}
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
	for (int i = 0; i < _nSystemButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	//Make menu buttons
	for (int i = 0; i < _nSystemButtons - _nFlowButtons; i++) {
		_systemButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,pmChar(systemButtonText[i]));
	}
}

//Draws entire screen System Settings
void WinSystemMenu::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinSystemMenu);
	addFlowButtons(true,false,true,_systemButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinSystemMenu::processTouch(const int x,const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _systemButtons[0])
		return MainMenu;
	//Exit
	else if (buttonIndex == _systemButtons[2]) 
		return MainScreen;
	//Watering Cycle
	else if (buttonIndex == _systemButtons[3]) 
		return WateringCycle;
	//Night Watering
	else if (buttonIndex == _systemButtons[4]) 
		return NightWater;
	//Reservoir Module
	else if (buttonIndex == _systemButtons[5]) 
		return Reservoir;
	return None;
}