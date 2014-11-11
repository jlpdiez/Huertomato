/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinSensorCalib.h"

WinSensorCalib::WinSensorCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSensorCalib::WinSensorCalib(const WinSensorCalib &other) : Window(other) { }
	
WinSensorCalib& WinSensorCalib::operator=(const WinSensorCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinSensorCalib::~WinSensorCalib() {}
	
Window::Screen WinSensorCalib::getType() const {
	return Window::SensorCalib;
}

void WinSensorCalib::print() {
	const int xSpacer = 15;
	const int ySpacer = 45;
	
	const int yFirst = 60;
	const int ySecond = 135;
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	
	//Triangles
	_lcd->setFont(various_symbols);
	_lcd->print("T",xSpacer,yFirst);
	_lcd->print("T",xSpacer,ySecond);
	//Buttons
	sensorCalibrationButtons[3] = _buttons.addButton(xSpacer+_bigFontSize*2,yFirst,sensorCalibrationButtonsText[0]);
	sensorCalibrationButtons[4] = _buttons.addButton(xSpacer+_bigFontSize*2,ySecond,sensorCalibrationButtonsText[1]);
}

//Draws entire screen Sensor Calibration
void WinSensorCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Calibration -");
	addFlowButtons(true,false,true,sensorCalibrationButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinSensorCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorCalibrationButtons[0]) { return SystemSettings; }
	//Exit
	else if (buttonIndex == sensorCalibrationButtons[2]) { return MainScreen; }
	//Water calibration
	else if (buttonIndex == sensorCalibrationButtons[3]) { return LvlCalib; }
	//Light Calibration
	else if (buttonIndex == sensorCalibrationButtons[4]) { return LightCalib; }
	return None;
}