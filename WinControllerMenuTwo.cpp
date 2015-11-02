#include "WinControllerMenuTwo.h"

WinControllerMenuTwo::WinControllerMenuTwo(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinControllerMenuTwo::WinControllerMenuTwo(const WinControllerMenuTwo &other) : Window(other) {
	for (uint8_t i = 0; i < _nControllerButtonsTwo; i++) {
		_controllerButtonsTwo[i] = other._controllerButtonsTwo[i];
	}
}
	
WinControllerMenuTwo& WinControllerMenuTwo::operator=(const WinControllerMenuTwo& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nControllerButtonsTwo; i++) {
		_controllerButtonsTwo[i] = other._controllerButtonsTwo[i];
	}
	return *this;
}

WinControllerMenuTwo::~WinControllerMenuTwo() {}
	
Window::Screen WinControllerMenuTwo::getType() const {
	return Window::ControllerSettingsTwo;
}

void WinControllerMenuTwo::printToggles() {
	_lcd->setFont(hallfetica_normal);
	//Serial ON/OFF
	if (_serialActive)
		_lcd->print(pmChar(onStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonTextTwo[2]),_yThreeLnsFirst+_bigFontSize*_yFactor3lines*2);
	else
		_lcd->print(pmChar(offStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(controllerButtonTextTwo[2]),_yThreeLnsFirst+_bigFontSize*_yFactor3lines*2);
}

void WinControllerMenuTwo::print() {
	_serialActive = _settings->getSerialDebug();
	
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	
	//Print bulletpoints & texts
	for (uint8_t i = 0; i < _nControllerButtonsTwo - _nFlowButtons; i++) {
		_lcd->print(pmChar(bulletStr),_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
		_controllerButtonsTwo[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,(char*)pgm_read_word(&controllerButtonTextTwo[i]));
	}
	printToggles();
}

//Draws entire screen Controller Settings
void WinControllerMenuTwo::draw() { 
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinControllerMenuTwo);
	addFlowButtons(true,false,true,_controllerButtonsTwo);
	print();
	_buttons.drawButtons();
}
 
//Redraws only controller settings text from inner temp vars
//Used when +- signs are pressed
void WinControllerMenuTwo::update() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setFont(hallfetica_normal);
	printToggles();
}

Window::Screen WinControllerMenuTwo::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _controllerButtonsTwo[0]) 
		return ControllerSettings;
	//Exit
	else if (buttonIndex == _controllerButtonsTwo[2]) 
		return MainScreen;
	//SD Card
	else if (buttonIndex == _controllerButtonsTwo[3]) 
		return SDCard;
	//Sensor polling
	else if (buttonIndex == _controllerButtonsTwo[4]) 
		return SensorPolling;
	//Serial debug toggle
	else if (buttonIndex == _controllerButtonsTwo[5]) {
		_serialActive = !_serialActive;
		_settings->setSerialDebug(_serialActive);
		update();
	}
	return None;
}
