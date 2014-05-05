// #############################################################################
// #
// # Name       : Huertomato
// # Version    : 1.2.6
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 04.05.2014
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
// INCLUDE
// *********************************************
// # Non-standard libraries:
// # Streaming http://arduiniana.org/libraries/streaming/
// # DHT11 http://playground.arduino.cc/Main/DHT11Lib
// Replace with http://playground.arduino.cc/Main/DHTLib
// # DS1307 RTC, Time & TimeAlarms http://arduino.cc/playground/Code/Time
// # OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html
// # DallasTemperature https://github.com/milesburton/Arduino-temp-Control-Library
// # EEPROMex http://playground.arduino.cc/Code/EEPROMex
// # UTouch & UTFT_Buttons http://www.henningkarlsen.com/electronics/library.php
// # UTFT custom version based on: http://arduinodev.com/arduino-sd-card-image-viewer-with-tft-shield/

#include "Sensors.h"
#include "Settings.h"
#include "RGBled.h"
#include "Buttons.h"
#include "GUI.h"
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
#include <UTFT_Buttons.h>
#include <EEPROMex.h>
#include <SD.h>
//TEMP from http://arduino.cc/playground/Code/AvailableMemory
#include <MemoryFree.h>


// *********************************************
// PINOUT ASSIGN
// *********************************************
// Pins that cant be used;
// 16 & 17 are Serial2 Tx,Rx used for EC circuit
// 18 & 19 are Serial1 Tx,Rx used for PH circuit
// 20 & 21 are IIC's SDA, SCL used for RTC
// 50, 51 & 52 are MISO, MOSI & SCK used for SD card
//RGB LED
const int redPin = 12;
const int greenPin = 11;
const int bluePin = 13;
//SENSORS
//A1 - Old: A13
const int humidIn = A1;
//const int humidIn = A13;
//A2 - Old: A15
const int lightIn = A2;
//const int lightIn = A15;
//A0 - Old: 42
const int tempIn = A0;
//const int tempIn = 42;
//D8 - Old: 44
const int waterEcho = 8;
//const int waterEcho = 44;
//D9 - Old: 45
const int waterTrigger = 9;
//const int waterTrigger = 45;
//ACTUATORS
//D10 - Old: 47
const int buzzPin = 10;
//const int buzzPin = 47;
//A9 - Old: 48
const int waterPump = A9;
//const int waterPump = 48;
//LCD
const int lcdRS = 38;
const int lcdWR = 39;
const int lcdCS = 40;
const int lcdRST = 41;
const int lcdTCLK = 6;
const int lcdTCS = 5;
const int lcdTDIN = 4;
const int lcdTDOUT = 3;
const int lcdIRQ = 2;
//SD card select
const int SDCardSS = 53;

// *********************************************
// OBJECT DECLARATIONS
// *********************************************
RGBled led(redPin, greenPin, bluePin);

dht11 DHT11;
// Setup a oneWire instance to communicate with DS18B20 temp sensor
OneWire oneWire(tempIn);
DallasTemperature temperature(&oneWire);

UTFT LCD(ITDB32WD,lcdRS,lcdWR,lcdCS,lcdRST);
UTouch Touch(lcdTCLK,lcdTCS,lcdTDIN,lcdTDOUT,lcdIRQ);

Settings settings;
Sensors sensors(&settings);

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

//True when system has beeping timers activated
boolean beeping;

//True if SD card has been initiated
boolean sdInitiated = false;

// *********************************************
// SETUP
// *********************************************
void setup() {  
	led.setOn();
	
	LCD.InitLCD();
	LCD.clrScr();
	LCD.fillScr(VGA_WHITE);
	Touch.InitTouch();
	Touch.setPrecision(PREC_MEDIUM);
	gui.drawSplashScreen();
	
	//Actuators
	pinMode(buzzPin, OUTPUT);
	pinMode(waterPump, OUTPUT);
	
	//First run
	//settings.setDefault();
	
	setupSerial();
	setupRTC();
	setupSD(); 
	
	initMusic();
	setupAlarms();
	setupWaterModes();

	 
	Alarm.delay(1000);
	LCD.fillScr(VGA_WHITE);
	gui.drawMainScreen();
}

//Initiates serial communication if needed
void setupSerial() {
	if (settings.getSerialDebug()) {
		Serial.begin(115200);
		Serial << endl << ".::[ Huertomato ]::." << endl;
		Serial << "By: Juan L. Perez Diez" << endl << endl;
	}
}

