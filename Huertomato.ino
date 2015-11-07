// #############################################################################
// #
// # Name       : Huertomato
// # Version    : 1.5.1
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 07.11.2015
// 
// # Description:
// # Implements an Arduino-based system for controlling hydroponics, aquaponics and the like
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

// *********************************************
// INCLUDES
// *********************************************
// # Non-standard libraries:
// # Streaming http://arduiniana.org/libraries/streaming/
// # DHT11 http://playground.arduino.cc/Main/DHT11Lib
// # DS1307 RTC, Time & TimeAlarms http://arduino.cc/playground/Code/Time
// # OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html
// # DallasTemperature https://github.com/milesburton/Arduino-temp-Control-Library
// # EEPROMex http://playground.arduino.cc/Code/EEPROMex
// # UTouch http://www.henningkarlsen.com/electronics/library.php
// # UTFT custom version based on: http://arduinodev.com/arduino-sd-card-image-viewer-with-tft-shield/
// # ArduinoSerialCommand https://github.com/fsb054c/ArduinoSerialCommand

#include "Sensors.h"
#include "Sensor.h"
#include "SensorEC.h"
#include "SensorHumid.h"
#include "SensorLight.h"
#include "SensorPH.h"
#include "SensorTemp.h"
#include "SensorWater.h"
#include "Settings.h"
#include "RGBled.h"
#include "Buttons.h"
#include "GUI.h"
#include "Settings.h"
#include "Sensors.h"
#include "SerialInterface.h"
#include "Window.h"
#include "WinAlarms.h"
#include "WinControllerMenu.h"
#include "WinControllerMenuTwo.h"
#include "WinEcAlarms.h"
#include "WinEcCalib.h"
#include "WinLvlAlarms.h"
#include "WinLvlCalib.h"
#include "WinMainMenu.h"
#include "WinMainScreen.h"
#include "WinPhAlarms.h"
#include "WinEcCalib.h"
#include "WinPump.h"
#include "WinReservoir.h"
#include "WinSD.h"
#include "WinSensorCalib.h"
#include "WinSensorPolling.h"
#include "WinSystemMenu.h"
#include "WinTime.h"
#include "WinWater.h"
#include "WinWaterNight.h"
#include <avr/pgmspace.h>
#include <Streaming.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <DHT11.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Time.h>  
#include <TimeAlarms.h>
#include <UTFT.h>
#include <UTouch.h>
#include <EEPROMEx.h>
#include <SD.h>
#include <SerialCommand.h>
#include <MemoryFree.h>
#include <string.h>

const float versionNumber = 1.5;

// *********************************************
// TEXTS STORED IN FLASH MEMORY
// *********************************************
const char rtcResetTxt[] PROGMEM = "RTC time has been recently reset. Manual adjustment needed.";
const char rtcInitOkTxt[] PROGMEM = "RTC init OK.";
const char rtcInitFailTxt[] PROGMEM = "RTC init FAIL! < --";
const char sdInitOkTxt[] PROGMEM = "SD init OK.";
const char sdInitFailTxt[] PROGMEM = "SD init FAIL! < --";
const char waterTimedTxt[] PROGMEM = "Timed watering mode enabled.";
const char waterContinuousTxt[] PROGMEM = "Continuous watering mode enabled.";
const char nightStoppedTxt[] PROGMEM = "Watering stopped for the night.";
const char nightStartTxt[] PROGMEM = "Watering reactivated with daylight.";
const char sdLogOnTxt[] PROGMEM = "SD logging turned ON.";
const char sdLogOffTxt[] PROGMEM = "SD logging turned OFF.";
const char pollingUpdateTxt[] PROGMEM = "Sensor polling timer updated.";
const char sdLogOk[] PROGMEM = "Logged sensor data to SD Card.";
const char sdLogFail[] PROGMEM = "Error opening SD file, can't log sensor readings. < --";
const char sensorsReadTxt[] PROGMEM = "Sensors read, data updated.";
const char ecAdjTxt[] PROGMEM = "EC sensor readings adjusted for temperature.";
const char phAdjTxt[] PROGMEM = "pH sensor readings adjusted for temperature.";
const char alarmTxT[] PROGMEM = "Alarm triggered! System needs human intervention. < --";
const char alarmOffTxt[] PROGMEM = "Huertomato recovered from an alarming situation! < -- ";
const char noWaterTxt[] PROGMEM = "Huertomato will NOT water to prevent pump damage. < --";
const char pumpOnTxt[] PROGMEM = "Nutrient level topped. Watering is safe again. < --";
const char waterStartTxt[] PROGMEM = "Huertomato is watering plants.";
const char waterStopTxt[] PROGMEM = "Watering cycle ended.";


