/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinSystemMenu.h"

WinSystemMenu::WinSystemMenu(UTFT *lcd, UTouch *touch) : Window(lcd,touch) { }

WinSystemMenu::WinSystemMenu(const WinSystemMenu &other) : Window(other) { }
	
WinSystemMenu& WinSystemMenu::operator=(const WinSystemMenu& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	return *this;
}

/*
//Draws main menu into LCD
//_actScreen == 1
void GUI::drawMainMenu() {
	_actScreen = 1;
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Main Menu -");
	printFlowButtons(false,false,true,mainMenuButtons);
	printMainMenu();
	_buttons.drawButtons();
}
*/