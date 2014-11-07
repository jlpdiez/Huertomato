/*
 * MainMenu.h
 *
 * Created: 07/11/2014 1:19:48
 *  Author: HAL
 */ 


#ifndef WINSENSORPOLLING_H_
#define WINSENSORPOLLING_H_

#include "Window.h"

//5-Sensor Polling

const int nSensorPollingButtons = 5;
static char* sensorPollingButtonText[nSensorPollingButtons] = {
	"=",
	">"
};
static int sensorPollingButtons[nSensorPollingButtons];


class WinSensorPolling: public Window {
	public:
		WinSensorPolling(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		WinSensorPolling(const WinSensorPolling &other);
		WinSensorPolling& operator=(const WinSensorPolling &other);
		virtual ~WinSensorPolling();
		void draw();
		void update();
		int processTouch(int x, int y);
	
	protected:
		static uint8_t _pollSec;
		void print();
};

#endif