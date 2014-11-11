/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinEcAlarms.h"

WinEcAlarms::WinEcAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinEcAlarms::WinEcAlarms(const WinEcAlarms &other) : Window(other) { }
	
WinEcAlarms& WinEcAlarms::operator=(const WinEcAlarms& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinEcAlarms::~WinEcAlarms() {}
	
Window::Screen WinEcAlarms::getType() const {
	return Window::EcAlarms;
}

void WinEcAlarms::print() {
	const int yFirstLine = 65;
	const int ySecondLine = 140;
	const int xSpacer = 25;
	const int signSpacer = 22;
	
	_ecAlarmMax = _settings->getECalarmUp();
	_ecAlarmMin = _settings->getECalarmDown();
	
	char* uLimit = "Upper Limit:";
	char* dLimit = "Lower Limit:";
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(uLimit,xSpacer,yFirstLine);
	_lcd->print(dLimit,xSpacer,ySecondLine);
	//Numbers
	int x = (4+strlen(uLimit))*_bigFontSize;
	_lcd->printNumI(_ecAlarmMax,x,yFirstLine,4);
	_lcd->printNumI(_ecAlarmMin,x,ySecondLine,4);
	//Buttons
	x += 1.5*_bigFontSize;
	ecAlarmsButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,ecAlarmsButtonsText[0],BUTTON_SYMBOL);
	ecAlarmsButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,ecAlarmsButtonsText[1],BUTTON_SYMBOL);
	ecAlarmsButtons[5] = _buttons.addButton(x,ySecondLine-signSpacer,ecAlarmsButtonsText[2],BUTTON_SYMBOL);
	ecAlarmsButtons[6] = _buttons.addButton(x,ySecondLine+signSpacer,ecAlarmsButtonsText[3],BUTTON_SYMBOL);
	//uS Text
	x += 3.5*_bigFontSize;
	_lcd->print("uS",x,yFirstLine);
	_lcd->print("uS",x,ySecondLine);
}

//Draws entire screen EC alarms
void WinEcAlarms::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(_nameS);
	addFlowButtons(true,true,true,ecAlarmsButtons);
	print();
	_buttons.drawButtons();
} 

void WinEcAlarms::update() {
	const int yFirstLine = 65;
	const int ySecondLine = 140;
	char* uLimit = "Upper Limit:";
	
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen(uLimit))*_bigFontSize;
	_lcd->printNumI(_ecAlarmMax,x,yFirstLine,4);
	_lcd->printNumI(_ecAlarmMin,x,ySecondLine,4);
}

Window::Screen WinEcAlarms::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == ecAlarmsButtons[0]) { return Alarms; }
	//Save
	else if (buttonIndex == ecAlarmsButtons[1]) {
		_settings->setECalarmUp(_ecAlarmMax);
		_settings->setECalarmDown(_ecAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == ecAlarmsButtons[2]) { return MainScreen; }
		
	//Max up
	else if(buttonIndex == ecAlarmsButtons[3]) {
		(_ecAlarmMax >= 9990) ? _ecAlarmMax=0 : _ecAlarmMax += 10;
		update();
	//Max down
	} else if(buttonIndex == ecAlarmsButtons[4]) {
		(_ecAlarmMax <= 0) ? _ecAlarmMax=9990 : _ecAlarmMax -= 10;
		update();
	//Min up
	} else if(buttonIndex == ecAlarmsButtons[5]) {
		(_ecAlarmMin >= 9990) ? _ecAlarmMin=0 : _ecAlarmMin += 10;
		update();
	//Min down
	} else if(buttonIndex == ecAlarmsButtons[6]) {
		(_ecAlarmMin <= 0) ? _ecAlarmMin=9990 : _ecAlarmMin -= 10;
		update();
	}
	return None;
}
