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

//Draws entire screen
void WinNutrient::draw() {
	_lcd->clear();
	_lcd->setCursor(0,0);
	_lcd->print(pmChar(cambioTxt));
	//Get days remanining for nutrient change
	int days = 	_settings->getNutChangeElapsed() - elapsedDays(now());
	if (days >= 0) {
		_lcd->setCursor(5,1);
		_lcd->print(days);
		_lcd->setCursor(8,1);
		_lcd->print(pmChar(cambioTxt1));
	//Se ha pasado la fecha del cambio
	} else {
		_lcd->setCursor(4,1);
		_lcd->print(pmChar(pasadoTxT));
	}
}

void WinNutrient::update() {
	draw();
}

Window::Screen WinNutrient::processTouch(int but) {
	//Select - Changes screen
	if (but == 5) {
		_lcd->noCursor();
		return MainScreen;
	//Up, down, left or right resets counter again
	} else if ((but == 1) || (but == 2) || (but == 3) || (but == 4)) {
		_settings->resetNutrientChangeDate();
		draw();	
	}
	return None;
}