#include "WinPhCalib.h"

WinPhCalib::WinPhCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) {
	_actScreen = 0;
}

WinPhCalib::WinPhCalib(const WinPhCalib &other) : Window(other) {
	_actScreen = 0;
}

WinPhCalib& WinPhCalib::operator=(const WinPhCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	_actScreen = 0;
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
		_lcd->print(pmStr(startCalibStr1),_xConfig,_yTwoLnsFirst);
		_lcd->print(pmStr(startCalibStr2),_xConfig,_yTwoLnsSecond);
		phCalibrationButtons[_nFlowButtons+1] = _buttons.addButton(_xConfig+(strlen_P(startCalibStr2)+1)*_bigFontSize,_yTwoLnsSecond,pmStr(yesStr));
	
	//pH 7 screen
	} else if (_actScreen == 1) {
		_lcd->print(pmStr(phText1),_xConfig,_yThreeLnsFirst);
		_lcd->print(pmStr(phText2),_xConfig,_yThreeLnsSecond);
		_lcd->print(pmStr(phText3),_xConfig,_yThreeLnsThird);
		phCalibrationButtons[_nFlowButtons+1] = _buttons.addButton(_xConfig+(strlen_P(phText3)+1)*_bigFontSize,_yThreeLnsThird,pmStr(phText4));
		
	//pH 4 screen	
	} else if (_actScreen == 2) {
		
	//pH 10 screen
	} else if (_actScreen == 3) {
	
	}
	
	//Print bulletpoints
	/*_lcd->setFont(various_symbols);
	//Before the buttons were adding there are the flow buttons
	for (int i = 0; i < nPHcalibrationButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	//Make menu buttons
	for (int i = 0; i < nPHcalibrationButtons - _nFlowButtons; i++) {
		phCalibrationButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,phCalibrationButtonsText[i]);
	}*/
}


//Draws entire screen Sensor Calibration
void WinPhCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinPhCalib);
	if (_actScreen == 0)
		addFlowButtons(true,false,true,phCalibrationButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinPhCalib::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back & exit buttons only activated at start screen
	if (_actScreen == 0) {
		//Back
		if (buttonIndex == phCalibrationButtons[0])
		return SensorCalib;
		//Exit
		else if (buttonIndex == phCalibrationButtons[2])
		return MainScreen;
		//Start calibration button
		else if (buttonIndex == phCalibrationButtons[4]) {
			//Command to pH circuit
			_actScreen = 1;
			draw();
			return None;
		}
	} else if ((_actScreen == 1) && (buttonIndex == phCalibrationButtons[4])) {
		//Command to pH circuit
		_actScreen = 2;
		draw();
		return None;
	}
	return None;
}
