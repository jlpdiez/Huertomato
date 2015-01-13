#include "WinPhCalib.h"

WinPhCalib::WinPhCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) {
	_actScreen = 0;
}

WinPhCalib::WinPhCalib(const WinPhCalib &other) : Window(other) {
	_actScreen = 0;
	for (uint8_t i = 0; i < _nPHcalibrationButtons; i++) {
		_phCalibrationButtons[i] = other._phCalibrationButtons[i];
	}
}

WinPhCalib& WinPhCalib::operator=(const WinPhCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	_actScreen = 0;
	for (uint8_t i = 0; i < _nPHcalibrationButtons; i++) {
		_phCalibrationButtons[i] = other._phCalibrationButtons[i];
	}
	return *this;
}

WinPhCalib::~WinPhCalib() {}

Window::Screen WinPhCalib::getType() const {
	return Window::PhCalib;
}

void WinPhCalib::print() {
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Start screen
	if (_actScreen == 0) {
		_lcd->print(pmChar(startCalibStr1),centerX(startCalibStr1),_yThreeLnsFirst);
		_lcd->print(pmChar(startCalibStr2),centerX(startCalibStr2),_yThreeLnsSecond);
		_phCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(yesStr),_yThreeLnsThird,yesStr);
	
	//pH 7,4 and 10 screens
	} else {
		_lcd->print(pmChar(phText1),centerX(phText1),_yThreeLnsFirst);
		_lcd->print(pmChar(phText2),centerX(phText2),_yThreeLnsSecond);
		uint16_t x = _xConfig + strlen_P(phText2)*_bigFontSize;
		if (_actScreen == 1)
			_lcd->printNumI(7,x,_yThreeLnsSecond);
		if (_actScreen == 2)
			_lcd->printNumI(4,x,_yThreeLnsSecond);
		if (_actScreen == 3) {
			_lcd->printNumI(10,x,_yThreeLnsSecond);
			x += _bigFontSize;
		}
		x += 2 * _bigFontSize;
		_lcd->print(pmChar(phText3),x,_yThreeLnsSecond);
		_lcd->print(pmChar(phText4),_xConfig,_yThreeLnsThird);
		if ((_actScreen == 1) || (_actScreen == 2))
			_phCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(continueStr),_flowButtonY,continueStr);
		else if (_actScreen == 3)
			_phCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(endStr),_flowButtonY,endStr);
	}
}


//Draws entire screen Sensor Calibration
void WinPhCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinPhCalib);
	if (_actScreen == 0)
		addFlowButtons(true,false,true,_phCalibrationButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinPhCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back & exit buttons only activated at start screen
	if (_actScreen == 0) {
		//Back
		if (buttonIndex == _phCalibrationButtons[0])
			return SensorCalib;
		//Exit
		else if (buttonIndex == _phCalibrationButtons[2])
			return MainScreen;
		//Start calibration button
		else if (buttonIndex == _phCalibrationButtons[3]) {
			//Command to pH circuit
			_actScreen = 1;
			draw();
			return None;
		}
	} else if ((_actScreen == 1) && (buttonIndex == _phCalibrationButtons[3])) {
		//Command to pH circuit
		_actScreen = 2;
		draw();
		return None;
	} else if ((_actScreen == 2) && (buttonIndex == _phCalibrationButtons[3])) {
		//Command to pH circuit
		_actScreen = 3;
		draw();
		return None;
	} else if ((_actScreen == 3) && (buttonIndex == _phCalibrationButtons[3])) {
		//Command to pH circuit
		return SensorCalib;
	}
	return None;
}
