#include "GUI.h"

//Constructors
GUI::GUI(LiquidCrystal *lcd, Sensors *sensors, Settings *settings)
: _lcd(lcd), _sensors(sensors), _settings(settings) {
	_window = new Window(lcd, sensors, settings);
	_prevADC = 1000;
}

GUI::GUI(const GUI &other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	_window = new Window(_lcd,_sensors,_settings);
	*_window = *other._window;
	_prevADC = other._prevADC;
}

GUI& GUI::operator=(const GUI &other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	*_window = *other._window;
	_prevADC = other._prevADC;
	return *this;
}

//Destructor
GUI::~GUI() {
	delete _window;
}

//First function to call. Setups and shows Splash Screen
void GUI::init() {
	_lcd->begin(16, 2);
	_window->draw();
}

//Second function. Shows Main Screen. Starts user interaction
void GUI::start() {
	if (_window->getType() == Window::None) {
		delete _window;
		_window = new WinMainScreen(_lcd,_sensors,_settings);
		_window->draw();
	}
}

//Refreshes non-static windows.
void GUI::refresh() {
	/*Window::Screen actScreen = _window->getType();
	if ((actScreen == Window::MainScreen) || (actScreen == Window::NightWater)
		|| (actScreen == Window::LvlCalib))
			_window->update();	*/
}

boolean GUI::isMainScreen() {
	return _window->getType() == Window::MainScreen;
}

void GUI::updateScreen(Window::Screen newScreen) {
	if (newScreen != Window::None)
		delete _window;
	switch (newScreen) {
		case Window::None:
			break;
		case Window::MainScreen:
			_window = new WinMainScreen(_lcd,_sensors,_settings);
			break;
		case Window::TimeDate:
			_window = new WinTime(_lcd,_sensors,_settings);
			break;
		case Window::WateringCycle:
			_window = new WinWater(_lcd,_sensors,_settings);
			break;
		default:
			_window = new Window(_lcd,_sensors,_settings);
			break;
	}
}

//Reads x,y press and calls one function or another depending on active screen
void GUI::processInput() {
	int adcIn = analogRead(lcdA0);
	//Some button pressed and distance between actual and previous analog value > 150
	if ((adcIn < 1000) && (adcIn != _prevADC)) {
		_prevADC = adcIn;
		Button but = None;
		if (adcIn < 50) 
			but = Right;
		else if (adcIn < 195)  
			but = Up;
		else if (adcIn < 380)
			but = Down;
		else if (adcIn < 555)
			but = Left;
		else if (adcIn < 790)
			but = Select;
		Window::Screen newScreen = _window->processTouch(but);
		if (newScreen != Window::None) {
			_prevADC = 1000;
			updateScreen(newScreen);
			_window->draw();
		}
	}
}