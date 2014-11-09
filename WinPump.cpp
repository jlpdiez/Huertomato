/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinPump.h"

WinPump::WinPump(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinPump::WinPump(const WinPump &other) : Window(other) { }
	
WinPump& WinPump::operator=(const WinPump& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinPump::~WinPump() {}

void WinPump::print() {
	const int yFirstLine = 100;
	const int xSpacer = 25;
	const int signSpacer = 22;
	
	_pumpProtectionLvl = _settings->getPumpProtectionLvl();
	
	char* wLimitS = "Min Water Lvl:";
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Text
	_lcd->print(wLimitS,xSpacer,yFirstLine);
	//Numbers
	int x = (4+strlen(wLimitS))*_bigFontSize;
	_lcd->printNumI(_pumpProtectionLvl,x,yFirstLine,3);
	//Buttons
	x += 1.5*_bigFontSize;
	pumpProtectionButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,pumpProtectionButtonsText[0],BUTTON_SYMBOL);
	pumpProtectionButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,pumpProtectionButtonsText[1],BUTTON_SYMBOL);
	//percent sign
	x += 2.5*_bigFontSize;
	_lcd->print("%",x,yFirstLine);
}

//Draws entire screen Pump Protection
void WinPump::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Pump Protection -");
	addFlowButtons(true,true,true,pumpProtectionButtons);
	print();
	_buttons.drawButtons();
}

void WinPump::update() {
	const int yFirstLine = 100;
	char* wLimitS = "Lower Limit:";
	
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen(wLimitS))*_bigFontSize;
	_lcd->printNumI(_pumpProtectionLvl,x,yFirstLine,3);
}

Window::Screen WinPump::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == pumpProtectionButtons[0]) { return SystemSettings; }
	//Save
	else if (buttonIndex == pumpProtectionButtons[1]) {
		_settings->setPumpProtectionLvl(_pumpProtectionLvl);
		printSavedButton();
	//Exit
	} else if (buttonIndex == pumpProtectionButtons[2]) { return MainScreen; }
		
	//Up
	else if (buttonIndex == pumpProtectionButtons[3]) {
		(_pumpProtectionLvl >= 100) ? _pumpProtectionLvl=0 : _pumpProtectionLvl++;
		update();
	//Down
	} else if (buttonIndex == pumpProtectionButtons[4]) {
		(_pumpProtectionLvl <= 0) ? _pumpProtectionLvl=100 : _pumpProtectionLvl--;
		update();
	}
	return None;
}