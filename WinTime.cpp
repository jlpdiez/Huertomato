#include "WinTime.h"

WinTime::WinTime(LiquidCrystal *lcd, Sensors *sensors, Settings *settings) 
: Window(lcd,sensors,settings) {
	_line = 0;
	_column = 0;
	_modified = false;
}

WinTime::WinTime(const WinTime &other) : Window(other) {
	_line = other._line;
	_column = other._column;
	_modified = false;
}
	
WinTime& WinTime::operator=(const WinTime& other) {
	_line = other._line;
	_column = other._column;
	_modified = false;
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	return *this;
}

WinTime::~WinTime() {}
	
Window::Screen WinTime::getType() const {
	return Window::TimeDate;
}

//Draws entire screen Time Settings
void WinTime::draw() {
	//Get actual time
	time_t t = now();
	_sysHour = hour(t);
	_sysMin = minute(t);
	_sysSec = second(t);
	_sysDay = day(t);
	_sysMonth = month(t);
	_sysYear = year(t);
	
	_lcd->clear();
	_lcd->setCursor(4,0);
	(_sysHour < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysHour);
	_lcd->print(pmChar(timeSeparator));
	(_sysMin < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysMin);
	_lcd->print(pmChar(timeSeparator));
	(_sysSec < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysSec);
	
	_lcd->setCursor(3,1);
	(_sysDay < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysDay);
	_lcd->print(pmChar(dateSeparator));
	(_sysMonth < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysMonth);
	_lcd->print(pmChar(dateSeparator));
	_lcd->print(_sysYear);
	
	_line = 0;
	_column = 5;
	_lcd->setCursor(_column,_line);
	_lcd->cursor();
}

//Redraws only time & date numbers from inner temp vars
//Used when +- signs are pressed
void WinTime::update() {
	//_lcd->clear();
	_lcd->setCursor(4,0);
	(_sysHour < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysHour);
	_lcd->print(pmChar(timeSeparator));
	(_sysMin < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysMin);
	_lcd->print(pmChar(timeSeparator));
	(_sysSec < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysSec);
	
	_lcd->setCursor(3,1);
	(_sysDay < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysDay);
	_lcd->print(pmChar(dateSeparator));
	(_sysMonth < 10) ? _lcd->print("0") : 0;
	_lcd->print(_sysMonth);
	_lcd->print(pmChar(dateSeparator));
	_lcd->print(_sysYear);
	
	_lcd->setCursor(_column,_line);
	_lcd->cursor();
}

Window::Screen WinTime::processTouch(int but) {
	//Select - Saves time and changes screen
	if (but == 5) {
		_lcd->noBlink();
		_lcd->noCursor();
		if (_modified)
			_settings->setRTCtime(_sysHour, _sysMin, _sysSec, _sysDay, _sysMonth, _sysYear);
		return MainScreen;
		
	//Right Button - Cycle between numbers
	} else if (but == 1) {
		//First line - Time
		if (_line == 0) {
			//From hour to min
			if (_column == 5) {
				_column = 8;
			//Min to sec
			} else if (_column == 8) {
				_column = 11;
			//Sec to day
			} else if (_column == 11) {
				_line = 1;
				_column = 4;
			}		
		//Second line - Date
		} else if (_line == 1) {
			//From day to month
			if (_column == 4) {
				_column = 7;
			//From month to year
			} else if(_column == 7) {
				_column = 12;
			//From year back to hour
			} else if (_column == 12) {
				_line = 0;
				_column = 5;
			}
		}
		_lcd->setCursor(_column,_line);
	
	//Left Button - Cycle between numbers
	} else if (but == 4) {
		//First line - Time
		if (_line == 0) {
			//From hour to year
			if (_column == 5) {
				_line = 1;
				_column = 12;
			//Min to hour
			} else if (_column == 8) {
				_column = 5;
			//Sec to min
			} else if (_column == 11) {
				_column = 8;
			}
		//Second line - Date
		} else if (_line == 1) {
			//From day to sec
			if (_column == 4) {
				_line = 0;
				_column = 11;
			//From month to day
			} else if(_column == 7) {
				_column = 4;
			//From year back to month
			} else if (_column == 12) {
				_column = 7;
			}
		}
		_lcd->setCursor(_column,_line);
			
	//UP Button
	} else if (but == 2) {
		//First line - Time
		if (_line == 0) {
			//Hour++
			if (_column == 5) {
				(_sysHour >= 23) ? _sysHour=0 : _sysHour++;
			//Min++
			} else if (_column == 8) {
				(_sysMin >= 59) ? _sysMin=0 : _sysMin++;
			//Sec++
			} else if (_column == 11) {
				(_sysSec >= 59) ? _sysSec=0 : _sysSec++;
			}
		//Second line - Date
		} else if (_line == 1) {
			//Day++
			if (_column == 4) {
				(_sysDay >= 31) ? _sysDay=1 : _sysDay++;
			//Month++
			} else if(_column == 7) {
				(_sysMonth >= 12) ? _sysMonth=1 : _sysMonth++;
			//Year++
			} else if (_column == 12) {
				(_sysYear >= 2037) ? _sysYear=1971 : _sysYear++;
			}
		}
		_modified = true;
		update();
		
	//DOWN	
	} else if (but == 3) {
		//First line - Time
		if (_line == 0) {
			//Hour--
			if (_column == 5) {
				(_sysHour <= 0) ? _sysHour=23 : _sysHour--;
			//Min--
			} else if (_column == 8) {
				(_sysMin <= 0) ? _sysMin=59 : _sysMin--;
			//Sec--
			} else if (_column == 11) {
				(_sysSec <= 0) ? _sysSec=59 : _sysSec--;
			}
		//Second line - Date
		} else if (_line == 1) {
			//Day--
			if (_column == 4) {
				(_sysDay <= 1) ? _sysDay=31 : _sysDay--;
			//Month--
			} else if(_column == 7) {
				(_sysMonth <= 1) ? _sysMonth=12 : _sysMonth--;
			//Year--
			} else if (_column == 12) {
				(_sysYear <= 1971) ? _sysYear=2037 : _sysYear--;
			}
		}
		_modified = true;
		update();
	}
	return None;
}
