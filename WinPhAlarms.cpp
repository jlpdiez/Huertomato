#include "WinPhAlarms.h"

WinPhAlarms::WinPhAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinPhAlarms::WinPhAlarms(const WinPhAlarms &other) : Window(other) {
	for (uint8_t i = 0; i < _nPHalarmsButtons; i++) {
		_phAlarmsButtons[i] = other._phAlarmsButtons[i];
	}
}
	
WinPhAlarms& WinPhAlarms::operator=(const WinPhAlarms& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nPHalarmsButtons; i++) {
		_phAlarmsButtons[i] = other._phAlarmsButtons[i];
	}
	return *this;
}

WinPhAlarms::~WinPhAlarms() {}
	
Window::Screen WinPhAlarms::getType() const {
	return Window::PhAlarms;
}

void WinPhAlarms::print() {
	_phAlarmMax = _settings->getPHalarmUp();
	_phAlarmMin = _settings->getPHalarmDown();
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(pmChar(uPhLimit),_xConfig,_yTwoLnsFirst);
	_lcd->print(pmChar(dPhLimit),_xConfig,_yTwoLnsSecond);
	//Numbers
	int x = (4+strlen_P(uPhLimit))*_bigFontSize;
	_lcd->printNumF(_phAlarmMax,2,x,_yTwoLnsFirst,'.',5);
	_lcd->printNumF(_phAlarmMin,2,x,_yTwoLnsSecond,'.',5);
	//Buttons
	x += 2*_bigFontSize;
	_phAlarmsButtons[_nFlowButtons] = _buttons.addButton(x,_yTwoLnsFirst-_signSpacer,plusStr,BUTTON_SYMBOL);
	_phAlarmsButtons[_nFlowButtons+1] = _buttons.addButton(x,_yTwoLnsFirst+_signSpacer,minusStr,BUTTON_SYMBOL);
	_phAlarmsButtons[_nFlowButtons+2] = _buttons.addButton(x,_yTwoLnsSecond-_signSpacer,plusStr,BUTTON_SYMBOL);
	_phAlarmsButtons[_nFlowButtons+3] = _buttons.addButton(x,_yTwoLnsSecond+_signSpacer,minusStr,BUTTON_SYMBOL);
}

//Draws entire screen pH Alarms
void WinPhAlarms::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinPhAlarms);
	addFlowButtons(true,true,true,_phAlarmsButtons);
	print();
	_buttons.drawButtons();
} 

void WinPhAlarms::update() {
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen_P(uPhLimit))*_bigFontSize;
	_lcd->printNumF(_phAlarmMax,2,x,_yTwoLnsFirst,'.',5);
	_lcd->printNumF(_phAlarmMin,2,x,_yTwoLnsSecond,'.',5);
}

Window::Screen WinPhAlarms::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _phAlarmsButtons[0]) 
		return Alarms;
	//Save
	else if (buttonIndex == _phAlarmsButtons[1]) {
		_settings->setPHalarmUp(_phAlarmMax);
		_settings->setPHalarmDown(_phAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _phAlarmsButtons[2]) 
		return MainScreen;
		
	//Max up
	else if(buttonIndex == _phAlarmsButtons[3]) {
		(_phAlarmMax >= 14) ? _phAlarmMax=0 : _phAlarmMax += 0.05;
		update();
	//Max down
	} else if(buttonIndex == _phAlarmsButtons[4]) {
		(_phAlarmMax <= 0) ? _phAlarmMax=14 : _phAlarmMax -= 0.05;
		update();
	//Min up
	} else if(buttonIndex == _phAlarmsButtons[5]) {
		(_phAlarmMin >= 14) ? _phAlarmMin=0 : _phAlarmMin += 0.05;
		update();
	//Min down
	} else if(buttonIndex == _phAlarmsButtons[6]) {
		(_phAlarmMin <= 0) ? _phAlarmMin=14 : _phAlarmMin -= 0.05;
		update();
	}
	return None;
}