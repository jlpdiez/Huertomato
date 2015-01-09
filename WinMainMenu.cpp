#include "WinMainMenu.h"

WinMainMenu::WinMainMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinMainMenu::WinMainMenu(const WinMainMenu &other) : Window(other) {
		for (int i = 0; i < _nMainMenuButtons; i++) {
			_mainMenuButtons[i] = other._mainMenuButtons[i];
		}
}
	
WinMainMenu& WinMainMenu::operator=(const WinMainMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nMainMenuButtons; i++) {
		_mainMenuButtons[i] = other._mainMenuButtons[i];
	}
	return *this;
}

WinMainMenu::~WinMainMenu() {}
	
Window::Screen WinMainMenu::getType() const {
	return Window::MainMenu;
}

//Makes window decoration and buttons
void WinMainMenu::print() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(hallfetica_normal);
	
	//Make menu buttons. System and Controller settings
	_mainMenuButtons[_nFlowButtons] = _buttons.addButton(_xMenu+70,_yTwoLnsFirst,pmChar(menuStr0));
	_mainMenuButtons[_nFlowButtons+1] = _buttons.addButton(_xMenu+70,_yTwoLnsSecond,pmChar(menuStr1));
	//Logos
	_lcd->drawBitmap (_xMenu, _yTwoLnsFirst-18, _smallIconSize, _smallIconSize, plant64);
	_lcd->drawBitmap (_xMenu, _yTwoLnsSecond-18, _smallIconSize, _smallIconSize, settings64);
	//With transparent buttons
	_mainMenuButtons[_nFlowButtons+2] = _buttons.addButton(_xMenu, _yTwoLnsFirst-18, _smallIconSize, _smallIconSize, 0);
	_mainMenuButtons[_nFlowButtons+3] = _buttons.addButton(_xMenu, _yTwoLnsSecond-18, _smallIconSize, _smallIconSize, 0);
}

//Draws main menu into LCD
void WinMainMenu::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinMainMenu);
	addFlowButtons(false,false,true,_mainMenuButtons);
	print();
	_buttons.drawButtons();
}
 
// Processes touch for main menu screen
Window::Screen WinMainMenu::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Exit
	if (buttonIndex == _mainMenuButtons[2]) { return MainScreen; }
	//System Settings
	else if ((buttonIndex == _mainMenuButtons[3]) || (buttonIndex == _mainMenuButtons[5])) { return SystemSettings; }
	//Controller Settings
	else if ((buttonIndex == _mainMenuButtons[4]) || (buttonIndex == _mainMenuButtons[6])) { return ControllerSettings; }
	return None;
}