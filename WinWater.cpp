#include "WinWater.h"

WinWater::WinWater(LiquidCrystal *lcd, Sensors *sensors, Settings *settings) 
: Window(lcd,sensors,settings) {
	_line = 0;
	_column = 0;
	_modified = false;
}

WinWater::WinWater(const WinWater &other) : Window(other) {
	_line = other._line;
	_column = other._column;
	_modified = false;
}
	
WinWater& WinWater::operator=(const WinWater& other) {
	_line = other._line;
	_column = other._column;
	_modified = false;
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
	//Get actual settings
	_waterHour = _settings->getWaterHour();
	_waterMin = _settings->getWaterMinute();
	_floodMin = _settings->getFloodMinute();
	
	_lcd->clear();
	//First line
	_lcd->setCursor(0,0);
	_lcd->print(pmChar(riegoTxt));
	(_waterHour < 10) ? _lcd->print(pmChar(zero)) : 0;
	_lcd->print(_waterHour);
	_lcd->print(pmChar(timeSeparator));
	(_waterMin < 10) ? _lcd->print(pmChar(zero)) : 0;
	_lcd->print(_waterMin);
	//Second line
	_lcd->setCursor(0,1);
	_lcd->print(pmChar(duranteTxt));
	(_floodMin < 10) ? _lcd->print(pmChar(zero)) : 0;
	_lcd->print(_floodMin);
	_lcd->print(pmChar(minutesChar));
	
	_line = 0;
	_column = 12;
	_lcd->setCursor(_column,_line);
	_lcd->cursor();
} 

//Redraws only water cycle numbers & text from inner temp vars
//Used when +- signs are pressed
void WinWater::update() {
	//First line
	_lcd->setCursor(11,0);
	(_waterHour < 10) ? _lcd->print(pmChar(zero)) : 0;
	_lcd->print(_waterHour);
	_lcd->print(pmChar(timeSeparator));
	(_waterMin < 10) ? _lcd->print(pmChar(zero)) : 0;
	_lcd->print(_waterMin);
	//Second line
	_lcd->setCursor(8,1);
	(_floodMin < 10) ? _lcd->print(pmChar(zero)) : 0;
	_lcd->print(_floodMin);
	_lcd->print(pmChar(minutesChar));
	
	_lcd->setCursor(_column,_line);
	_lcd->cursor();
}

Window::Screen WinWater::processTouch(int but) {
	//Select - Saves and changes screen
	if (but == 5) {
		_lcd->noCursor();
		if (_modified) {
			_settings->setWaterHour(_waterHour);
			_settings->setWaterMinute(_waterMin);
			_settings->setFloodMinute(_floodMin);
		}
		return TimeDate;
		
	//Right Button - Cycle between numbers
	} else if (but == 1) {
		//First line - Time
		if (_line == 0) {
			//From hour to min
			if (_column == 12) {
				_column = 15;
			//From water to flood
			} else if (_column == 15) {
				_line = 1;
				_column = 9;
			}
			//Second line
			} else if ((_line == 1) && (_column == 9)) {
				_line = 0;
				_column = 12;
		}
		_lcd->setCursor(_column,_line);
	
	//Left Button - Cycle between numbers
	} else if (but == 4) {
	//First line - Time
	if (_line == 0) {
		//From hour to flood
		if (_column == 12) {
			_line = 1;
			_column = 9;
		//From min to hour
		} else if (_column == 15) {
			_column = 12;
		}
	//Second line - Date
	} else if ((_line == 1) && (_column == 9)) {
		_line = 0;
		_column = 15;
	}
	_lcd->setCursor(_column,_line);
	
	//UP Button
	} else if (but == 2) {
		if (_line == 0) {
			//Hour++
			if (_column == 12) {
				(_waterHour >= 23) ? _waterHour=0 : _waterHour++;
			//Minute++
			} else if (_column == 15) {
				(_waterMin >= 59) ? _waterMin=0 : _waterMin++;	
			}
		} else if ((_line = 1) && (_column == 9)) {
			(_floodMin >= 59) ? _floodMin=0 : _floodMin++;
		}
		_modified = true;
		update();
	
	//DOWN
	} else if (but == 3) {
		if (_line == 0) {
			//Hour++
			if (_column == 12) {
				(_waterHour <= 0) ? _waterHour=23 : _waterHour--;
			//Minute++
			} else if (_column == 15) {
				(_waterMin <= 1) ? _waterMin=59 : _waterMin--;
			}
		} else if ((_line = 1) && (_column == 9)) {
			(_floodMin <= 0) ? _floodMin=59 : _floodMin--;
		}
		_modified = true;
		update();
	}
	return None;
}