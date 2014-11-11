#include "Window.h"

//Constructors
Window::Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: _lcd(lcd), _touch(touch), _sensors(sensors),_settings(settings),_buttons(lcd,touch) {
	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
}

Window::Window(const Window &other) : _buttons(other._buttons) {
	_lcd = other._lcd;
	_touch = other._touch;	
	_sensors = other._sensors;
	_settings = other._settings;
}

Window& Window::operator=(const Window &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}
 
//Destructor
Window::~Window() {}
	
Window::Screen Window::getType() const {
	return Window::None;
}

//Draw splash Screen
//TODO: warn when no RTC or SD present- would be cool if we asked for a touchScreen
void Window::print() {
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Shows centered icon
	_lcd->drawBitmap(_xSize/2-(_iconSize/2),10,_iconSize,_iconSize,logo126);
	//Shows centered text
	char* message = "Huertomato is loading...";
	_lcd->print(message,_xSize/2-(_bigFontSize*(strlen(message)/2)),50+_iconSize);
}

void Window::draw() {
	_lcd->fillScr(VGA_WHITE);
	print();
}

void Window::update() {  
	print();
}

Window::Screen Window::processTouch(const int x, const int y) { return Splash; }
	
//These function should be the first to get its buttons into the array buttons
//It gets input button array and adds appropriate back/save/cancel to positions 0, 1 & 2
void Window::addFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]) {
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		
	if (backButton) {
		const int backX = 15;
		_lcd->drawLine(backX-1,_buttonY-5,backX+_bigFontSize*strlen(backText),_buttonY-5);
		buttonArray[0] = _buttons.addButton(backX, _buttonY, backText);
	} else
		buttonArray[0] = -1;
		
	if (saveButton) {
		const int saveX = _xSize/2 - _bigFontSize*strlen(saveText)/2;
		_lcd->drawLine(saveX-1,_buttonY-5,saveX+_bigFontSize*strlen(saveText),_buttonY-5);
		buttonArray[1] = _buttons.addButton(saveX, _buttonY, saveText);
	} else
		buttonArray[1] = -1;
		
	if (exitButton) {
		const int cancelX = _xSize - 15 - _bigFontSize*strlen(cancelText);
		_lcd->drawLine(cancelX-1,_buttonY-5,cancelX+_bigFontSize*strlen(cancelText),_buttonY-5);
		buttonArray[2] = _buttons.addButton(cancelX, _buttonY, cancelText);
	} else
		buttonArray[2] = -1;
}

//Prints header background and separator line
void Window::printHeaderBackground() {
	//Header background
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->fillRect(0,0,_xSize,_headerHeight);
	//Separator line
	_lcd->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
	_lcd->drawLine(0, _headerHeight, _xSize, _headerHeight);
}

//Prints header with centered text
void Window::printMenuHeader(char* c) {
	printHeaderBackground();
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	//Print title centered
	_lcd->print(c,_xSize/2-(_bigFontSize*(strlen(c)/2)),2);
}

//Overlays "Saved" text over save button
//Used when button is pressed to inform the user values have been stored
void Window::printSavedButton() {
	const int saveX = _xSize/2 - _bigFontSize*strlen(savedText)/2;
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->print(savedText,saveX,_buttonY);
}