// *********************************************
// PINOUT ASSIGN
// *********************************************
// Pins that can't be used;
// 16 & 17 are Serial2 Tx,Rx used for pH circuit
// 18 & 19 are Serial1 Tx,Rx used for EC circuit
// 20 & 21 are I2C's SDA, SCL used for RTC
// 50, 51 & 52 are MISO, MOSI & SCK used for SD card
//RGB LED
const uint8_t redPin = 12;
const uint8_t greenPin = 11;
const uint8_t bluePin = 13;
//SENSORS
const uint8_t humidIn = A1;
const uint8_t lightIn = A2;
const uint8_t tempIn = A0;
const uint8_t waterEcho = 8;
const uint8_t waterTrigger = 9;
//ACTUATORS
const uint8_t buzzPin = 10;
const uint8_t waterPump = A9;
//LCD
const uint8_t lcdRS = 38;
const uint8_t lcdWR = 39;
const uint8_t lcdCS = 40;
const uint8_t lcdRST = 41;
const uint8_t lcdTCLK = 6;
const uint8_t lcdTCS = 5;
const uint8_t lcdTDIN = 4;
const uint8_t lcdTDOUT = 3;
const uint8_t lcdIRQ = 2;
//SD card select
const uint8_t SDCardSS = 53;

// *********************************************
// OBJECT DECLARATIONS
// *********************************************
RGBled led(redPin, greenPin, bluePin);
dht11 DHT11;
// Setup a oneWire instance to communicate with DS18B20 temp sensor
OneWire oneWire(tempIn);
DallasTemperature temperature(&oneWire);
//LCD & touch
UTFT LCD(ITDB32WD,lcdRS,lcdWR,lcdCS,lcdRST);
UTouch Touch(lcdTCLK,lcdTCS,lcdTDIN,lcdTDOUT,lcdIRQ);
//Huertomato internal data
Settings settings;
Sensors sensors(&settings);
//Human interfaces
SerialInterface ui; //&sensors,&settings are also used but from global var
GUI gui(&LCD,&Touch,&sensors,&settings);

//Stores timers ID's and status
struct alarm {
	AlarmID_t id;
	boolean enabled;	
};
//Init everything to 0
alarm sensorAlarm = {};
alarm waterAlarm = {};
alarm waterOffAlarm = {};
alarm sdAlarm = {};
alarm serialAlarm = {};
alarm pumpProtAlarm = {};

//True when system has beeping timers activated
boolean beeping = false;
//True if SD has already been initiated
boolean sdInit = false;

// *********************************************
// SETUP
// *********************************************
void setup() {  
	led.setOn();
	ui.init();
	gui.init();
	//Actuators
	pinMode(buzzPin, OUTPUT);
	pinMode(waterPump, OUTPUT);
	setupRTC();
	setupSD(); 
	setupAlarms();
	setupWaterModes();
	initMusic();
	Alarm.delay(10);
	sensors.fastUpdate();
	gui.start();
}

//Initiates system time from RTC
void setupRTC() {
	setSyncProvider(RTC.get);
	time_t t = now();
	int d = day(t);
	int mo = month(t);
	int y = year(t);
	int h = hour(t);
	int m = minute(t);
	if ((timeStatus() == timeSet) && (d == 1) && (mo == 1) && (y = 2000)) {
		//This prevents a bug when time resets and then loops 00:00 - 00:05
		sensors.setRTCtime(10,10,10,10,10,2010);
		ui.timeStamp(rtcResetTxt);
	} else if (timeStatus() == timeSet)
		ui.timeStamp(rtcInitOkTxt);
	if (timeStatus() != timeSet)
		ui.timeStamp(rtcInitFailTxt);
}

//Inits SD card and creates timer for SD logs
void setupSD() {
	if (settings.getSDactive()) {
		pinMode(SDCardSS, OUTPUT);
		if (SD.begin(SDCardSS) || sdInit) {
			sdInit = true;
			//Timer to log sensor data to SD Card
			startSDlogTimer();
			ui.timeStamp(sdInitOkTxt);
		} else 
			ui.timeStamp(sdInitFailTxt);
	}
}

//Initiates system alarms and timers
//timerOnce is used and then another timerOnce is configured inside the called functions
void setupAlarms() { 
	//Sensor polling and smoothing
	sensorAlarm.id = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
	sensorAlarm.enabled = true;
	//Every 10min we adjust pH & EC circuit readings to temperature
	/*if (settings.getReservoirModule()) {
		Alarm.timerOnce(0,10,0,adjustECtemp);
		Alarm.timerOnce(0,10,0,adjustPHtemp);
	}*/
}

