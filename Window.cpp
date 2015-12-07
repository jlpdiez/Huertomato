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
	_lcd->print("Loading system...");
}

void Window::draw() {
	print();
}

void Window::update() {  
	print();
}

Window::Screen Window::processTouch(int but) { return Splash; }
	
/*//Returns the x where the PROGMEM char* should be printed for it to get centered in screen
int Window::centerX(const char* c) {
	return (_xSize / 2) - (_bigFontSize * (strlen_P(c) / 2));
}*/

//Converts a char array from PROGMEM to variable in SRAM
char* Window::pmChar(const char *pmArray) {
	strcpy_P(_stringBuffer, (char*)pmArray);
	return _stringBuffer;
}