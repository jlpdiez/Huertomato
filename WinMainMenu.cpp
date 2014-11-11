#include "WinMainMenu.h"

WinMainMenu::WinMainMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinMainMenu::WinMainMenu(const WinMainMenu &other) : Window(other) { }
	
WinMainMenu& WinMainMenu::operator=(const WinMainMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
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
	mainMenuButtons[3] = _buttons.addButton(_xSpacer+70,_systemY,mainMenuButtonText[0]);
	mainMenuButtons[4] = _buttons.addButton(_xSpacer+70,_controllerY,mainMenuButtonText[1]);
	//Logos
	_lcd->drawBitmap (_xSpacer, _systemY-18, _iconSize, _iconSize, plant64);
	_lcd->drawBitmap (_xSpacer, _controllerY-18, _iconSize, _iconSize, settings64);
	//With transparent buttons
	mainMenuButtons[5] = _buttons.addButton(_xSpacer, _systemY-18, _iconSize, _iconSize, 0);
	mainMenuButtons[6] = _buttons.addButton(_xSpacer, _controllerY-18, _iconSize, _iconSize, 0);
}

//Draws main menu into LCD
void WinMainMenu::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinMainMenu);
	addFlowButtons(false,false,true,mainMenuButtons);
	print();
	_buttons.drawButtons();
}
 
// Processes touch for main menu screen
Window::Screen WinMainMenu::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Exit
	if (buttonIndex == mainMenuButtons[2]) { return MainScreen; }
	//System Settings
	else if ((buttonIndex == mainMenuButtons[3]) || (buttonIndex == mainMenuButtons[5])) { return SystemSettings; }
	//Controller Settings
	else if ((buttonIndex == mainMenuButtons[4]) || (buttonIndex == mainMenuButtons[6])) { return ControllerSettings; }
	return None;
}