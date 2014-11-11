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

//First function to call. Setups and shows Splash Screen
void GUI::init() {
	_lcd->InitLCD();
	_lcd->clrScr();
	_lcd->fillScr(VGA_WHITE);
	_touch->InitTouch();
	_touch->setPrecision(PREC_HI);
	_window->draw();
}

//Second function. Shows Main Screen. Starts user interaction
void GUI::start() {
	if (_window->getType() == Window::None) {
		delete _window;
		_window = new WinMainScreen(_lcd,_touch,_sensors,_settings);
		_window->draw();
	}
}

//Refreshes non-static windows.
void GUI::refresh() {
	Window::Screen actScreen = _window->getType();
	if ((actScreen == Window::MainScreen) || (actScreen == Window::LightCalib)
		|| (actScreen == Window::LvlCalib))
			_window->update();	
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
			_window = new WinMainScreen(_lcd,_touch,_sensors,_settings);
			break;
		case Window::MainMenu:
			_window = new WinMainMenu(_lcd,_touch,_sensors,_settings);
			break;
		case Window::SystemSettings:
			_window = new WinSystemMenu(_lcd,_touch,_sensors,_settings);
			break;
		case Window::ControllerSettings:
			_window = new WinControllerMenu(_lcd,_touch,_sensors,_settings);
			break;
		case Window::TimeDate:
			_window = new WinTime(_lcd,_touch,_sensors,_settings);
			break;
		case Window::SensorPolling:
			_window = new WinSensorPolling(_lcd,_touch,_sensors,_settings);
			break;
		case Window::SDCard:
			_window = new WinSD(_lcd,_touch,_sensors,_settings);
			break;
		case Window::WateringCycle:
			_window = new WinWater(_lcd,_touch,_sensors,_settings);
			break;
		case Window::Alarms:
			_window = new WinAlarms(_lcd,_touch,_sensors,_settings);	
			break;
		case Window::PhAlarms:
			_window = new WinPhAlarms(_lcd,_touch,_sensors,_settings);
			break;
		case Window::EcAlarms:
			_window = new WinEcAlarms(_lcd,_touch,_sensors,_settings);
			break;
		case Window::LvlAlarms:
			_window = new WinLvlAlarms(_lcd,_touch,_sensors,_settings);
			break;
		case Window::SensorCalib:
			_window = new WinSensorCalib(_lcd,_touch,_sensors,_settings);
			break;
		case Window::LvlCalib:
			_window = new WinLvlCalib(_lcd,_touch,_sensors,_settings);
			break;
		case Window::LightCalib:
			_window = new WinLightCalib(_lcd,_touch,_sensors,_settings);
			break;
		case Window::Pump:
			_window = new WinPump(_lcd,_touch,_sensors,_settings);
			break;
		default:
			_window = new Window(_lcd,_touch,_sensors,_settings);
			break;
	}
}

//Reads x,y press and calls one function or another depending on active screen
void GUI::processTouch() {
	if (_touch->dataAvailable()) {
		_touch->read();
		int x = _touch->getX();
		int y = _touch->getY();
		
		Window::Screen newScreen = _window->processTouch(x,y);
		if (newScreen != Window::None) {
			updateScreen(newScreen);
			_window->draw();
		}
	}
}