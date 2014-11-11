/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinLvlCalib.h"

WinLvlCalib::WinLvlCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinLvlCalib::WinLvlCalib(const WinLvlCalib &other) : Window(other) { }
	
WinLvlCalib& WinLvlCalib::operator=(const WinLvlCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinLvlCalib::~WinLvlCalib() {}
	
Window::Screen WinLvlCalib::getType() const {
	return Window::LvlCalib;
}

void WinLvlCalib::print() {
	const int yFirstLine = 50;
	const int ySecondLine = 100;
	const int yThirdLine = 150;
	const int xSpacer = 25;
	
	_rawWaterLvl = _sensors->getRawWaterLevel();
	_waterLvlMax = _settings->getMaxWaterLvl();
	_waterLvlMin = _settings->getMinWaterLvl();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer;
	_lcd->print("Current Reading:",x,yFirstLine);
	x += 16*_bigFontSize;
	_lcd->printNumI(_rawWaterLvl,x,yFirstLine,3,' ');
	x +=3*_bigFontSize;
	_lcd->print("cm",x,yFirstLine);
	
	//Second Line
	x = xSpacer;
	_lcd->print("Current Top:",x,ySecondLine);
	x += 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMax,x,ySecondLine,3,' ');
	x += 3*_bigFontSize;
	_lcd->print("cm",x,ySecondLine);
	x += 3*_bigFontSize;
	waterLevelButtons[3] = _buttons.addButton(x,ySecondLine,waterLevelButtonsText[0]);
	
	//Third Line
	x = xSpacer;
	_lcd->print(" and Bottom:",x,yThirdLine);
	x += 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMin,x,yThirdLine,3,' ');
	x += 3*_bigFontSize;
	_lcd->print("cm",x,yThirdLine);
	x += 3*_bigFontSize;
	waterLevelButtons[4] = _buttons.addButton(x,yThirdLine,waterLevelButtonsText[1]);
}

void WinLvlCalib::update() {
	const int yFirstLine = 50;
	const int ySecondLine = 100;
	const int yThirdLine = 150;
	const int xSpacer = 25;
	
	_rawWaterLvl = _sensors->getRawWaterLevel();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer + 16*_bigFontSize;
	_lcd->printNumI(_rawWaterLvl,x,yFirstLine,3,' ');
	
	//Second Line
	x = xSpacer + 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMax,x,ySecondLine,3,' ');
	
	//Third Line
	x = xSpacer + 12*_bigFontSize;
	_lcd->printNumI(_waterLvlMin,x,yThirdLine,3,' ');
}

//Draws entire screen Water Level Calibration
void WinLvlCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Nutrient Levels -");
	addFlowButtons(true,true,true,waterLevelButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinLvlCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == waterLevelButtons[0]) { return SensorCalib; }
	//Save
	else if (buttonIndex == waterLevelButtons[1]) {
		_settings->setMaxWaterLvl(_waterLvlMax);
		_settings->setMinWaterLvl(_waterLvlMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == waterLevelButtons[2]) { return MainScreen; }
	
	//Top, bottom calib buttons
	else if (buttonIndex == waterLevelButtons[3]) {
		_waterLvlMax = _rawWaterLvl;
		update();
	} else if (buttonIndex == waterLevelButtons[4]) {
		_waterLvlMin = _rawWaterLvl;
		update();
	}
	return None;
}