//Initiates system time from RTC
void setupRTC() {
	setSyncProvider(RTC.get); 
	if(timeStatus() != timeSet) {
		timestampToSerial("RTC init problem!!");
	} 
}

//Inits SD card and creates timer for SD logs
void setupSD() {
	if (settings.getSDactive()) {
		if (!sdInitiated) {
			pinMode(SDCardSS, OUTPUT);
			if (!SD.begin(SDCardSS)) {
				settings.setSDactive(false);
				timestampToSerial("SD init problem!! Make sure it's correctly inserted"); 		
			} else {
				timestampToSerial("SD init OK");
				sdInitiated = true;
			}
		}
		//Timer to log sensor data to SD Card
		sdAlarm.id = Alarm.timerOnce(settings.getSDhour(),settings.getSDminute(),0,logSensorReadings);
		sdAlarm.enabled = true;
	}
}

//Initiates system alarms and timers
//timerOnce is used and then another timerOnce is configured inside the called functions
void setupAlarms() { 
	//Sensor polling and smoothing
	sensorAlarm.id = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
	sensorAlarm.enabled = true;
	//Every min we adjust EC circuit readings to temperature
	if (settings.getReservoirModule())
		Alarm.timerOnce(0,1,0,adjustECtemp);		
	//Every 5secs we send sensor status to Serial if needed
	//if (settings.getSerialDebug()) 
		//Alarm.timerOnce(0,0,5,showStatsSerial);   
}

