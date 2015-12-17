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
	_lcd->clear();
	_lcd->setCursor(1,0);
	//First line
	_lcd->print(_sensors->getTemp(),1);
	_lcd->print(pmChar(celsStr));
	_lcd->print(" LVL:");
	_lcd->print(_sensors->getWaterLevel());
	_lcd->print(pmChar(percentSign));
	//Second line
	_lcd->setCursor(0,1);
	//Prints one or two decimals depending on value
	float ph = _sensors->getPH();
	(ph < 10) ? _lcd->print(ph,2) : _lcd->print(ph,1);
	_lcd->print("pH EC:");
	//Prints one or two decimals depending on value
	float ec = _sensors->getEC();
	(ec < 10) ? _lcd->print(ec,2) : _lcd->print(ec,1);
	_lcd->print(pmChar(miliSiemens));
}

//Refreshes minimum data
void WinMainScreen::update() {
	//First line
	_lcd->setCursor(1,0);
	_lcd->print(_sensors->getTemp(),1);
	_lcd->setCursor(11,0);
	_lcd->print(_sensors->getWaterLevel());
	_lcd->print(pmChar(percentSign));
	_lcd->print(pmChar(spaceChar));
	_lcd->print(pmChar(spaceChar));
	//Second line
	_lcd->setCursor(0,1);
	_lcd->print(_sensors->getPH(),1);
	_lcd->setCursor(10,1);
	float ec = _sensors->getEC();
	(ec < 10) ? _lcd->print(_sensors->getEC(),2) : _lcd->print(_sensors->getEC(),1);
}

Window::Screen WinMainScreen::processTouch(int but) { 
	//Select button
	if (but == 5)
		return WateringCycle;
	else
		return None;
}