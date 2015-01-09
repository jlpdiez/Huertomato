#include "WinWater.h"

WinWater::WinWater(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinWater::WinWater(const WinWater &other) : Window(other) {
	for (int i = 0; i < _nWaterCycleButtons; i++) {
		_waterCycleButtons[i] = other._waterCycleButtons[i];
	}
}
	
WinWater& WinWater::operator=(const WinWater& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nWaterCycleButtons; i++) {
		_waterCycleButtons[i] = other._waterCycleButtons[i];
	}
	return *this;
}

WinWater::~WinWater() {}
	
Window::Screen WinWater::getType() const {
	return Window::WateringCycle;
}

void WinWater::print() {
	const int houU[] = {240, _yThreeLnsSecond-22};       //hour up
	const int houD[] = {240, _yThreeLnsSecond+22};       //hour down
	const int minU[] = {305, _yThreeLnsSecond-22};       //min up
	const int minD[] = {305, _yThreeLnsSecond+22};       //min down
	const int fMinU[] = {225, _yThreeLnsThird-22};       //active for min
	const int fMinD[] = {225, _yThreeLnsThird+22};       //active for min
	
	_waterTimed = _settings->getWaterTimed();
	_waterHour = _settings->getWaterHour();
	_waterMin = _settings->getWaterMinute();
	_floodMin = _settings->getFloodMinute();
	
	//First Line - Triangle
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setFont(various_symbols);
	_lcd->print(bulletStr,_xConfig,_yThreeLnsFirst);
	//Water mode button
	_lcd->setFont(hallfetica_normal);
	int x = _xConfig + 2*_bigFontSize;
	_waterCycleButtons[_nFlowButtons] = _buttons.addButton(x,_yThreeLnsFirst,pmChar(modeStr));
	//Continuous/timed text
	x += (1+strlen_P(modeStr))*_bigFontSize;
	if (_waterTimed)
		_lcd->print(modeTimedS,x,_yThreeLnsFirst);
	else
		_lcd->print(modeContS,x,_yThreeLnsFirst);
	
	//Second Line
	_lcd->setColor(grey[0],grey[1],grey[2]);
	x = _xConfig;
	_lcd->print(waterTwo,x,_yThreeLnsSecond);
	x += 13*_bigFontSize;
	_lcd->printNumI(_waterHour,x,_yThreeLnsSecond,2,'0');
	x += 2*_bigFontSize;
	_lcd->print("h",x,_yThreeLnsSecond);
	x += 2*_bigFontSize;
	_lcd->printNumI(_waterMin,x,_yThreeLnsSecond,2,'0');
	x += 2*_bigFontSize;
	_lcd->print("m",x,_yThreeLnsSecond);
	_waterCycleButtons[_nFlowButtons+1] = _buttons.addButton(houU[0],houU[1],pmChar(plusStr),BUTTON_SYMBOL);
	_waterCycleButtons[_nFlowButtons+2] = _buttons.addButton(houD[0],houD[1],pmChar(minusStr),BUTTON_SYMBOL);
	_waterCycleButtons[_nFlowButtons+3] = _buttons.addButton(minU[0],minU[1],pmChar(plusStr),BUTTON_SYMBOL);
	_waterCycleButtons[_nFlowButtons+4] = _buttons.addButton(minD[0],minD[1],pmChar(minusStr),BUTTON_SYMBOL);
	
	//Third line
	x = _xConfig;
	_lcd->print(waterThree,x,_yThreeLnsThird);
	x += 12*_bigFontSize;
	_lcd->printNumI(_floodMin,x,_yThreeLnsThird,2,'0');
	x += 3*_bigFontSize;
	_lcd->print("minutes",x,_yThreeLnsThird);
	_waterCycleButtons[_nFlowButtons+5] = _buttons.addButton(fMinU[0],fMinU[1],pmChar(plusStr),BUTTON_SYMBOL);
	_waterCycleButtons[_nFlowButtons+6] = _buttons.addButton(fMinD[0],fMinD[1],pmChar(minusStr),BUTTON_SYMBOL);
	
	//If first toggle is inactive we grey out buttons
	if (!_waterTimed) {
		for (int i = 4; i < _nWaterCycleButtons; i++)
		_buttons.disableButton(_waterCycleButtons[i],true);
		} else {
		for (int i = 4; i < _nWaterCycleButtons; i++)
		_buttons.enableButton(_waterCycleButtons[i],true);
	}
} 

