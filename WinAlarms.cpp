#include "WinAlarms.h"

WinAlarms::WinAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinAlarms::WinAlarms(const WinAlarms &other) : Window(other) { }
	
WinAlarms& WinAlarms::operator=(const WinAlarms& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinAlarms::~WinAlarms() {}

Window::Screen WinAlarms::getType() const {
	return Window::Alarms;
}

void WinAlarms::print() {	
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	
	//Print bulletpoints
	_lcd->setFont(various_symbols);
	for (int i = 0; i < nSensorAlarmsButtons - _nFlowButtons; i++) {
		_lcd->print(bulletStr,_xMenu,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i);
	}
	
	//Make menu buttons
	//Before the buttons were adding there are the flow buttons
	for (int i = 0; i < nSensorAlarmsButtons - _nFlowButtons; i++) {
		sensorAlarmsButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yThreeLnsFirst+_bigFontSize*_yFactor3lines*i,sensorAlarmsButtonsText[i]);
	}
}
 
//Draws entire screen Sensor Alarms
void WinAlarms::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinAlarms);
	addFlowButtons(true,false,true,sensorAlarmsButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinAlarms::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorAlarmsButtons[0]) { return SystemSettings; }
	//Exit
	else if (buttonIndex == sensorAlarmsButtons[2]) { return MainScreen;  }
	//pH Thresholds
	else if (buttonIndex == sensorAlarmsButtons[3]) { return PhAlarms; }
	//EC Thresholds
	else if (buttonIndex == sensorAlarmsButtons[4]) { return EcAlarms; }
	//Nutrient Level
	else if (buttonIndex == sensorAlarmsButtons[5]) { return LvlAlarms; }
	return None;
}
