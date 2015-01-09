#include "WinSensorPolling.h"

WinSensorPolling::WinSensorPolling(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSensorPolling::WinSensorPolling(const WinSensorPolling &other) : Window(other) {
	for (int i = 0; i < _nSensorPollingButtons; i++) {
		_sensorPollingButtons[i] = other._sensorPollingButtons[i];
	}
}
	
WinSensorPolling& WinSensorPolling::operator=(const WinSensorPolling& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nSensorPollingButtons; i++) {
		_sensorPollingButtons[i] = other._sensorPollingButtons[i];
	}
	return *this;
}

WinSensorPolling::~WinSensorPolling() {}
	
Window::Screen WinSensorPolling::getType() const {
	return Window::SensorPolling;
}

void WinSensorPolling::print() {
	const int xSpacer2 = 72+3*_bigFontSize;	
	const int secU[] = {xSpacer2+_bigFontSize/2, _yTwoLnsSecond-22};       //sec up
	const int secD[] = {xSpacer2+_bigFontSize/2, _yTwoLnsSecond+22};       //sec down
	
	_pollSec = _settings->getSensorSecond();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(hallfetica_normal);
	
	//Time between readings text
	_lcd->print(sensorPollingText1, _xConfig, _yTwoLnsFirst);
	//XX
	_lcd->printNumI(_pollSec,xSpacer2,_yTwoLnsSecond,2,'0');
	//secs
	_lcd->print(sensorPollingText2,xSpacer2+3*_bigFontSize,_yTwoLnsSecond);
	
	//Make +/- buttons
	_sensorPollingButtons[_nFlowButtons] = _buttons.addButton(secU[0],secU[1],pmChar(plusStr),BUTTON_SYMBOL);
	_sensorPollingButtons[_nFlowButtons+1] = _buttons.addButton(secD[0],secD[1],pmChar(minusStr),BUTTON_SYMBOL);
} 

//Draws entire screen Sensor Polling
void WinSensorPolling::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinSensorPolling);
	addFlowButtons(true,true,true,_sensorPollingButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only sensor polling numbers from inner temp vars
//Used when +- signs are pressed
void WinSensorPolling::update() {
	const int  xSpacer2 = 72+3*_bigFontSize;
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_pollSec,xSpacer2,_yTwoLnsSecond,2,'0');
}

Window::Screen WinSensorPolling::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _sensorPollingButtons[0]) { return ControllerSettings; }
	//Save
	else if (buttonIndex == _sensorPollingButtons[1]) {
		_settings->setSensorSecond(_pollSec);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _sensorPollingButtons[2]) { return MainScreen; }
		
	//Sec up
	else if (buttonIndex == _sensorPollingButtons[3]) {
		(_pollSec >= 59) ? _pollSec=1 : _pollSec++;
		update();
	//Sec down
	} else if (buttonIndex == _sensorPollingButtons[4]) {
		(_pollSec <= 1) ? _pollSec=59 : _pollSec--;
		update();
	}
	return None;
}