#include "WinEcAlarms.h"

WinEcAlarms::WinEcAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinEcAlarms::WinEcAlarms(const WinEcAlarms &other) : Window(other) {
	for (uint8_t i = 0; i < _nECalarmsButtons; i++) {
		_ecAlarmsButtons[i] = other._ecAlarmsButtons[i];
	}
}
	
WinEcAlarms& WinEcAlarms::operator=(const WinEcAlarms& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nECalarmsButtons; i++) {
		_ecAlarmsButtons[i] = other._ecAlarmsButtons[i];
	}
	return *this;
}

WinEcAlarms::~WinEcAlarms() {}
	
Window::Screen WinEcAlarms::getType() const {
	return Window::EcAlarms;
}

void WinEcAlarms::print() {
	_ecAlarmMax = _settings->getECalarmUp();
	_ecAlarmMin = _settings->getECalarmDown();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(pmChar(uEcLimit),_xConfig,_yTwoLnsFirst);
	_lcd->print(pmChar(dEcLimit),_xConfig,_yTwoLnsSecond);
	//Numbers
	int x = (4+strlen_P(uEcLimit))*_bigFontSize;
	_lcd->printNumI(_ecAlarmMax,x,_yTwoLnsFirst,4);
	_lcd->printNumI(_ecAlarmMin,x,_yTwoLnsSecond,4);
	//Buttons
	x += 1.5*_bigFontSize;
	//We have already created the flow buttons in positions 0.._nFlowButtons-1  of buttons array
	_ecAlarmsButtons[_nFlowButtons] = _buttons.addButton(x,_yTwoLnsFirst-_signSpacer,plusStr,BUTTON_SYMBOL);
	_ecAlarmsButtons[_nFlowButtons+1] = _buttons.addButton(x,_yTwoLnsFirst+_signSpacer,minusStr,BUTTON_SYMBOL);
	_ecAlarmsButtons[_nFlowButtons+2] = _buttons.addButton(x,_yTwoLnsSecond-_signSpacer,plusStr,BUTTON_SYMBOL);
	_ecAlarmsButtons[_nFlowButtons+3] = _buttons.addButton(x,_yTwoLnsSecond+_signSpacer,minusStr,BUTTON_SYMBOL);
	//uS Text
	x += 3.5*_bigFontSize;
	_lcd->print(pmChar(unitEcS),x,_yTwoLnsFirst);
	_lcd->print(pmChar(unitEcS),x,_yTwoLnsSecond);
}

//Draws entire screen EC alarms
void WinEcAlarms::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinEcAlarms);
	addFlowButtons(true,true,true,_ecAlarmsButtons);
	print();
	_buttons.drawButtons();
} 

void WinEcAlarms::update() {
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen_P(uEcLimit))*_bigFontSize;
	_lcd->printNumI(_ecAlarmMax,x,_yTwoLnsFirst,4);
	_lcd->printNumI(_ecAlarmMin,x,_yTwoLnsSecond,4);
}

Window::Screen WinEcAlarms::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _ecAlarmsButtons[0]) { return Alarms; }
	//Save
	else if (buttonIndex == _ecAlarmsButtons[1]) {
		_settings->setECalarmUp(_ecAlarmMax);
		_settings->setECalarmDown(_ecAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _ecAlarmsButtons[2]) 
		return MainScreen;
		
	//Max up
	else if(buttonIndex == _ecAlarmsButtons[3]) {
		(_ecAlarmMax >= 9990) ? _ecAlarmMax=0 : _ecAlarmMax += 10;
		update();
	//Max down
	} else if(buttonIndex == _ecAlarmsButtons[4]) {
		(_ecAlarmMax <= 0) ? _ecAlarmMax=9990 : _ecAlarmMax -= 10;
		update();
	//Min up
	} else if(buttonIndex == _ecAlarmsButtons[5]) {
		(_ecAlarmMin >= 9990) ? _ecAlarmMin=0 : _ecAlarmMin += 10;
		update();
	//Min down
	} else if(buttonIndex == _ecAlarmsButtons[6]) {
		(_ecAlarmMin <= 0) ? _ecAlarmMin=9990 : _ecAlarmMin -= 10;
		update();
	}
	return None;
}
