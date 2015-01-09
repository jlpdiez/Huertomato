#include "WinPump.h"

WinPump::WinPump(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinPump::WinPump(const WinPump &other) : Window(other) {
	for (int i = 0; i < _nPumpProtectionButtons; i++) {
		_pumpProtectionButtons[i] = other._pumpProtectionButtons[i];
	}
}
	
WinPump& WinPump::operator=(const WinPump& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nPumpProtectionButtons; i++) {
		_pumpProtectionButtons[i] = other._pumpProtectionButtons[i];
	}
	return *this;
}

WinPump::~WinPump() {}
	
Window::Screen WinPump::getType() const {
	return Window::Pump;
}

void WinPump::print() {	
	_pumpProtectionLvl = _settings->getPumpProtectionLvl();	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(wPumpLimit,_xConfig,_yOneLine);
	//Numbers
	int x = (4+strlen_P(wPumpLimit))*_bigFontSize;
	_lcd->printNumI(_pumpProtectionLvl,x,_yOneLine,3);
	//Buttons
	x += 1.5*_bigFontSize;
	_pumpProtectionButtons[_nFlowButtons] = _buttons.addButton(x,_yOneLine-_signSpacer,pumpProtectionButtonsText[0],BUTTON_SYMBOL);
	_pumpProtectionButtons[_nFlowButtons+1] = _buttons.addButton(x,_yOneLine+_signSpacer,pumpProtectionButtonsText[1],BUTTON_SYMBOL);
	//percent sign
	x += 2.5*_bigFontSize;
	_lcd->print("%",x,_yOneLine);
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
	int x = (4+strlen_P(wPumpLimit))*_bigFontSize;
	_lcd->printNumI(_pumpProtectionLvl,x,_yOneLine,3);
}

Window::Screen WinPump::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _pumpProtectionButtons[0]) 
		return Reservoir;
	//Save
	else if (buttonIndex == _pumpProtectionButtons[1]) {
		_settings->setPumpProtectionLvl(_pumpProtectionLvl);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _pumpProtectionButtons[2]) 
		return MainScreen;
		
	//Up
	else if (buttonIndex == _pumpProtectionButtons[3]) {
		(_pumpProtectionLvl >= 100) ? _pumpProtectionLvl=0 : _pumpProtectionLvl++;
		update();
	//Down
	} else if (buttonIndex == _pumpProtectionButtons[4]) {
		(_pumpProtectionLvl <= 0) ? _pumpProtectionLvl=100 : _pumpProtectionLvl--;
		update();
	}
	return None;
}