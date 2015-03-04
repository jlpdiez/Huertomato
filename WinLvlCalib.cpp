#include "WinLvlCalib.h"

WinLvlCalib::WinLvlCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinLvlCalib::WinLvlCalib(const WinLvlCalib &other) : Window(other) { 
	for (uint8_t i = 0; i < _nWaterLevelButtons; i++) {
		_waterLevelButtons[i] = other._waterLevelButtons[i];
	}
}
	
WinLvlCalib& WinLvlCalib::operator=(const WinLvlCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nWaterLevelButtons; i++) {
		_waterLevelButtons[i] = other._waterLevelButtons[i];
	}
	return *this;
}

WinLvlCalib::~WinLvlCalib() {}
	
Window::Screen WinLvlCalib::getType() const {
	return Window::LvlCalib;
}

void WinLvlCalib::print() {
	_rawWaterLvl = _sensors->getRawWaterLevel();
	_waterLvlMax = _settings->getMaxWaterLvl();
	_waterLvlMin = _settings->getMinWaterLvl();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = _xConfig;
	_lcd->print(pmChar(firstLvlCalib),x,_yThreeLnsFirst);
	x += 16*_bigFontSize;
	_lcd->printNumI(_rawWaterLvl,x,_yThreeLnsFirst,3,' ');
	x +=3*_bigFontSize;
	_lcd->print(pmChar(unitLvl),x,_yThreeLnsFirst);
	
	//Second Line
	x = _xConfig;
	_lcd->print(pmChar(secondLvlCalib),x,_yThreeLnsSecond);
	x += 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMax,x,_yThreeLnsSecond,3,' ');
	x += 3*_bigFontSize;
	_lcd->print(pmChar(unitLvl),x,_yThreeLnsSecond);
	x += 3*_bigFontSize;
	_waterLevelButtons[_nFlowButtons] = _buttons.addButton(x,_yThreeLnsSecond,setStr);
	
	//Third Line
	x = _xConfig;
	_lcd->print(pmChar(thirdLvlCalib),x,_yThreeLnsThird);
	x += 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMin,x,_yThreeLnsThird,3,' ');
	x += 3*_bigFontSize;
	_lcd->print(pmChar(unitLvl),x,_yThreeLnsThird);
	x += 3*_bigFontSize;
	_waterLevelButtons[_nFlowButtons+1] = _buttons.addButton(x,_yThreeLnsThird,setStr);
}

void WinLvlCalib::update() {
	_rawWaterLvl = _sensors->getRawWaterLevel();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = _xConfig + 16*_bigFontSize;
	_lcd->printNumI(_rawWaterLvl,x,_yThreeLnsFirst,3,' ');
	
	//Second Line
	x = _xConfig + 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMax,x,_yThreeLnsSecond,3,' ');
	
	//Third Line
	x = _xConfig + 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMin,x,_yThreeLnsThird,3,' ');
}
 
//Draws entire screen Water Level Calibration
void WinLvlCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinLvlCalib);
	addFlowButtons(true,true,true,_waterLevelButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinLvlCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _waterLevelButtons[0]) 
		return SensorCalib;
	//Save
	else if (buttonIndex == _waterLevelButtons[1]) {
		_settings->setMaxWaterLvl(_waterLvlMax);
		_settings->setMinWaterLvl(_waterLvlMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _waterLevelButtons[2]) 
		return MainScreen;
	
	//Top, bottom calib buttons
	else if (buttonIndex == _waterLevelButtons[3]) {
		_waterLvlMax = _rawWaterLvl;
		update();
	} else if (buttonIndex == _waterLevelButtons[4]) {
		_waterLvlMin = _rawWaterLvl;
		update();
	}
	return None;
}
