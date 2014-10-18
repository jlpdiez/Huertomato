#include "MainScreen.h"

//Prints mainscreen header text and clock
void MainScreen::printMainHeader() {
	//For small font y = 6. For big one y = 2;
	const int ySpacer = 2;
	
	//Get actual time
	time_t t = now();
	uint8_t hou = hour(t);
	uint8_t min = minute(t);
	uint8_t sec = second(t);

	printHeaderBackground();
	
	//Header title text
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->print("Huertomato 1.2",10,ySpacer);

	//Clock display HH:MM
	//X is calculated from the end of size
	_lcd->printNumI(hou,xSize-(5*bigFontSize)-2,ySpacer,2,'0');
	_lcd->print(":",xSize-(3*bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,xSize-(2*bigFontSize)-2,ySpacer,2,'0');
}

//Updates main header's clock
void MainScreen::updateMainHeader() {
	const int ySpacer = 2;
	
	//Get actual time
	time_t t = now();
	uint8_t hou = hour(t);
	uint8_t min = minute(t);
	uint8_t sec = second(t);
	
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	//Clock display HH:MM
	//X is calculated from the end of size
	_lcd->printNumI(hou,xSize-(5*bigFontSize)-2,ySpacer,2,'0');
	_lcd->print(":",xSize-(3*bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,xSize-(2*bigFontSize)-2,ySpacer,2,'0');
}

