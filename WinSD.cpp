#include "WinSD.h"

WinSD::WinSD(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSD::WinSD(const WinSD &other) : Window(other) { }
	
WinSD& WinSD::operator=(const WinSD& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinSD::~WinSD() {}
	
Window::Screen WinSD::getType() const {
	return Window::SDCard;
}

void WinSD::print() {	
	const int houU[] = {217, _ySecondLine-22};       //hour up
	const int minU[] = {280, _ySecondLine-22};       //min up
	const int houD[] = {217, _ySecondLine+22};       //hour down
	const int minD[] = {280, _ySecondLine+22};       //min down
	
	_sdActive = _settings->getSDactive();
	_sdHour = _settings->getSDhour();
	_sdMin = _settings->getSDminute();
	
	//Triangle symbol
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print(bulletStr,_xSpacer,_yFirstLine);
	//First line button
	sdCardButtons[3] = _buttons.addButton(_xSpacer+2*_bigFontSize,_yFirstLine,sdCardButtonsText[0]);
	//On - off symbol -
	_lcd->setFont(hallfetica_normal);
	if (_sdActive)
		_lcd->print(onStr,_xSpacer+((3+strlen(sdCardButtonsText[0]))*_bigFontSize),_yFirstLine);
	else
		_lcd->print(offStr,_xSpacer+((3+strlen(sdCardButtonsText[0]))*_bigFontSize),_yFirstLine);
	
	//Second line
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->print(sdCardText1,_xSpacer,_ySecondLine);
	//hours
	int x = houU[0]+_bigFontSize/2-_bigFontSize+2;
	_lcd->printNumI(_sdHour,x,_ySecondLine,2,'0');
	x += 2*_bigFontSize;
	_lcd->print("h",x,_ySecondLine);
	//Mins
	x += 2*_bigFontSize;
	_lcd->printNumI(_sdMin,x,_ySecondLine,2,'0');
	x += 2*_bigFontSize;
	_lcd->print("m",x,_ySecondLine);
	
	//Make +/- buttons
	sdCardButtons[4] = _buttons.addButton(houU[0],houU[1],sdCardButtonsText[1],BUTTON_SYMBOL);
	sdCardButtons[5] = _buttons.addButton(minU[0],minU[1],sdCardButtonsText[2],BUTTON_SYMBOL);
	sdCardButtons[6] = _buttons.addButton(houD[0],houD[1],sdCardButtonsText[3],BUTTON_SYMBOL);
	sdCardButtons[7] = _buttons.addButton(minD[0],minD[1],sdCardButtonsText[4],BUTTON_SYMBOL);
	
	//If first toggle is inactive we grey out buttons
	if (!_sdActive) {
		for (int i = 4; i < nSDcardButtons; i++)
		_buttons.disableButton(sdCardButtons[i],true);
		} else {
		for (int i = 4; i < nSDcardButtons; i++)
		_buttons.enableButton(sdCardButtons[i],true);
	}
} 

//Draws entire screen SD Card
void WinSD::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinSD);
	addFlowButtons(true,true,true,sdCardButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only SD numbers & text from inner temp vars
//Used when +- signs are pressed
void WinSD::update() {
	const int houU[] = {217, _ySecondLine-22};       //hour up
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	if (_sdActive)
	_lcd->print(onStr,_xSpacer+((3+strlen(sdCardButtonsText[0]))*_bigFontSize),_yFirstLine);
	else
	_lcd->print(offStr,_xSpacer+((3+strlen(sdCardButtonsText[0]))*_bigFontSize),_yFirstLine);

	_lcd->setColor(grey[0],grey[1],grey[2]);
	//hours
	int x = houU[0]+_bigFontSize/2-_bigFontSize+2;
	_lcd->printNumI(_sdHour,x,_ySecondLine,2,'0');
	x += 2*_bigFontSize;
	//Mins
	x += 2*_bigFontSize;
	_lcd->printNumI(_sdMin,x,_ySecondLine,2,'0');
	
	//If first toggle is inactive we grey out buttons
	if (!_sdActive) {
		for (int i = 4; i < nSDcardButtons; i++)
		_buttons.disableButton(sdCardButtons[i],true);
		} else {
		for (int i = 4; i < nSDcardButtons; i++)
		_buttons.enableButton(sdCardButtons[i],true);
	}
}

Window::Screen WinSD::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sdCardButtons[0]) { return ControllerSettings; }
	//Save
	else if (buttonIndex == sdCardButtons[1]) {
		_settings->setSDactive(_sdActive);
		_settings->setSDhour(_sdHour);
		_settings->setSDminute(_sdMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == sdCardButtons[2]) { return MainScreen; }
				
	//Card active toggle
	else if (buttonIndex == sdCardButtons[3]) {
		_sdActive = !_sdActive;
		update();
	//Hour up
	} else if (buttonIndex == sdCardButtons[4]) {
		(_sdHour >= 23) ? _sdHour=0 : _sdHour++;
		update();
	//Minute up
	} else if (buttonIndex == sdCardButtons[5]) {
		(_sdMin >= 59) ? _sdMin=0 : _sdMin++;
		update();
	//Hour down
	} else if (buttonIndex == sdCardButtons[6]) {
		(_sdHour <= 0) ? _sdHour=23 : _sdHour--;
		update();
	//Minute down
	} else if (buttonIndex == sdCardButtons[7]) {
		(_sdMin <= 0) ? _sdMin=59 : _sdMin--;
		update();
	}
	return None;
}