//Sets watering timer or starts continuous water
void setupWaterModes() {
	//Not if pump protection activated
	if (settings.getPumpProtected())
		ui.timeStamp(noWaterTxt);
	//Not if stopped watering for the night
	else if (settings.getNightWateringStopped())
		ui.timeStamp(nightStoppedTxt);
	else {
		if (settings.getWaterTimed()) {
			startWaterTimer();
			updateNextWateringTime();
			ui.timeStamp(waterTimedTxt);
		} else {
			digitalWrite(waterPump,HIGH);
			settings.setWateringPlants(true);
			ui.timeStamp(waterContinuousTxt);
		}
	}
	
}

//Updates variables used for displaying next watering time
void updateNextWateringTime() {
	time_t t = now();
	uint8_t min = minute(t) + settings.getWaterMinute();
	uint8_t hou = hour(t) + settings.getWaterHour();
	//Adjust values for real time
	if (min >= 60) {
		min -= 60;
		hou++;
	}
	if (hou >= 24)
		hou -= 24; 
	//Write values to settings class
	settings.setNextWhour(hou);
	settings.setNextWminute(min);
}

//Plays Close Encounters of the Third Kind theme music
void initMusic() {
	if (settings.getSound()) {
		tone(buzzPin, 783.99);
		Alarm.delay(750);
		tone(buzzPin, 880.00);
		Alarm.delay(750);
		tone(buzzPin, 698.46);
		Alarm.delay(750);
		tone(buzzPin, 349.23);
		Alarm.delay(750);
		tone(buzzPin, 523.25);
		Alarm.delay(1000);
		noTone(buzzPin);
	}
}

// *********************************************
// LOOP
// *********************************************
void loop() {
	ui.processInput();
	gui.refresh();
	gui.processInput();
	
	//Check if led needs color change
	checkLed();
	//Check if pump protection toggled
	checkPump();
	//Trigger alarm if needed
	checkAlarms();
	//Check if night time has come and system change necessary
	checkNightTime();
	//Checks if settings have changed and system needs updating
	checkSettingsChanged();
	
	//Delays are needed for alarms to work
	Alarm.delay(10);
}

// *********************************************
// LOOP CHECKS
// *********************************************
//Checks system status and updates led color if needed
void checkLed() {
	if (settings.getLed()) {
		//Timed mode & watering
		if (settings.getWateringPlants() && settings.getWaterTimed())
			led.setColour(BLUE);
		//Alarm triggered
		else if (settings.getAlarmTriggered() || settings.getPumpProtected())
			led.setColour(RED);
		//Normal operation
		else
			led.setColour(GREEN);
	} else 
		led.setOff();
}

//Checks if pump protection has been activated - System will stop watering if it has!
//Also starts a timer for pump protection serial messages if enabled
//Internally toggles waterSettingsChanged when setPumpProtected() used
void checkPump() {
	//Alarm not already toggled but triggered
	if ((!settings.getPumpProtected()) 
		&& (settings.getReservoirModule()) && (settings.getPumpProtection())  
		&& (sensors.getWaterLevel() < settings.getPumpProtectionLvl())) {
			settings.setPumpProtected(true);
			if (settings.getSerialDebug())
				startSerialPumpProtTimer();
	//Alarm toggled but conditions changed	
	} else if (settings.getPumpProtected() 
		&& ((!settings.getReservoirModule()) || (!settings.getPumpProtection()) 
		|| (sensors.getWaterLevel() > settings.getPumpProtectionLvl()))) {
			settings.setPumpProtected(false);
			stopSerialPumpProtTimer();
			if (settings.getSerialDebug())
				ui.timeStamp(pumpOnTxt);
	}
}

//Checks if any alarm has been triggered and changes alarm setting if needed
//Also starts timer for alarm serial messages if option enabled
//and checks if buzzer has to beep to signal user
void checkAlarms() {
	if (settings.getReservoirModule()) {
		//Only activate if not already done and something off range
		if ((!settings.getAlarmTriggered()) 
			&& (sensors.ecOffRange() || sensors.phOffRange() || sensors.lvlOffRange())) {
				settings.setAlarmTriggered(true);
				if (settings.getSerialDebug()) {
					printAlarm();
					startSerialAlarmTimer();
				}
		//Theres an alarm triggered but everything is ok
		} else if ((settings.getAlarmTriggered()) 
			&& (!sensors.ecOffRange() && !sensors.phOffRange() && !sensors.lvlOffRange())) {		
				settings.setAlarmTriggered(false);
				stopSerialAlarmTimer();
				if (settings.getSerialDebug())
					ui.timeStamp(alarmOffTxt);
		}
		checkSoundAlarm();
	}
}

