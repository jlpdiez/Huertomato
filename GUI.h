// #############################################################################
// 
// # Name       : GUI
// # Version    : 2.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 04.05.2014
// 
// # Description: Library for controlling Huertomato's GUI
// # Contains all windows and manages its transition and button presses
// 
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
//#include "Settings.h"
//#include "Sensors.h" 
#include "Window.h"
#include <UTFT.h>
#include <UTouch.h>
//#include <UTFT_Buttons.h>
//#include <Time.h> 
//#include <TimeAlarms.h>
//#include <SD.h>    


//Holds text strings for each sensor
/*const int nSensorText = 6;
static char* sensorText[nSensorText] = {
	"Humidity:",
	"Temp:",
	"Light:",
	"pH:",
	"EC:",
	"Deposit:"
};*/

//Here lies the button info for each window
//We add 3 to all nButtons to account for back/save/exit (These are always 0,1,2)
//static in this context means vars will only be accesible in this file
//http://www.cprogramming.com/tutorial/statickeyword.html

const int nMainMenuButtons = 7;
static char* mainMenuButtonText[nMainMenuButtons] = {
	"System Settings",
	"Controller Settings"
};
static int mainMenuButtons[nMainMenuButtons];

const int nControllerButtons = 8;
static char* controllerButtonText[nControllerButtons] = {
	"Time & Date",
	"Sensor Polling",
	"SD Card",
	"Sound:",
	"Serial Debugging:"
};
static int controllerButtons[nControllerButtons];

const int nSystemButtons = 8;
static char* systemButtonText[nSystemButtons] = {
	"Watering Cycle",
	"Sensor Alarms",
	"Calibration",
	"Pump protection",
	"Watering at Night:"
}; 
static int systemButtons[nSystemButtons];

//These buttons use symbol font hence the strange texts
const int nTimeButtons = 15;
static char* timeButtonText[nTimeButtons] = {
	"=",
	"=",
	"=",
	">",
	">",
	">",
	"=",
	"=",
	"=",
	">",
	">",
	">" 
};
static int timeButtons[nTimeButtons];

const int nSensorPollingButtons = 5;
static char* sensorPollingButtonText[nSensorPollingButtons] = {
	"=",
	">"
};
static int sensorPollingButtons[nSensorPollingButtons];

const int nSDcardButtons = 8;
static char* sdCardButtonsText[nSDcardButtons] = {
	"SD Card Log:",
	"=",
	"=",
	">",
	">"
};
static int sdCardButtons[nSDcardButtons];

const int nWaterCycleButtons = 10;
static char* waterCycleButtonsText[nWaterCycleButtons] = {
	"Mode:",
	"=",
	">",
	"=",
	">",
	"=",
	">",
};
static int  waterCycleButtons[nWaterCycleButtons];

const int nSensorAlarmsButtons = 6;
static char* sensorAlarmsButtonsText[nSensorAlarmsButtons] = {
	//"Auto Config",
	"pH Thresholds",
	"EC Thresholds",
	"Nutrient Level"
};
static int sensorAlarmsButtons[nSensorAlarmsButtons];

const int nPHalarmsButtons = 7;
static char* phAlarmsButtonsText[nPHalarmsButtons] = {
	"=",
	">",
	"=",
	">"
};
static int phAlarmsButtons[nPHalarmsButtons];

const int nECalarmsButtons = 7;
static char* ecAlarmsButtonsText[nECalarmsButtons] = {
	"=",
	">",
	"=",
	">" 
};
static int ecAlarmsButtons[nECalarmsButtons];

const int nWaterAlarmsButtons = 5;
static char* waterAlarmsButtonsText[nWaterAlarmsButtons] = {
	"=",
	">" 
};
static int waterAlarmsButtons[nWaterAlarmsButtons];

const int nAutoConfigButtons = 13;
static char* autoConfigButtonsText[nAutoConfigButtons] = {
  
};
static int autoConfigButtons[nAutoConfigButtons];

const int nSensorCalibrationButtons = 5;
static char* sensorCalibrationButtonsText[nSensorCalibrationButtons] = {
	"Water Levels",
	"Night Threshold"
};
static int sensorCalibrationButtons[nSensorCalibrationButtons];

const int nWaterLevelButtons = 5;
static char* waterLevelButtonsText[nWaterLevelButtons] = {
	"Set",
	"Set"
};
static int waterLevelButtons[nWaterLevelButtons];

const int nLightCalibrationButtons = 4;
static char* lightCalibrationButtonsText[nLightCalibrationButtons] = {
	"Set"
};
static int lightCalibrationButtons[nLightCalibrationButtons];

const int nPumpProtectionButtons = 5;
static char* pumpProtectionButtonsText[nLightCalibrationButtons] = {
	"=",
	">"
};
static int pumpProtectionButtons[nPumpProtectionButtons];

