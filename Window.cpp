#include "Window.h"

//Constructors
Window::Window(UTFT *lcd, UTouch *touch) : _lcd(lcd), _touch(touch), _buttons(lcd,touch) {
	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
	//Colours
	lightGreen[3] = {184, 210, 60};
	darkGreen[3] = {141, 170, 39};
	grey[3] = {100,100,100};
	white[3] = {255,255,255};
}

Window::Window(const Window &other) : _buttons(other._buttons) {
	_lcd = other._lcd;
	_touch = other._touch;	
}

Window& Window::operator=(const Window &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_buttons = other._buttons;

	return *this;
}

//Destructor
Window::~Window() {}
	
//Draw splash Screen
//TODO: warn when no RTC or SD present- would be cool if we asked for a touchScreen
void Window::drawSplashScreen() {
	//_window->drawSplashScreen();
	const int iconSize = 126;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Shows centered icon
	_lcd->drawBitmap(xSize/2-(iconSize/2),10,iconSize,iconSize,logo126);
	//Shows centered text
	char* message = "Huertomato is loading...";
	_lcd->print(message,xSize/2-(bigFontSize*(strlen(message)/2)),50+iconSize);
}

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