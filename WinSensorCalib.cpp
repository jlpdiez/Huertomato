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
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Print bulletpoints
	_lcd->setFont(various_symbols);
	//Before the buttons were adding there are the flow buttons
	for (int i = 0; i < nSensorCalibrationButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	//Make menu buttons
	for (int i = 0; i < nSensorCalibrationButtons - _nFlowButtons; i++) {
		sensorCalibrationButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,sensorCalibrationButtonsText[i]);
	}
}

//Draws entire screen Sensor Calibration
void WinSensorCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinSensorCalib);
	addFlowButtons(true,false,true,sensorCalibrationButtons);
	print();
	_buttons.drawButtons();
}
 
Window::Screen WinSensorCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorCalibrationButtons[0]) 
		return Reservoir;
	//Exit
	else if (buttonIndex == sensorCalibrationButtons[2])
		return MainScreen;
	else if (buttonIndex == sensorCalibrationButtons[3])
		return LvlCalib;
	else if (buttonIndex == sensorCalibrationButtons[4])
		return NightWater;
	else if (buttonIndex == sensorCalibrationButtons[5])
		return WateringCycle;
	return None;
}