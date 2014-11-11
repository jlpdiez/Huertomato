/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinLvlAlarms.h"

WinLvlAlarms::WinLvlAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings)  { }

WinLvlAlarms::WinLvlAlarms(const WinLvlAlarms &other) : Window(other) { }
	
WinLvlAlarms& WinLvlAlarms::operator=(const WinLvlAlarms& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinLvlAlarms::~WinLvlAlarms() {}
	
Window::Screen WinLvlAlarms::getType() const {
	return Window::LvlAlarms;
}

void WinLvlAlarms::print() {
	const int yFirstLine = 100;
	const int xSpacer = 25;
	const int signSpacer = 22;
	
	_waterAlarmMin = _settings->getWaterAlarm();
	
	char* wLimitS = "Lower Limit:";
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(wLimitS,xSpacer,yFirstLine);
	//Numbers
	int x = (4+strlen(wLimitS))*_bigFontSize;
	_lcd->printNumI(_waterAlarmMin,x,yFirstLine,3);
	//Buttons
	x += 1.5*_bigFontSize;
	waterAlarmsButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,waterAlarmsButtonsText[0],BUTTON_SYMBOL);
	waterAlarmsButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,waterAlarmsButtonsText[1],BUTTON_SYMBOL);
	//percent sign
	x += 2.5*_bigFontSize;
	_lcd->print("%",x,yFirstLine);
} 

//Draws entire screen Nutrient level alarms
void WinLvlAlarms::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(_nameS);
	addFlowButtons(true,true,true,waterAlarmsButtons);
	print();
	_buttons.drawButtons();
}

void WinLvlAlarms::update() {
	const int yFirstLine = 100;
	char* wLimitS = "Lower Limit:";
	
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen(wLimitS))*_bigFontSize;
	_lcd->printNumI(_waterAlarmMin,x,yFirstLine,3);
}

Window::Screen WinLvlAlarms::processTouch(int x,int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == waterAlarmsButtons[0]) { return Alarms; }
	//Save
	else if (buttonIndex == waterAlarmsButtons[1]) {
		_settings->setWaterAlarm(_waterAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == waterAlarmsButtons[2]) { return MainScreen; }
		
	//Up
	else if (buttonIndex == waterAlarmsButtons[3]) {
		(_waterAlarmMin >= 100) ? _waterAlarmMin=0 : _waterAlarmMin++;
		update();
	//Down
	} else if (buttonIndex == waterAlarmsButtons[4]) {
		(_waterAlarmMin <= 0) ? _waterAlarmMin=100 : _waterAlarmMin--;
		update();
	}
	return None;
}
