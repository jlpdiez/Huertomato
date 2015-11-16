#include "WinControllerMenu.h"

WinControllerMenu::WinControllerMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinControllerMenu::WinControllerMenu(const WinControllerMenu &other) : Window(other) {
	for (uint8_t i = 0; i < _nControllerButtons; i++) {
		_controllerButtons[i] = other._controllerButtons[i];
	}
}
	
WinControllerMenu& WinControllerMenu::operator=(const WinControllerMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nControllerButtons; i++) {
		_controllerButtons[i] = other._controllerButtons[i];
	}
	return *this;
}

WinControllerMenu::~WinControllerMenu() {}
	
Window::Screen WinControllerMenu::getType() const {
	return Window::ControllerSettings;
}

void WinControllerMenu::printToggles() {
	_lcd->setFont(hallfetica_normal);
	//LED ON/OFF
	if (_ledActive)
		_lcd->print(pmChar(onStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonText[1]),_yFiveLines+_bigFontSize*_yFactor5lines*1);
	else
		_lcd->print(pmChar(offStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonText[1]),_yFiveLines+_bigFontSize*_yFactor5lines*1);
	//Sound ON/OFF
	if (_soundActive)
		_lcd->print(pmChar(onStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonText[2]),_yFiveLines+_bigFontSize*_yFactor5lines*2);
	else
		_lcd->print(pmChar(offStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonText[2]),_yFiveLines+_bigFontSize*_yFactor5lines*2);
	//Units C/F
	if (_celsiusActive)
		_lcd->print(pmChar(celsStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonText[3]),_yFiveLines+_bigFontSize*_yFactor5lines*3);
	else
		_lcd->print(pmChar(farenhStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonText[3]),_yFiveLines+_bigFontSize*_yFactor5lines*3);
}

void WinControllerMenu::print() {
	_soundActive = _settings->getSound();
	_ledActive = _settings->getLed();
	_celsiusActive = _settings->getCelsius();
	
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	
	//Print bulletpoints & texts
	for (uint8_t i = 0; i < _nControllerButtons - _nFlowButtons; i++) {
		_lcd->print(pmChar(bulletStr),_xMenu,_yFiveLines+_bigFontSize*_yFactor5lines*i);
		_controllerButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yFiveLines+_bigFontSize*_yFactor5lines*i,(char*)pgm_read_word(&controllerButtonText[i]));
	}
	printToggles();
}

//Draws entire screen Controller Settings
void WinControllerMenu::draw() { 
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinControllerMenu);
	addFlowButtons(true,false,true,_controllerButtons);
	print();
	_buttons.drawButtons();
}
 
//Redraws only controller settings text from inner temp vars
//Used when +- signs are pressed
void WinControllerMenu::update() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setFont(hallfetica_normal);
	printToggles();
}

Window::Screen WinControllerMenu::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _controllerButtons[0]) 
		return MainMenu;
	//Exit
	else if (buttonIndex == _controllerButtons[2]) 
		return MainScreen;
	//Time & Date
	else if (buttonIndex == _controllerButtons[3]) 
		return TimeDate;
	//LED
	else if (buttonIndex == _controllerButtons[4]) {
		_ledActive = !_ledActive;
		_settings->setLed(_ledActive);
		update();
	//Sound
	} else if (buttonIndex == _controllerButtons[5]) {
		_soundActive = !_soundActive;
		_settings->setSound(_soundActive);
		update();
	//Unit
	} else if (buttonIndex == _controllerButtons[6]) {
		_celsiusActive = !_celsiusActive;
		_settings->setCelsius(_celsiusActive);
		_sensors->setCelsius(_celsiusActive);
		update();
	//More
	} else if (buttonIndex == _controllerButtons[7])
		return ControllerSettingsTwo;
	return None;
}
