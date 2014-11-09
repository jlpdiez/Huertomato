/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINLVLALARMS_H_
#define WINLVLALARMS_H_

#include "Window.h"

//11-Nutrient Level Alarms

const int nWaterAlarmsButtons = 5;
static char* waterAlarmsButtonsText[nWaterAlarmsButtons] = {
	"=",
	">"
};
static int waterAlarmsButtons[nWaterAlarmsButtons];


class WinLvlAlarms: public Window {
	public:
		WinLvlAlarms(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinLvlAlarms(const WinLvlAlarms &other);
		WinLvlAlarms& operator=(const WinLvlAlarms &other);
		virtual ~WinLvlAlarms();
		void draw();
		void update();
		Window::Screen processTouch(const int x, const int y);
	
	protected:
		uint8_t _waterAlarmMin;
		void print();
};



#endif