//These are temp variables used for displaying data
//They are read from _settings in print() funcs. Changed in processTouch()
//displayed again with update() and saved to eeprom when button save is pressed
//System Settings
//Watering cycle
static boolean waterTimed;
static uint8_t waterHour, waterMin, floodMin;
//pH alarms
static float phAlarmMax, phAlarmMin;
//EC alarms
static uint16_t ecAlarmMax, ecAlarmMin;
//Water alarms
static uint8_t waterAlarmMin;
//Water levels calibration
static uint16_t waterLvlMax;
static uint16_t waterLvlMin;
static uint16_t rawWaterLvl;
//Light calibration
static uint16_t lightThreshold;
static uint16_t rawLightLvl;
//Pump protection
static uint8_t pumpProtectionLvl;

//Night toggle
static boolean nightWater;
//Water pump toggle
static boolean waterPumpState;
//Controller Settings
//Time & Date
static uint8_t sysHour, sysMin, sysSec, sysDay, sysMonth;
static int sysYear;
//Sensor Polling
static uint8_t pollMin, pollSec;
//SD Card
static boolean sdActive;
static uint8_t sdHour, sdMin;
//Sound toggle
static boolean soundActive;
//Serial toggle
static boolean serialActive;

enum screens {
	Main = 1,
	Menu = 2
};

class GUI {
	public:		
		//Constructors
		GUI(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings);
		GUI(const GUI &other);
		GUI& operator=(const GUI &other);
		//Destructor
		~GUI();
		
		void init();
		void processTouch();
		void refresh();
		boolean isMainScreen();
		
	private:
		void drawSplashScreen();
		void printWindow(const int screen=0);
			
	    UTFT *_lcd;
	    UTouch *_touch;
	    Sensors *_sensors;
	    Settings *_settings;
		
		//Const inside a class!!
		enum { MAX = 100 };
			
		//Screen currently active
		//0-Main Screen, 1-Main Menu, 2-System Settings, 3-Controller Settings,
		//4-Time & Date, 5-Sensor Polling, 6-SD Card, 7-Watering Cycle
		//8-Sensor Alarms, 9-pH Alarms, 10-EC Alarms, 11-Nutrient Level Alarms,
		//12-Auto Config Alarms, 13-Sensor Calibration, 14-Water Level Calibration
		//15-Light Calibration, 16-Pump Protection
		static uint8_t _actScreen;
		
		Window _window;
};

  
//Handless all the gui
/*
  private:
    //Private functions
    void printHeaderBackground();
    void printMainHeader();
    void updateMainHeader();
    void printMenuHeader(char* c);
    void printFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]);
	void printSavedButton();
    
    void printSensorInfo();
    void updateSensorInfo();
    void printIconAndStatus();
    void updateIconAndStatus();
    
	//1 - Main menu
    void printMainMenu();
    void drawMainMenu();
    void processTouchMainMenu(int x, int y);
    
	//2-System Settings
    void printSystemSettings();
    void drawSystemSettings();
	void updateSystemSettings();
    void processTouchSystem(int x, int y);
    
	//3-Controller Settings
    void printControllerSettings();
    void drawControllerSettings();
	void updateControllerSettings();
    void processTouchController(int x,int y);
    
    //4-Time & Date
    void printTimeSettings();
    void drawTimeSettings();
	void updateTimeSettings();
    void processTouchTime(int x, int y);
    
	//5-Sensor Polling
    void printSensorPolling();
    void drawSensorPolling();
	void updateSensorPolling();  
    void processTouchSensorPolling(int x, int y);
    
	//6-SD Card
    void printSDcard();
    void drawSDcard();
	void updateSDcard();
    void processTouchSDcard(int x, int y);
	
    //7-Watering Cycle
    void printWaterCycle();
    void drawWaterCycle();
	void updateWaterCycle();
    void processTouchWaterCycle(int x, int y);
	
	//8-Sensor Alarms
    void printSensorAlarms();
    void drawSensorAlarms();
    void processTouchSensorAlarms(int x, int y);
    
	//9-pH Alarms
    void printPHalarms();
    void drawPHalarms();
	void updatePHalarms();
    void processTouchPHalarms(int x, int y);
    
	//10-EC Alarms
    void printECalarms();
    void drawECalarms();
	void updateECalarms();
    void processTouchECalarms(int x,int y);
    
	//11-Nutrient Level Alarms
    void printWaterAlarms();
    void drawWaterAlarms();
	void updateWaterAlarms();
    void processTouchWaterAlarms(int x,int y);
	
	/*
    //12-Auto Config Alarms
    void printAutoConfig();
    void drawAutoConfig();
    void processTouchAutoConfig(int x,int y);*/
	/*
	//13-Sensor Calibration
    void printSensorCalibration();
    void drawSensorCalibration();
    void processTouchSensorCalibration(int x, int y);
    
	//14-Water Level Calibration
    void printWaterCalibration();
    void drawWaterCalibration();    
    void processTouchWaterCalibration(int x,int y);
    
	//15-Light Calibration
    void printLightCalibration();
    void drawLightCalibration();
    void processTouchLightCalibration(int x, int y);  
	
	//16-Pump Protection
	void printPumpProtection();
	void drawPumpProtection();
	void updatePumpProtection();
	void processTouchPumpProtection(int x, int y);
};*/
    
#endif
