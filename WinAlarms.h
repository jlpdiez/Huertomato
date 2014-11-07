/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINALARMS_H_
#define WINALARMS_H_

#include "Window.h"
/*
	//8-Sensor Alarms
	*/
const int nSensorAlarmsButtons = 6;
static char* sensorAlarmsButtonsText[nSensorAlarmsButtons] = {
	//"Auto Config",
	"pH Thresholds",
	"EC Thresholds",
	"Nutrient Level"
};
static int sensorAlarmsButtons[nSensorAlarmsButtons];


class WinAlarms: public Window {
	public:
		WinAlarms(UTFT *lcd, UTouch *touch);
		WinAlarms(const WinAlarms &other);
		WinAlarms& operator=(const WinAlarms &other);
		virtual ~WinAlarms();
		void draw();
		int processTouch(int x, int y);
	
	protected:
		void print();
};



#endif