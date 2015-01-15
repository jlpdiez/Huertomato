#include "Window.h"

//Constructors
Window::Window(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: _lcd(lcd), _touch(touch), _sensors(sensors),_settings(settings),_buttons(lcd,touch) {
	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, grey, white);
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
void Window::print() {
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Shows centered icon
	_lcd->drawBitmap(_xSize/2-(_bigIconSize/2),10,_bigIconSize,_bigIconSize,logo126);
	//Shows centered text
	_lcd->print(pmChar(loadingText),centerX(loadingText),50+_bigIconSize);
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
void Window::addFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int8_t buttonArray[]) {
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		
	if (backButton) {
		const int backX = 15;
		_lcd->drawLine(backX-1,_flowButtonY-5,backX+_bigFontSize*strlen_P(backText),_flowButtonY-5);
		buttonArray[0] = _buttons.addButton(backX, _flowButtonY, backText);
	} else
		buttonArray[0] = -1;
		
	if (saveButton) {
		const int saveX = centerX(saveText);
		_lcd->drawLine(saveX-1,_flowButtonY-5,saveX+_bigFontSize*strlen_P(saveText),_flowButtonY-5);
		buttonArray[1] = _buttons.addButton(saveX, _flowButtonY, saveText);
	} else
		buttonArray[1] = -1;
		
	if (exitButton) {
		const int cancelX = _xSize - 15 - _bigFontSize*strlen_P(cancelText);
		_lcd->drawLine(cancelX-1,_flowButtonY-5,cancelX+_bigFontSize*strlen_P(cancelText),_flowButtonY-5);
		buttonArray[2] = _buttons.addButton(cancelX, _flowButtonY, cancelText);
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

//Prints header with centered text. Adds headerDecoration to start and end of title
void Window::printMenuHeader(const char* c) {
	printHeaderBackground();
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	//Prepare header string and convert to char array
	String title = (String)pmChar(headerDecoration) + (String)pmChar(spaceChar) + (String)pmChar(c) + (String)pmChar(spaceChar) + (String)pmChar(headerDecoration);
	char titleArray[title.length() + 1];
	title.toCharArray(titleArray, sizeof(titleArray));
	//Print title centered
	_lcd->print(titleArray,(_xSize/2)-(_bigFontSize*(strlen(titleArray)/2)),2);
}

//Overlays "Saved" text over save button
//Used when button is pressed to inform the user values have been stored
void Window::printSavedButton() {
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->print(pmChar(savedText),centerX(savedText),_flowButtonY);
}

//Returns the x where the PROGMEM char* should be printed for it to get centered in screen
int Window::centerX(const char* c) {
	return (_xSize / 2) - (_bigFontSize * (strlen_P(c) / 2));
}

//Converts a char array from PROGMEM to variable in SRAM
char* Window::pmChar(const char *pmArray) {
	strcpy_P(_stringBuffer, (char*)pmArray);
	return _stringBuffer;
}