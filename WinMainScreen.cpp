#include "WinMainScreen.h"

WinMainScreen::WinMainScreen(LiquidCrystal *lcd, Sensors *sensors, Settings *settings) 
: Window(lcd,sensors,settings) {}

WinMainScreen::WinMainScreen(const WinMainScreen &other) : Window(other) {}
WinMainScreen& WinMainScreen::operator=(const WinMainScreen &other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	return *this;
}

WinMainScreen::~WinMainScreen() {}
	
Window::Screen WinMainScreen::getType() const {
	return Window::MainScreen;
}

void WinMainScreen::draw() {
	//_lcd->fillScr(VGA_WHITE);
	/*printMainHeader();
	(_settings->getReservoirModule()) ? printInfoReservoir() : printInfoNoModule();
	printIconAndStatus();*/
	_lcd->clear();
	_lcd->setCursor(0,0);
	_lcd->print("Main");
	_lcd->setCursor(0,1);
	_lcd->print("Screen");
}

//Refreshes minimun or redraws if status of system has changed
void WinMainScreen::update() {
	/*if (!_settings->moduleChanged()) {
		updateMainHeader();
		(_settings->getReservoirModule()) ? updateInfoReservoir() : updateInfoNoModule();
		updateIconAndStatus();
	} else 
		draw();*/
}

Window::Screen WinMainScreen::processTouch(int but) { 
	if (but == 4)
		return WateringCycle;
	else if (but == 1)
		return TimeDate; 
	else
		return None;
}