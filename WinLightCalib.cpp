/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinLightCalib.h"

WinLightCalib::WinLightCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinLightCalib::WinLightCalib(const WinLightCalib &other) : Window(other) { }
	
WinLightCalib& WinLightCalib::operator=(const WinLightCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

void WinLightCalib::print() {
	const int yFirstLine = 60;
	const int ySecondLine = 135;
	const int xSpacer = 25;
	
	_rawLightLvl = _sensors->getRawLight();
	_lightThreshold = _settings->getLightThreshold();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer;
	_lcd->print("Current Reading:",x,yFirstLine);
	x += 16*bigFontSize;
	_lcd->printNumI(_rawLightLvl,x,yFirstLine,4,' ');
	//x +=4*bigFontSize;
	//_lcd->print("lux",x,yFirstLine);
	
	//Second Line
	x = xSpacer;
	_lcd->print("Threshold:",x,ySecondLine);
	x += 10*bigFontSize;
	_lcd->printNumI(_lightThreshold,x,ySecondLine,4,' ');
	//x += 4*bigFontSize;
	//_lcd->print("lux",x,ySecondLine);
	x += 5*bigFontSize;
	lightCalibrationButtons[3] = _buttons.addButton(x,ySecondLine,lightCalibrationButtonsText[0]);
}

void WinLightCalib::update() {
	const int yFirstLine = 60;
	const int ySecondLine = 135;
	const int xSpacer = 25;
	
	_rawLightLvl = _sensors->getRawLight();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer + 16*bigFontSize;
	_lcd->printNumI(_rawLightLvl,x,yFirstLine,4,' ');
	
	//Second Line
	x = xSpacer + 10*bigFontSize;
	_lcd->printNumI(_lightThreshold,x,ySecondLine,4,' ');
}

//Draws entire screen Light Calibration
void WinLightCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Night Calibration -");
	addFlowButtons(true,true,true,lightCalibrationButtons);
	print();
	_buttons.drawButtons();
}

int WinLightCalib::processTouch(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == lightCalibrationButtons[0]) {
		//Go to calibration menu
		return SensorCalib;
	//Save
	} else if (buttonIndex == lightCalibrationButtons[1]) {
		_settings->setLightThreshold(_lightThreshold);
		printSavedButton();
	//Exit
	} else if (buttonIndex == lightCalibrationButtons[2]) {
		//Go to main screen
		return MainScreen;
	} else if (buttonIndex == lightCalibrationButtons[3]) {
		_lightThreshold = _rawLightLvl;
		update();
	}
	return 0;
}
