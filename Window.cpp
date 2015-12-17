#include "Window.h"

//Constructors
Window::Window(LiquidCrystal *lcd, Sensors *sensors, Settings *settings) 
: _lcd(lcd), _sensors(sensors),_settings(settings) {}

Window::Window(const Window &other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
}

Window& Window::operator=(const Window &other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	return *this;
}
 
//Destructor
Window::~Window() {}
	
Window::Screen Window::getType() const {
	return Window::None;
}

//Draw splash Screen
void Window::print() {
	_lcd->setCursor(0,0);
	_lcd->print(pmChar(loadingText));
}

void Window::draw() {
	print();
}

void Window::update() {  
	print();
}

Window::Screen Window::processTouch(int but) { return Splash; }

//Converts a char array from PROGMEM to variable in SRAM
char* Window::pmChar(const char *pmArray) {
	strcpy_P(_stringBuffer, (char*)pmArray);
	return _stringBuffer;
}