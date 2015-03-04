#include "WinEcCalib.h"

WinEcCalib::WinEcCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) {
	_actScreen = 0;
}

WinEcCalib::WinEcCalib(const WinEcCalib &other) : Window(other) {
	_actScreen = 0;
	for (uint8_t i = 0; i < _nECcalibrationButtons; i++) {
		_ecCalibrationButtons[i] = other._ecCalibrationButtons[i];
	}
}

WinEcCalib& WinEcCalib::operator=(const WinEcCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	_actScreen = 0;
	for (uint8_t i = 0; i < _nECcalibrationButtons; i++) {
		_ecCalibrationButtons[i] = other._ecCalibrationButtons[i];
	}
	return *this;
}

WinEcCalib::~WinEcCalib() {}

Window::Screen WinEcCalib::getType() const {
	return Window::EcCalib;
}

void WinEcCalib::print() {
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Start screen
	if (_actScreen == 0) {
		_lcd->print(pmChar(startCalibStr1),centerX(startCalibStr1),_yThreeLnsFirst);
		_lcd->print(pmChar(startCalibStr2),centerX(startCalibStr2),_yThreeLnsSecond);
		_ecCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(yesStr),_yThreeLnsThird,yesStr);	
	//Dry calibration screen
	} else if (_actScreen == 1) {
		_lcd->print(pmChar(ecText1),centerX(ecText1),_yThreeLnsFirst);
		_lcd->print(pmChar(ecText2),centerX(ecText2),_yThreeLnsSecond);
		_lcd->print(pmChar(ecText3),centerX(ecText3),_yThreeLnsThird);
		_ecCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(continueStr),200,continueStr);
	//40,000uS screen
	} else if (_actScreen == 2) {
		_lcd->print(pmChar(ecText4),centerX(ecText4),_yThreeLnsFirst);
		_lcd->print(pmChar(ecText5),centerX(ecText5),_yThreeLnsSecond);
		_lcd->print(pmChar(ecText6),centerX(ecText6),_yThreeLnsThird);
		_ecCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(continueStr),200,continueStr);
	//10,500uS screen
	} else if (_actScreen = 3) {
		_lcd->print(pmChar(ecText7),centerX(ecText7),_yThreeLnsFirst);
		_lcd->print(pmChar(ecText8),centerX(ecText8),_yThreeLnsSecond);
		_lcd->print(pmChar(ecText6),centerX(ecText6),_yThreeLnsThird);
		_ecCalibrationButtons[_nFlowButtons] = _buttons.addButton(centerX(endStr),200,endStr);
	}
}

//Draws entire screen Sensor Calibration
void WinEcCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinEcCalib);
	if (_actScreen == 0)
		addFlowButtons(true,false,true,_ecCalibrationButtons);
	print();
	_buttons.drawButtons();
	if (_actScreen == 0)
		_sensors->getECinfo();
}

Window::Screen WinEcCalib::processTouch(const int x, const int y) {
	int8_t buttonIndex = _buttons.checkButtons(x,y);
	//Back & exit buttons only activated at start screen
	if (_actScreen == 0) {
		//Back
		if (buttonIndex == _ecCalibrationButtons[0])
			return SensorCalib;
		//Exit
		else if (buttonIndex == _ecCalibrationButtons[2])
			return MainScreen;
		//Start calibration button
		else if (buttonIndex == _ecCalibrationButtons[3]) {
			_sensors->calibratingEC(true);
			_sensors->resetEC();
			_sensors->setECstandby();
			_sensors->setECprobeType();
			_actScreen = 1;
			draw();
			return None;
		}
	} else if ((_actScreen == 1) && (buttonIndex == _ecCalibrationButtons[3])) {
		_sensors->setECdry();
		_sensors->setECcontinuous();
		_actScreen = 2;
		draw();
		return None;
	} else if ((_actScreen == 2) && (buttonIndex == _ecCalibrationButtons[3])) {
		_sensors->setECfortyThousand();
		_actScreen = 3;
		draw();
		return None;
	} else if ((_actScreen == 3) && (buttonIndex == _ecCalibrationButtons[3])) {
		_sensors->setECtenThousand();
		//_sensors->setECcontinuous();
		_sensors->calibratingEC(false);
		return SensorCalib;
	}
	return None;
}