//Sets watering timer or starts continuous water
void setupWaterModes() {
    if (settings.getWaterTimed()) {
		digitalWrite(waterPump, LOW);
		settings.setWateringPlants(false);
		startWaterTimer();
		updateNextWateringTime();
		timestampToSerial("Timed watering mode enabled");
    } else {
		digitalWrite(waterPump,HIGH);	
		settings.setWateringPlants(true);
		timestampToSerial("Continuous watering mode enabled");
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
	gui.processTouch();
	
	//Serial << "Available memory: " << freeMemory() << " bytes"<< endl << endl;
	//Alarm reporting
	//Serial << "total: " << Alarm.count() << endl;
	//Serial << " waterAlarmID: " << waterAlarm.id << "|"<< waterAlarm.enabled << endl;
	//Serial << " waterOffAlarmID: " << waterOffAlarm.id << "|" << waterOffAlarm.enabled << endl;
	/*Serial << " sdAlarmID: " << sdAlarm.id << "|" << sdAlarm.enabled << endl;
	Serial << " sensorAlarmID: " << sensorAlarm.id << "|" << sensorAlarm.enabled << endl;
	for (int i = 0; i < Alarm.count(); i++) {
		Serial << "id: " << i;
		Serial << " time_t: " << Alarm.read(i);
		Serial << " type: " << Alarm.readType(i) << endl;
	}*/
	
	//Manage LED & Sound
	if (settings.getWateringPlants() && settings.getWaterTimed()) {
		led.setColour(BLUE);
	} else if (settings.getAlarmTriggered()) {
		led.setColour(RED);
		//Sound alarm in main screen only
		if (gui.getActScreen() == 0 && settings.getSound() && !beeping && 
			!settings.getNightWateringStopped()) {
			beeping = true;	
			beepOn();
		}
	} else
		led.setColour(GREEN);
		
	//Refresh screens if needed
    if (gui.getActScreen() == 0)
		gui.updateMainScreen();
	//Nutrient level calibration
	else if (gui.getActScreen() == 14)
		gui.updateWaterCalibration();
	//Night threshold calibration
	else if (gui.getActScreen() == 15)
		gui.updateLightCalibration();
	
	//Trigger alarm if needed
	checkAlarms();
	//Stop/start watering if day/night changed
	checkNightTime();
	//Checks if settings have changed and system needs updating
	checkSettingsChanged();

	//Delays are needed for alarms to work
	Alarm.delay(10);
}

//Checks if any alarm should be triggered and changes alarm setting if needed
void checkAlarms() {
	//Only activate if not already done
	if (settings.getReservoirModule()) {
		if ((!settings.getAlarmTriggered()) && ((sensors.getPH() < settings.getPHalarmDown()) 
			|| (sensors.getPH() > settings.getPHalarmUp())
			|| (sensors.getEC() < settings.getECalarmDown()) 
			|| (sensors.getEC() > settings.getECalarmUp())
			|| (sensors.getWaterLevel() < settings.getWaterAlarm()))) {
			
				settings.setAlarmTriggered(true);
			
		} else if ((settings.getAlarmTriggered()) && ((sensors.getPH() >= settings.getPHalarmDown()) 
			&& (sensors.getPH() <= settings.getPHalarmUp())
			&& (sensors.getEC() >= settings.getECalarmDown()) 
			&& (sensors.getEC() <= settings.getECalarmUp())
			&& (sensors.getWaterLevel() >= settings.getWaterAlarm()))) {
			
				settings.setAlarmTriggered(false);
		}
	}
}

//Checks for night-watering option and for day/night change. Updates watering if needed
//Before sleeping system will do a last water cycle
void checkNightTime() {
	//Only if night-watering is disabled
	if (!settings.getNightWatering()) {
		//Its night-time and watering not stopped already
		if ((sensors.getRawLight() < settings.getLightThreshold()) && !settings.getNightWateringStopped()) {
			stopWaterTimer();
			stopWaterOffTimer();
			//System in timed mode and not currently watering
			if (settings.getWaterTimed() && !settings.getWateringPlants()) {
				//We make a last watering cycle
				startWatering();
				timestampToSerial("a last time before stopping for night");
			//System in continuous mode	
			} else {
				digitalWrite(waterPump,LOW);
				settings.setWateringPlants(false);
				timestampToSerial("Watering stopped for the night");
			}
			settings.setNightWateringStopped(true);
			
		//Day-time and watering not reactivated already
		} else if ((sensors.getRawLight() >= settings.getLightThreshold()) && settings.getNightWateringStopped()) {
			settings.setNightWateringStopped(false);
			//Just in case. Prevents overflow when there are a lot of night/day triggers in short time
			stopWaterOffTimer();
			digitalWrite(waterPump, LOW);
			settings.setWateringPlants(false);
			setupWaterModes();
			timestampToSerial("Watering reactivated with daylight");
		}
	}
	//This handles changes in settings while watering stopped for night
	if (settings.getNightWatering() && settings.getNightWateringStopped()) {
		settings.setNightWateringStopped(false);
		stopWaterTimer();
		stopWaterOffTimer();
		digitalWrite(waterPump, LOW);
		settings.setWateringPlants(false);
		setupWaterModes();
	}
}

//Checks if some setting has been changed and updates accordingly
void checkSettingsChanged() {
	checkWater();
	checkSD();
	checkSensors();
	checkSerial();
}

//Checks if water settings have changed and updates system
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
		if (sdAlarm.enabled) {
			Alarm.free(sdAlarm.id);
			sdAlarm.enabled = false;
		}
		if (settings.getSDactive()) {
			setupSD();
			timestampToSerial("SD logging turned ON");
		} else
			timestampToSerial("SD logging turned OFF");
	}
}

//Checks if sensor polling settings have changed and updates system
void checkSensors() {
	if (settings.sensorPollingChanged()) {
		Alarm.free(sensorAlarm.id);
		sensorAlarm.id = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
		sensorAlarm.enabled = true;
		timestampToSerial("Sensor polling timer updated");
	}
}

//Checks if serial settings have been changed and updates system
void checkSerial() {
	if (settings.serialDebugChanged()) {
		if (settings.getSerialDebug()) {
			setupSerial();
		} else {
			timestampToSerial("Deactivating serial communications");
			Serial.end();
		}
	}
}

// *********************************************
// TEXT OUTPUTS
// *********************************************

//Writes "HH:MM:SS - <Text>" to serial console if serial debugging is on
void timestampToSerial(String str) {
	if (settings.getSerialDebug()) {
		time_t t = now();
		int h = hour(t);
		int m = minute(t);
		int s = second(t);
	
		Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s;
		Serial  << " - " << str << endl;
	}
}

//Write input string to file and endl
//Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << endl;
/*void writeLog(char* t) {
    int h = hour();
    int m = minute();
    int s = second();
    stateLog = SD.open(stateLogFile, FILE_WRITE); 
    stateLog << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s;
    stateLog << t << endl;
    //stateLog << time(now()) << s << endl;
    //twoD(hour(t)) + ":" + twoD(minute(t)) + ":" + twoD(second(t));
    stateLog.close();
}*/