//Draws entire screen Watering Cycle
void WinWater::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinWater);
	addFlowButtons(true,true,true,_waterCycleButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only water cycle numbers & text from inner temp vars
//Used when +- signs are pressed
void WinWater::update() {
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	
	//Continuous/timed text
	int x = _xConfig + 2*_bigFontSize;;
	x += (1+strlen_P(modeStr))*_bigFontSize;
	if (_waterTimed)
		_lcd->print(modeTimedS,x,_yThreeLnsFirst);
	else
		_lcd->print(modeContS,x,_yThreeLnsFirst);
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Water every
	x = _xConfig;
	x += 13*_bigFontSize;
	_lcd->printNumI(_waterHour,x,_yThreeLnsSecond,2,'0');
	x += 2*_bigFontSize;
	x += 2*_bigFontSize;
	_lcd->printNumI(_waterMin,x,_yThreeLnsSecond,2,'0');
	//Flood time
	x = _xConfig;
	x += 12*_bigFontSize;
	_lcd->printNumI(_floodMin,x,_yThreeLnsThird,2,'0');
	
	//If first toggle is inactive we grey out buttons
	if (!_waterTimed) {
		for (int i = 4; i < _nWaterCycleButtons; i++)
		_buttons.disableButton(_waterCycleButtons[i],true);
	} else {
		for (int i = 4; i < _nWaterCycleButtons; i++)
		_buttons.enableButton(_waterCycleButtons[i],true);
	}
}

Window::Screen WinWater::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _waterCycleButtons[0]) { return SystemSettings; }
	//Save
	else if (buttonIndex == _waterCycleButtons[1]) {
		//Prevents flood time > time inactive as it will mess up alarms
		//As flood time always > 1 this also prevents a watering time of 0
		if ((_waterHour == 0) && (_floodMin >= _waterMin)) {
			_waterMin = _floodMin + 1;
			update();
		}
		_settings->setWaterTimed(_waterTimed);
		_settings->setWaterHour(_waterHour);
		_settings->setWaterMinute(_waterMin);
		_settings->setFloodMinute(_floodMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _waterCycleButtons[2]) { return MainScreen; }	
	//Water mode
	else if (buttonIndex == _waterCycleButtons[3]) {
		_waterTimed = !_waterTimed;
		update();
	//Hour up
	} else if (buttonIndex == _waterCycleButtons[4]) {
		(_waterHour >= 23) ? _waterHour=0 : _waterHour++;
		update();
	//Hour down
	} else if (buttonIndex == _waterCycleButtons[5]) {
		(_waterHour <= 0) ? _waterHour=23 : _waterHour--;
		update();
	//Minute up
	} else if (buttonIndex == _waterCycleButtons[6]) {
		(_waterMin >= 59) ? _waterMin=0 : _waterMin++;;
		update();
	//Minute down
	} else if (buttonIndex == _waterCycleButtons[7]) {
		(_waterMin <= 0) ? _waterMin=59 : _waterMin--;
		update();
	//Flood minute up
	} else if (buttonIndex == _waterCycleButtons[8]) {
		(_floodMin >= 59) ? _floodMin=1 : _floodMin++;
		update();
	//Flood minute down
	} else if (buttonIndex == _waterCycleButtons[9]) {
		(_floodMin <= 1) ? _floodMin=59 : _floodMin--;
		update();
	}
	return None;
}