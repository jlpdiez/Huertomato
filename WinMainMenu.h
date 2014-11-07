/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINMAINMENU_H_
#define WINMAINMENU_H_

#include "Window.h"

const int nMainMenuButtons = 7;
static char* mainMenuButtonText[nMainMenuButtons] = {
	"System Settings",
	"Controller Settings"
};
static int mainMenuButtons[nMainMenuButtons];


class WinMainMenu: public Window {
	public:
		WinMainMenu(UTFT *lcd, UTouch *touch);
		WinMainMenu(const WinMainMenu &other);
		WinMainMenu& operator=(const WinMainMenu &other);
		virtual ~WinMainMenu();
		void draw();
		int processTouch(int x, int y);
	
	protected:
		void print();
};



#endif