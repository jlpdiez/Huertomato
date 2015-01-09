#include "WinSD.h"

WinSD::WinSD(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSD::WinSD(const WinSD &other) : Window(other) {
	for (int i = 0; i < _nSDcardButtons; i++) {
		_sdCardButtons[i] = other._sdCardButtons[i];
	}
}
	
WinSD& WinSD::operator=(const WinSD& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nSDcardButtons; i++) {
		_sdCardButtons[i] = other._sdCardButtons[i];
	}
	return *this;
}

WinSD::~WinSD() {}
	
Window::Screen WinSD::getType() const {
	return Window::SDCard;
}

void WinSD::print() {	
	const int houU[] = {217, _yTwoLnsSecond-22};       //hour up
	const int minU[] = {280, _yTwoLnsSecond-22};       //min up
	const int houD[] = {217, _yTwoLnsSecond+22};       //hour down
	const int minD[] = {280, _yTwoLnsSecond+22};       //min down
	
	_sdActive = _settings->getSDactive();
	_sdHour = _settings->getSDhour();
	_sdMin = _settings->getSDminute();
	
	//Triangle symbol
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print(bulletStr,_xConfig,_yTwoLnsFirst);
	//First line button
	_sdCardButtons[_nFlowButtons] = _buttons.addButton(_xConfig+2*_bigFontSize,_yTwoLnsFirst,sdCardButtonsText[0]);
	//On - off symbol -
	_lcd->setFont(hallfetica_normal);
	if (_sdActive)
		_lcd->print(onStr,_xConfig+((3+strlen_P(sdCardButtonsText[0]))*_bigFontSize),_yTwoLnsFirst);
	else
		_lcd->print(offStr,_xConfig+((3+strlen_P(sdCardButtonsText[0]))*_bigFontSize),_yTwoLnsFirst);
	
	//Second line
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->print(sdCardText1,_xConfig,_yTwoLnsSecond);
	//hours
	int x = houU[0]+_bigFontSize/2-_bigFontSize+2;
	_lcd->printNumI(_sdHour,x,_yTwoLnsSecond,2,'0');
	x += 2*_bigFontSize;
	_lcd->print("h",x,_yTwoLnsSecond);
	//Mins
	x += 2*_bigFontSize;
	_lcd->printNumI(_sdMin,x,_yTwoLnsSecond,2,'0');
	x += 2*_bigFontSize;
	_lcd->print("m",x,_yTwoLnsSecond);
	
	//Make +/- buttons
	_sdCardButtons[_nFlowButtons+1] = _buttons.addButton(houU[0],houU[1],sdCardButtonsText[1],BUTTON_SYMBOL);
	_sdCardButtons[_nFlowButtons+2] = _buttons.addButton(minU[0],minU[1],sdCardButtonsText[2],BUTTON_SYMBOL);
	_sdCardButtons[_nFlowButtons+3] = _buttons.addButton(houD[0],houD[1],sdCardButtonsText[3],BUTTON_SYMBOL);
	_sdCardButtons[_nFlowButtons+4] = _buttons.addButton(minD[0],minD[1],sdCardButtonsText[4],BUTTON_SYMBOL);
	
	//If first toggle is inactive we grey out buttons
	if (!_sdActive) {
		for (int i = 4; i < _nSDcardButtons; i++)
			_buttons.disableButton(_sdCardButtons[i],true);
		} else {
			for (int i = 4; i < _nSDcardButtons; i++)
				_buttons.enableButton(_sdCardButtons[i],true);
	}
} 

//Draws entire screen SD Card
void WinSD::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinSD);
	addFlowButtons(true,true,true,_sdCardButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only SD numbers & text from inner temp vars
//Used when +- signs are pressed
void WinSD::update() {
	const int houU[] = {217, _yTwoLnsSecond-22};       //hour up
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	if (_sdActive)
		_lcd->print(onStr,_xConfig+((3+strlen_P(sdCardButtonsText[0]))*_bigFontSize),_yTwoLnsFirst);
	else
		_lcd->print(offStr,_xConfig+((3+strlen_P(sdCardButtonsText[0]))*_bigFontSize),_yTwoLnsFirst);

	_lcd->setColor(grey[0],grey[1],grey[2]);
	//hours
	int x = houU[0]+_bigFontSize/2-_bigFontSize+2;
	_lcd->printNumI(_sdHour,x,_yTwoLnsSecond,2,'0');
	x += 2*_bigFontSize;
	//Mins
	x += 2*_bigFontSize;
	_lcd->printNumI(_sdMin,x,_yTwoLnsSecond,2,'0');
	
	//If first toggle is inactive we grey out buttons
	if (!_sdActive) {
		for (int i = 4; i < _nSDcardButtons; i++)
		_buttons.disableButton(_sdCardButtons[i],true);
		} else {
		for (int i = 4; i < _nSDcardButtons; i++)
		_buttons.enableButton(_sdCardButtons[i],true);
	}
}

Window::Screen WinSD::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _sdCardButtons[0]) { return ControllerSettings; }
	//Save
	else if (buttonIndex == _sdCardButtons[1]) {
		_settings->setSDactive(_sdActive);
		_settings->setSDhour(_sdHour);
		_settings->setSDminute(_sdMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _sdCardButtons[2]) { return MainScreen; }
				
	//Card active toggle
	else if (buttonIndex == _sdCardButtons[3]) {
		_sdActive = !_sdActive;
		update();
	//Hour up
	} else if (buttonIndex == _sdCardButtons[4]) {
		(_sdHour >= 23) ? _sdHour=0 : _sdHour++;
		update();
	//Minute up
	} else if (buttonIndex == _sdCardButtons[5]) {
		(_sdMin >= 59) ? _sdMin=0 : _sdMin++;
		update();
	//Hour down
	} else if (buttonIndex == _sdCardButtons[6]) {
		(_sdHour <= 0) ? _sdHour=23 : _sdHour--;
		update();
	//Minute downs
	} else if (buttonIndex == _sdCardButtons[7]) {
		(_sdMin <= 0) ? _sdMin=59 : _sdMin--;
		update();
	}
	return None;
}