//Log data to SDCard and set next timer
//File name is : YYYMMDD.txt
//Format is: Date,Time,Temp,Humidity,Light,EC,PH,WaterLevel
void logSensorReadings() {
	time_t t = now();
	int d = day(t);
	int mo = month(t);
	int y = year(t);
	int h = hour(t);
	int m = minute(t);
  
	//Filename must be at MAX 8chars + "." + 3chars
	//We choose it to be YYYY+MM+DD.txt
	String fileName = ""; 
	fileName.reserve(12);
	fileName = (String)y + (String)mo + (String)d + ".txt";
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
		timestampToSerial("Logged sensor data to SD Card.");
	} else {
		timestampToSerial("Error opening SD file, can't log sensor readings.");
	}
	//Set next timer
	if (settings.getSDactive()) {
		sdAlarm.id = Alarm.timerOnce(settings.getSDhour(),settings.getSDminute(),0,logSensorReadings);
		sdAlarm.enabled = true;
	}
}

//Sends sensor data through serial
//void showStatsSerial() { 
//  time_t t = now();
//  int d = day(t);
//  int mo = month(t);
//  int y = year(t);
//  int h = hour(t);
//  int m = minute(t);
//  int s = second(t);
//  //Serial << "Uptime: " << elapsed(initTime) << endl;
//  Serial << "Available memory: " << freeMemory() << " bytes"<< endl << endl;
//  Serial << "Date: " << ((d<10)?"0":"") << d << "-" << ((mo<10)?"0":"") << mo << "-" << y << endl;
//  Serial << "Time: " << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << endl;
//  Serial << "Temp: " << sensors.getTemp() << "C" << endl;
//  Serial << "Humidity: " << sensors.getHumidity() << "%" << endl;
//  Serial << "Light level: " << sensors.getLight() << "%" << endl;
//  Serial << "EC: " << sensors.getEC() << endl;
//  Serial << "pH: " << sensors.getPH() << endl;
//  Serial << "Water level: " << sensors.getWaterLevel() << "%" << endl << endl;
//}

// *********************************************
// OTHER
// *********************************************

//Updates sensor readings and sets next timer
void updateSensors() {
	sensors.update();
	timestampToSerial("Sensors read, data updated.");
	//Set next timer
	sensorAlarm.id = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
	sensorAlarm.enabled = true;
}

//Adjusts EC sensor readings to temperature and sets next timer
void adjustECtemp() {
	//Don't adjust if sensor is being calibrated
	if (gui.getActScreen() != 16) {
		sensors.adjustECtemp();
		timestampToSerial("EC sensor readings adjusted for temperature.");
	}
	//Set next timer
	Alarm.timerOnce(0,1,0,adjustECtemp);
}

//These handle beeping when an alarm is triggered. It warns in serial too
void beepOn() {
	const int onSecs = 1;
	timestampToSerial("Alarm triggered! System needs human intervention.");
	tone(buzzPin,440.00);
	Alarm.timerOnce(0,0,onSecs,beepOff);
}

void beepOff() {
	const int offSecs = 2;
	noTone(buzzPin);
	if (settings.getAlarmTriggered() && settings.getSound() && (gui.getActScreen() == 0) 
		&& !settings.getNightWateringStopped())
		Alarm.timerOnce(0,0,offSecs,beepOn);
	else
		beeping = false;
}

//Timer managers
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

//TIMED WATERING ROUTINES
void startWatering() {
	updateNextWateringTime();	
	//If theres enough water to activate pump
	if (sensors.getWaterLevel() >= settings.getPumpProtectionLvl()) {	
		digitalWrite(waterPump, HIGH);
		settings.setWateringPlants(true);
		led.setColour(BLUE);
		//Refresh main screen if needed
		if (gui.getActScreen() == 0)
			gui.updateMainScreen();
		timestampToSerial("Huertomato is watering plants < --");
		//Creates timer to stop watering
		stopWaterOffTimer();
		startWaterOffTimer();
	//Pump will get damaged - System will NOT water	
	} else
		timestampToSerial("Huertomato will NOT water to prevent pump damage < --");
}

//Stops watering pump and updates system status
void stopWatering() {
	stopWaterOffTimer();
	digitalWrite(waterPump, LOW);
	settings.setWateringPlants(false);
	timestampToSerial("Watering cycle ended < --");
}