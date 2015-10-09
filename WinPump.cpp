#include "WinPump.h"

WinPump::WinPump(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinPump::WinPump(const WinPump &other) : Window(other) {
	for (uint8_t i = 0; i < _nPumpProtectionButtons; i++) {
		_pumpProtectionButtons[i] = other._pumpProtectionButtons[i];
	}
}
	
WinPump& WinPump::operator=(const WinPump& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nPumpProtectionButtons; i++) {
		_pumpProtectionButtons[i] = other._pumpProtectionButtons[i];
	}
	return *this;
}

WinPump::~WinPump() {}
	
Window::Screen WinPump::getType() const {
	return Window::Pump;
}

void WinPump::print() {	
	_pumpProtection = _settings->getPumpProtection();
	_pumpProtectionLvl = _settings->getPumpProtectionLvl();	
	
	//Triangle symbol
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print(pmChar(bulletStr),_xConfig,_yTwoLnsFirst);
	//First line button
	_lcd->setFont(hallfetica_normal);
	_pumpProtectionButtons[_nFlowButtons] = _buttons.addButton(_xConfig+2*_bigFontSize,_yTwoLnsFirst,pumpProtTxt);
	//ON/OFF
	if (_pumpProtection)
		_lcd->print(pmChar(onStr),_xConfig+((3+strlen_P(pumpProtTxt))*_bigFontSize),_yTwoLnsFirst);
	else
		_lcd->print(pmChar(offStr),_xConfig+((3+strlen_P(pumpProtTxt))*_bigFontSize),_yTwoLnsFirst);
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(pmChar(wPumpLimit),_xConfig,_yTwoLnsSecond);
	//Numbers
	int x = (3+strlen_P(wPumpLimit))*_bigFontSize;
	_lcd->printNumI(_pumpProtectionLvl,x,_yTwoLnsSecond,3);
	//Buttons
	x += 1.5*_bigFontSize;
	_pumpProtectionButtons[_nFlowButtons+1] = _buttons.addButton(x,_yTwoLnsSecond-_signSpacer,plusStr,BUTTON_SYMBOL);
	_pumpProtectionButtons[_nFlowButtons+2] = _buttons.addButton(x,_yTwoLnsSecond+_signSpacer,minusStr,BUTTON_SYMBOL);
	//percent sign
	x += 2.5*_bigFontSize;
	_lcd->print(pmChar(percentSign),x,_yTwoLnsSecond);
	
	//If first toggle is inactive we grey out buttons
	if (!_pumpProtection) {
		for (uint8_t i = 4; i < _nPumpProtectionButtons; i++)
			_buttons.disableButton(_pumpProtectionButtons[i],true);
	} else {
		for (uint8_t i = 4; i < _nPumpProtectionButtons; i++)
			_buttons.enableButton(_pumpProtectionButtons[i],true);
	}
}

//Draws entire screen Pump Protection
void WinPump::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinPump);
	addFlowButtons(true,true,true,_pumpProtectionButtons);
	print();
	_buttons.drawButtons();
}
 
void WinPump::update() {
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0],grey[1],grey[2]);
	int x = (3+strlen_P(wPumpLimit))*_bigFontSize;
	_lcd->printNumI(_pumpProtectionLvl,x,_yTwoLnsSecond,3);
	//ON/OFF
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	if (_pumpProtection)
		_lcd->print(pmChar(onStr),_xConfig+((3+strlen_P(pumpProtTxt))*_bigFontSize),_yTwoLnsFirst);
	else
		_lcd->print(pmChar(offStr),_xConfig+((3+strlen_P(pumpProtTxt))*_bigFontSize),_yTwoLnsFirst);
	//If first toggle is inactive we grey out buttons
	if (!_pumpProtection) {
		for (uint8_t i = 4; i < _nPumpProtectionButtons; i++)
			_buttons.disableButton(_pumpProtectionButtons[i],true);
	} else {
		for (uint8_t i = 4; i < _nPumpProtectionButtons; i++)
			_buttons.enableButton(_pumpProtectionButtons[i],true);
	}
}

Window::Screen WinPump::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _pumpProtectionButtons[0]) 
		return Reservoir;
	//Save
	else if (buttonIndex == _pumpProtectionButtons[1]) {
		_settings->setPumpProtection(_pumpProtection);
		_settings->setPumpProtectionLvl(_pumpProtectionLvl);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _pumpProtectionButtons[2]) 
		return MainScreen;
		
	//On/off toggle
	else if (buttonIndex == _pumpProtectionButtons[3]) {
		_pumpProtection = !_pumpProtection;
		update();
	//Up
	} else if (buttonIndex == _pumpProtectionButtons[4]) {
		(_pumpProtectionLvl >= 100) ? _pumpProtectionLvl=0 : _pumpProtectionLvl++;
		update();
	//Down
	} else if (buttonIndex == _pumpProtectionButtons[5]) {
		(_pumpProtectionLvl <= 0) ? _pumpProtectionLvl=100 : _pumpProtectionLvl--;
		update();
	}
	return None;
}