//Checks if sound alarm must be triggered
void checkSoundAlarm() {
	if (settings.getAlarmTriggered() || settings.getPumpProtected()) {
		//Sound alarm in main screen, when sound activated and not night watering stopped
		if (gui.isMainScreen() && settings.getSound() && !beeping &&
		!settings.getNightWateringStopped()) {
			beeping = true;
			beepOn();
		}
	}
}

//Checks for night-watering option and for day/night change. Updates watering if needed
//Internally toggles waterSettingsChanged when setNightWateringStopped() used
void checkNightTime() {
	//Not stopped for night and conditions for it to stop met
	if ((!settings.getNightWateringStopped()) 
		&& (!settings.getNightWatering()) && (sensors.getLight() < settings.getLightThreshold())) {
			settings.setNightWateringStopped(true);
	//Stopped for night but setting changed or day has come
	} else if ((settings.getNightWateringStopped()) 
		&& ((settings.getNightWatering()) || (sensors.getLight() > settings.getLightThreshold()))) {
			settings.setNightWateringStopped(false);		
	}
}

//These rely on settings class toggling changed flags when there is a change in settings
//Checks if some setting has been changed and updates accordingly
void checkSettingsChanged() {
	checkWater();
	checkSD();
	checkSensors();
	checkSerial();
}

//Checks if water settings have changed and resets watering cycles
void checkWater() {
	if (settings.waterSettingsChanged()) {
		//Clear alarms, stop water
		stopWaterTimer();
		stopWaterOffTimer();
		digitalWrite(waterPump, LOW);
		settings.setWateringPlants(false);
		//Reset system
		setupWaterModes();
	}
}

//Checks if SD settings have been changed and updates system
void checkSD() {
	if (settings.sdSettingsChanged()) {
		stopSDlogTimer();
		if (settings.getSDactive()) {
			setupSD();
			ui.timeStamp(sdLogOnTxt);
		} else
			ui.timeStamp(sdLogOffTxt);
	}
}

//Checks if sensor polling settings have changed and updates system
void checkSensors() {
	if (settings.sensorPollingChanged()) {
		Alarm.free(sensorAlarm.id);
		sensorAlarm.id = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
		sensorAlarm.enabled = true;
		ui.timeStamp(pollingUpdateTxt);
	}
}

//Checks if serial settings have been changed and updates system
void checkSerial() {
	if (settings.serialDebugChanged())
		(settings.getSerialDebug()) ? ui.init() : ui.end();
}

// *********************************************
// SD AND SERIAL LOG FUNCTIONS
// *********************************************
//Logs system data to SDCard
//File name will be: YYYMMDD.csv
//Format is: Date,Time,Temp,Humidity,Light,EC,PH,WaterLevel
void logSensorReadings() {
	time_t t = now();
	int d = day(t);
	int mo = month(t);
	int y = year(t);
	int h = hour(t);
	int m = minute(t);
	
	//Filename must be at MAX 8chars + "." + 3chars
	//We choose it to be YYYY+MM+DD.csv
	String fileName = "";
	fileName.reserve(12);
	fileName = (String)y + ((mo<10)?"0":"") + (String)mo + ((d<10)?"0":"") + (String)d + ".csv";
	char fileNameArray[fileName.length() + 1];
	fileName.toCharArray(fileNameArray, sizeof(fileNameArray));
	File sensorLog = SD.open(fileNameArray, FILE_WRITE);
	
	if (sensorLog) {
		sensorLog << ((d<10)?"0":"") << d << "-" << ((mo<10)?"0":"") << mo << "-" << y << ",";
		sensorLog << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m;
		sensorLog << "," << sensors.getTemp() << "," << sensors.getHumidity();
		sensorLog << "," << sensors.getLight() << "," << sensors.getEC();
		sensorLog << "," << sensors.getPH() << "," << sensors.getWaterLevel() << endl;
		sensorLog.close();
		//Inform through serial
		ui.timeStamp(sdLogOk);
	} else
		ui.timeStamp(sdLogFail);
}

//Timestamps to Serial that there's an alarm triggered
void printAlarm() {
	ui.timeStamp(alarmTxT);
}

//Timestamps to Serial if pump protection toggled
void printPump() {
	ui.timeStamp(noWaterTxt);
}

