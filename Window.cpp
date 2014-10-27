#include "Window.h"

//Constructors
Window::Window(UTFT *lcd, UTouch *touch) : _lcd(lcd), _touch(touch), _buttons(lcd,touch) {
	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
	_actScreen = 0;
}

Window::Window(const Window &other) : _buttons(other._buttons) {
	_lcd = other._lcd;
	_touch = other._touch;
	_actScreen = 0;
}

Window& Window::operator=(const Window &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_buttons = other._buttons;
	_actScreen = 0;
	return *this;
}

//Destructor
Window::~Window() {}

//Getter
int Window::getActScreen() const {	return _actScreen; }

//Prints header background and separator line
void Window::printHeaderBackground() {
	const int headerHeight = 20;
	//Header background
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->fillRect(0,0,xSize,headerHeight);
	//Separator line
	_lcd->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
	_lcd->drawLine(0, headerHeight, xSize, headerHeight);
}