#include "WinNutrient.h"

WinNutrient::WinNutrient(LiquidCrystal *lcd, Sensors *sensors, Settings *settings)
: Window(lcd,sensors,settings) {}

WinNutrient::WinNutrient(const WinNutrient &other) : Window(other) {}

WinNutrient& WinNutrient::operator=(const WinNutrient& other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	return *this;
}

WinNutrient::~WinNutrient() {}

Window::Screen WinNutrient::getType() const {
	return Window::NutrientCounter;
}

//Draws entire screen Watering Cycle
void WinNutrient::draw() {
	_lcd->clear();
	_lcd->setCursor(0,0);
	_lcd->print(pmChar(cambioTxt));
	_lcd->setCursor(5,1);
	_lcd->print("12");
	_lcd->setCursor(8,1);
	_lcd->print(pmChar(cambioTxt1));
}

//Redraws only water cycle numbers & text from inner temp vars
//Used when +- signs are pressed
void WinNutrient::update() {
	
}

Window::Screen WinNutrient::processTouch(int but) {
	//Select - Saves and changes screen
	if (but == 5) {
		_lcd->noCursor();
		return MainScreen;
	}
	return None;
}