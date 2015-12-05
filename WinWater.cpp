#include "WinWater.h"

WinWater::WinWater(LiquidCrystal *lcd, Sensors *sensors, Settings *settings) 
: Window(lcd,sensors,settings) { }

WinWater::WinWater(const WinWater &other) : Window(other) {}
	
WinWater& WinWater::operator=(const WinWater& other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	return *this;
}

WinWater::~WinWater() {}
	
Window::Screen WinWater::getType() const {
	return Window::WateringCycle;
}

//Draws entire screen Watering Cycle
void WinWater::draw() {
	_lcd->clear();
	_lcd->setCursor(0,0);
	_lcd->print("Riego cada:");
	_lcd->print("01:30");
	_lcd->setCursor(0,1);
	_lcd->print("Durante:");
	_lcd->print("15m");
} 

//Redraws only water cycle numbers & text from inner temp vars
//Used when +- signs are pressed
void WinWater::update() {
	/*_lcd->setFont(hallfetica_normal);
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	
	//Continuous/timed text
	int x = _xConfig + 2*_bigFontSize;;
	x += (1+strlen_P(modeStr))*_bigFontSize;
	if (_waterTimed)
		_lcd->print(pmChar(modeTimedS),x,_yThreeLnsFirst);
	else
		_lcd->print(pmChar(modeContS),x,_yThreeLnsFirst);
	
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
		for (uint8_t i = 4; i < _nWaterCycleButtons; i++)
			_buttons.disableButton(_waterCycleButtons[i],true);
	} else {
		for (uint8_t i = 4; i < _nWaterCycleButtons; i++)
			_buttons.enableButton(_waterCycleButtons[i],true);
	}*/
}

//TODO: Rutina para incrementar/decrementar valores
Window::Screen WinWater::processTouch(int but) {
	if (but == 4)
		return TimeDate;
	else if (but == 1)
		return MainScreen;
	else
		return None;	
	/*int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _waterCycleButtons[0]) 
		return SystemSettings;
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
	} else if (buttonIndex == _waterCycleButtons[2]) 
		return MainScreen;	
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
	return None;*/
}