// *********************************************
// ROUTINES THAT SET UP A TIMERONCE ALARM
// *********************************************
//Updates sensor readings and sets next timer
void updateSensors() {
	sensors.update();
	gui.refresh();
	//ui.timeStamp(sensorsReadTxt);
	//Set next timer
	sensorAlarm.id = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
	sensorAlarm.enabled = true;
}

//Adjusts EC sensor readings to temperature and sets next timer
void adjustECtemp() {
	if (gui.isMainScreen()) {
		sensors.adjustECtemp();
		ui.timeStamp(ecAdjTxt);
	}
	//Set next timer
	Alarm.timerOnce(0,10,0,adjustECtemp);
}

//Adjusts pH sensor readings to temperature and sets next timer
void adjustPHtemp() {
	if (gui.isMainScreen()) {
		sensors.adjustPHtemp();
		ui.timeStamp(phAdjTxt);
	}
	//Set next timer
	Alarm.timerOnce(0,10,0,adjustPHtemp);
}

//These handle beeping when an alarm is triggered.
void beepOn() {
	const int onSecs = 1;
	tone(buzzPin,440.00);
	Alarm.timerOnce(0,0,onSecs,beepOff);
}

void beepOff() {
	const int offSecs = 2;
	noTone(buzzPin);
	if ((settings.getAlarmTriggered() || settings.getPumpProtected()) 
		&& settings.getSound() && gui.isMainScreen() && !settings.getNightWateringStopped())
			Alarm.timerOnce(0,0,offSecs,beepOn);
	else
		beeping = false;
}

// *********************************************
// TIMER'S MANAGERS
// *********************************************
//Timers for informing of an alarm through Serial
void startSerialAlarmTimer() {
	if (!serialAlarm.enabled) {
		serialAlarm.id = Alarm.timerRepeat(0,15,0,printAlarm);
		serialAlarm.enabled = true;
	}	
}

void stopSerialAlarmTimer() {
	if (serialAlarm.enabled) {
		Alarm.free(serialAlarm.id);
		serialAlarm.enabled = false;
	}
}

//Timers for informing of a pump protection state through Serial
void startSerialPumpProtTimer() {
	if (!pumpProtAlarm.enabled) {
		pumpProtAlarm.id = Alarm.timerRepeat(0,15,0,printPump);
		pumpProtAlarm.enabled = true;
	}
}

void stopSerialPumpProtTimer() {
	if (pumpProtAlarm.enabled) {
		Alarm.free(pumpProtAlarm.id);
		pumpProtAlarm.enabled = false;
	}
}

//Timers for logging data to SD
void startSDlogTimer() {
	if (!sdAlarm.enabled) {
		sdAlarm.id = Alarm.timerRepeat(settings.getSDhour(),settings.getSDminute(),0,logSensorReadings);
		sdAlarm.enabled = true;
	}
}

void stopSDlogTimer() {
	if (sdAlarm.enabled) {
		Alarm.free(sdAlarm.id);
		sdAlarm.enabled = false;
	}
}

//Timers for watering cycles
void startWaterTimer() {
	if (!waterAlarm.enabled) {
		waterAlarm.id = Alarm.timerRepeat(settings.getWaterHour(),settings.getWaterMinute(),0,startWatering);
		waterAlarm.enabled = true;
	}
}

void stopWaterTimer() {
	if (waterAlarm.enabled) {
		Alarm.free(waterAlarm.id);
		waterAlarm.enabled = false;
	}
}

void startWaterOffTimer() {
	if (!waterOffAlarm.enabled) {
		waterOffAlarm.id = Alarm.timerOnce(0,settings.getFloodMinute(),0,stopWatering);
		waterOffAlarm.enabled = true;
	}
}

void stopWaterOffTimer() {
	if (waterOffAlarm.enabled) {
		Alarm.free(waterOffAlarm.id);
		waterOffAlarm.enabled = false;
	}
}

// *********************************************
// TIMED WATERING ROUTINES
// *********************************************
void startWatering() {
	updateNextWateringTime();	
	//Only water if pump protection is off & not night 
	if (!settings.getPumpProtected() && !settings.getNightWateringStopped()) {
		digitalWrite(waterPump, HIGH);
		settings.setWateringPlants(true);
		ui.timeStamp(waterStartTxt);
		//Creates timer to stop watering
		stopWaterOffTimer();
		startWaterOffTimer();	
	}
}

//Stops watering pump and updates system status
void stopWatering() {
	stopWaterOffTimer();
	digitalWrite(waterPump, LOW);
	settings.setWateringPlants(false);
	ui.timeStamp(waterStopTxt);
}