#include "GUI.h"

//Constructors
GUI::GUI(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: _lcd(lcd), _touch(touch), _sensors(sensors), _settings(settings) {
	_window = new Window(lcd, touch, sensors, settings);
}

GUI::GUI(const GUI &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_window = new Window(_lcd,_touch,_sensors,_settings);
	*_window = *other._window;
}

GUI& GUI::operator=(const GUI &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	*_window = *other._window;

	return *this;
}

//Destructor
GUI::~GUI() {
	delete _window;
}

void GUI::init() {
	_lcd->InitLCD();
	_lcd->clrScr();
	_lcd->fillScr(VGA_WHITE);
	_touch->InitTouch();
	_touch->setPrecision(PREC_HI);
}

void GUI::refresh() {
	//Refresh screens if needed
	//if (gui.getActScreen() == 0)
	//TODO:
	//gui.updateMainScreen();
	//Nutrient level calibration
	//else if (gui.getActScreen() == 14)
	//TODO:
	//gui.updateWaterCalibration();
	//Night threshold calibration
	//else if (gui.getActScreen() == 15)
	//TODO:
	//gui.updateLightCalibration();
}

boolean GUI::isMainScreen() {
	return _actScreen == 1;
}

void GUI::createAndRenderWindow() {
	delete _window;
	switch (_actScreen) {
		case 0:		
			break;
		case 1:
			_window = new WinMainScreen(_lcd,_touch,_sensors,_settings);
			break;
		default:
			_window = new Window(_lcd,_touch,_sensors,_settings);
			break;
	}
	_window->draw();
}

//Reads x,y press and calls one function or another depending on active screen
void GUI::processTouch() {
	if (_touch->dataAvailable()) {
		_touch->read();
		int x = _touch->getX();
		int y = _touch->getY();
		
		int newScreen = _window->processTouch(x,y);
		if (newScreen != 0) {
			_actScreen = newScreen;
			createAndRenderWindow();
		}
	}
}