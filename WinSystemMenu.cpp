/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinSystemMenu.h"

WinSystemMenu::WinSystemMenu(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinSystemMenu::WinSystemMenu(const WinSystemMenu &other) : Window(other) { }
	
WinSystemMenu& WinSystemMenu::operator=(const WinSystemMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}


void WinSystemMenu::print() {
	const int xSpacer = 15;
	const int ySpacer = 35;
	
	_nightWater = _settings->getNightWatering();
	//waterPumpState = _settings->getManualPump();

	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	
	//Print triangles
	_lcd->setFont(various_symbols);
	//Before 3 there are the flow buttons
	for (int i = 0; i < nSystemButtons -3; i++) {
		_lcd->print("T",xSpacer,ySpacer+bigFontSize*2*i);
	}
	
	//Make menu buttons
	//-3 because back/save/exit are already added at this stage
	for (int i = 0; i < nSystemButtons - 3; i++) {
		systemButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*2*i,systemButtonText[i]);
	}
	_lcd->setFont(hallfetica_normal);
	
	//Watering at night ON/OFF
	if (_nightWater)
		_lcd->print("ON",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);
	else
		_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);
	
	//7 buttons
	// _lcd->print("T",xSpacer,ySpacer+bigFontSize*1.5*i);
	//systemButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*1.5*i,systemButtonText[i]);
	//if (waterAtNight)
	//_lcd->print("ON",xSpacer+bigFontSize*3+bigFontSize*strlen(systemButtonText[5]),ySpacer+bigFontSize*1.5*5);
	//if !(manualWaterPump)
	//_lcd->print("OFF",xSpacer+bigFontSize*3+bigFontSize*strlen(systemButtonText[6]),ySpacer+bigFontSize*1.5*6);
}

//Draws entire screen System Settings
void WinSystemMenu::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- System Settings -");
	addFlowButtons(true,false,true,systemButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only system settings text from inner temp vars
//Used when +- signs are pressed
void WinSystemMenu::update() {
	const int xSpacer = 15;
	const int ySpacer = 35;
	
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setFont(hallfetica_normal);
	
	//Watering at night ON/OFF
	if (_nightWater)
	_lcd->print("ON ",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);
	else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);

}

int WinSystemMenu::processTouch(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == systemButtons[0]) { return MainMenu; }
	//Exit
	else if (buttonIndex == systemButtons[2]) { return MainScreen; }
	//Watering Cycle
	else if (buttonIndex == systemButtons[3]) { return WateringCycle; }
	//Sensor Alarms
	else if (buttonIndex == systemButtons[4]) { return Alarms; }
	//Sensor Calibration
	else if (buttonIndex == systemButtons[5]) { return SensorCalib; }
	//Pump Protection
	else if (buttonIndex == systemButtons[6]) { return Pump; }
	//Water at night Toggle
	else if (buttonIndex == systemButtons[7]) {
		_nightWater = !_nightWater;
		_settings->setNightWatering(_nightWater);
		update();
	}
	return 0;
}