#include "Window.h"

//Constructors
Window::Window(UTFT *lcd, UTouch *touch) : _lcd(lcd), _touch(touch), _buttons(lcd,touch) {
	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, grey, white, grey, white);
	printWindow();
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
void Window::printWindow() {
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

void Window::draw() {
	printWindow();
}

void Window::update() {  
	printWindow();
}

int Window::processTouch(int x, int y) { return 0; }

	
//These function should be the first to get its buttons into the array buttons
//It gets input button array and adds appropriate back/save/cancel to positions 0, 1 & 2
void Window::addFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]) {
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(hallfetica_normal);
		
	if (backButton) {
		const int backX = 15;
		const int backY = 215;
		char* backText = " Back ";
		//_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
		_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		_lcd->drawLine(backX-1,backY-5,backX+bigFontSize*strlen(backText),backY-5);
		buttonArray[0] = _buttons.addButton(backX, backY, backText);
	} else
	buttonArray[0] = -1;
		
	if (saveButton) {
		char* saveText = " Save ";
		const int saveX = xSize/2 - bigFontSize*strlen(saveText)/2;
		const int saveY = 215;
		//_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
		_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		_lcd->drawLine(saveX-1,saveY-5,saveX+bigFontSize*strlen(saveText),saveY-5);
		buttonArray[1] = _buttons.addButton(saveX, saveY, saveText);
	} else
	buttonArray[1] = -1;
		
	if (exitButton) {
		char* cancelText = " Exit ";
		const int cancelX = xSize - 15 - bigFontSize*strlen(cancelText);
		const int cancelY = 215;
		//_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
		_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		_lcd->drawLine(cancelX-1,cancelY-5,cancelX+bigFontSize*strlen(cancelText),cancelY-5);
		buttonArray[2] = _buttons.addButton(cancelX, cancelY, cancelText);
	} else
	buttonArray[2] = -1;
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

//Prints header with centered text
void Window::printMenuHeader(char* c) {
	printHeaderBackground();
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	//Print title centered
	_lcd->print(c,xSize/2-(bigFontSize*(strlen(c)/2)),2);
}



//Overlays "Saved" text over save button
//Used when button is pressed to inform the user values have been stored
void Window::printSavedButton() {
	char* savedText = " Saved ";
	const int saveX = xSize/2 - bigFontSize*strlen(savedText)/2;
	const int saveY = 215;
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->print(savedText,saveX,saveY);
}