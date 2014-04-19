// #############################################################################
// #
// # Name       : Huertomato
// # Version    : 1.2.3
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 08.04.2014
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

#include "Other.h"
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
//A9 - Old: 49
const int waterPump = A9;
//const int waterPump = 49;
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

//Stores ID of timers. If not present it is 0
AlarmID_t sensorAlarmID;
AlarmID_t waterAlarmID;
AlarmID_t sdAlarmID;

//True when system has beeping timers activated
boolean beeping;

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
	//TODO: Implement menu option
	//settings.setDefault();
	
	setupSerial();
	setupRTC();
	setupSD(); 
	
	setupAlarms();
	setupWaterModes();
	initMusic();
	 
	Alarm.delay(1000);
	LCD.fillScr(VGA_WHITE);
	gui.drawMainScreen();
}

//Initiates serial communication
void setupSerial() {
	Serial.begin(115200);
	Serial << endl << ".::[ Huertomato ]::." << endl;
	Serial << "By: Juan L. Perez Diez" << endl << endl;
	
	/*Serial1.begin(38400);
	Serial1 << "I/r";
	Serial1.print("I/r");
	Serial1.print('I');
	Serial1.print('/r');
	Serial1.flush();
	String info = "";
	info.reserve(30);
	//Read data from sensor
	while (Serial1.peek() != '\r') {
		char inchar = (char)Serial1.read();
		info += inchar;
	}
	//Discard <CR>
	Serial1.read();
	Serial << info << endl;*/
	
}

//Initiates system time from RTC
void setupRTC() {
	setSyncProvider(RTC.get); 
	if(timeStatus() != timeSet) {
		if (settings.getSerialDebug()) {
			writeSerialTimestamp();
			Serial << "RTC init problem!!" << endl;
		}
		//TODO: Warn in init screen LCD
	} 
}

//Inits SD card
void setupSD() {
	if (settings.getSDactive()) {
		pinMode(SDCardSS, OUTPUT);
		if (!SD.begin(SDCardSS)) {
			//If theres an error we switch SD off
			//settings.setSDactive(false);
			if (settings.getSerialDebug()) {
				writeSerialTimestamp();
				Serial << "SD init problem!! Make sure it's correctly inserted" << endl; 
			}		
		}
	//Timer to log sensor data to SD Card
	sdAlarmID = Alarm.timerOnce(settings.getSDhour(),settings.getSDminute(),0,logSensorReadings);
	}
}

//Initiates system alarms and timers
//timerOnce is used and then another timerOnce is configured inside the called functions
//This approach takes care of changes in settings mid sketch although of course
//these will not occur instantly but on next function call.
void setupAlarms() { 
	//Sensor polling and smoothing
	sensorAlarmID = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
	//Every min we adjust EC circuit readings to temperature
	if (settings.getReservoirModule())
		Alarm.timerOnce(0,1,0,adjustECtemp);		
	//Every 5secs we send sensor status to Serial if needed
	//if (activateSerial) 
	//Alarm.timerOnce(0,0,5, showStatsSerial);   
}

//Sets watering timer or starts continuous water
void setupWaterModes() {
    if (settings.getWaterTimed()) {
		digitalWrite(waterPump, LOW);
		settings.setWateringPlants(false);
		waterAlarmID = Alarm.timerOnce(settings.getWaterHour(),settings.getWaterMinute(),0,startWatering);
		updateNextWateringTime();
    } else {
		digitalWrite(waterPump,HIGH);	
		settings.setWateringPlants(true);
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
//TODO: Make prettier
void loop() {
	
	//Serial << "Available memory: " << freeMemory() << " bytes"<< endl << endl;
	//Alarm reporting
	/*Serial << "total: " << Alarm.count() << endl;
	Serial << " waterAlarmID: " << waterAlarmID << endl;
	Serial << " sdAlarmID: " << sdAlarmID << endl;
	Serial << " sensorAlarmID: " << sensorAlarmID << endl;
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
	else if (gui.getActScreen() == 17)
		gui.updateLightCalibration();
    
    gui.processTouch();
   
//  //TODO: Will warn when initiating as values = 0
//  //Initiate arrays to be lowerLimit < vars < upperLimit
//  //Or activate alarms when 1-2 minutes have passed 
	
	// ALARMS TRIGGERING
	//Checks if any alarm should be triggered
	if (settings.getReservoirModule()) {
		if ((sensors.getPH() < settings.getPHalarmDown()) || (sensors.getPH() > settings.getPHalarmUp()) 
			|| (sensors.getEC() < settings.getECalarmDown()) || (sensors.getEC() > settings.getECalarmUp()) 
			|| (sensors.getWaterLevel() < settings.getWaterAlarm())) { 
			  
				settings.setAlarmTriggered(true);		
				//TODO: Do not SPAM console!	
				if (settings.getSerialDebug()) {
					//writeSerialTimestamp();
					//Serial << "Alarm triggered! System needs human intervention." << endl;
				}

		} else if ((sensors.getPH() >= settings.getPHalarmDown()) && (sensors.getPH() <= settings.getPHalarmUp()) 
			&& (sensors.getEC() >= settings.getECalarmDown()) && (sensors.getEC() <= settings.getECalarmUp()) 
			&& (sensors.getWaterLevel() >= settings.getWaterAlarm())) {   
			
				settings.setAlarmTriggered(false);         
		}
	}
  
	// NIGHT-DAY CHECK
	//If watering has been stopped for the night and day has come or
	//same thing and night watering setting has been reactivated, we start water cycle again
	if (((settings.getNightWateringStopped()) && (sensors.getRawLight() > settings.getLightThreshold())) 
		|| ((settings.getNightWateringStopped()) && (settings.getNightWatering()))) {		
			
		settings.setNightWateringStopped(false);
		//Inform through serial
		if (settings.getSerialDebug()) {
			writeSerialTimestamp();
			Serial << "Watering restarted because of daytime or settings change." << endl;
		}
		//Restart watering timers if needed
		if (settings.getWaterTimed()) 
			startWatering(); 	
	}
	//TODO: Stop watering if in continuous mode?
	// WATER SETTINGS CHANGED CHECK
	if (settings.waterSettingsChanged()) {
		//Free previous water alarm if needed
		if (waterAlarmID != 0) {
			Alarm.free(waterAlarmID);
			waterAlarmID = 0;
		}
			
		//Activate continuous mode
		if (!settings.getWaterTimed()) {
			digitalWrite(waterPump, HIGH);
			settings.setWateringPlants(true);
		//Timed mode	
		} else {
			//Stop pump. Plan next water time
			digitalWrite(waterPump, LOW);
			settings.setWateringPlants(false);
			waterAlarmID = Alarm.timerOnce(settings.getWaterHour(),settings.getWaterMinute(),0,startWatering);	
			updateNextWateringTime();	
		}
	}
	
	// SD SETTINGS CHANGED CHECK
	if (settings.sdSettingsChanged()) {
		if (sdAlarmID != 0) {
			Alarm.free(sdAlarmID);
			sdAlarmID = 0;
		}
		if (settings.getSDactive()) {
			setupSD();
		}
	}
	
	// SENSOR POLLING INTERVAL CHANGED
	if (settings.sensorPollingChanged()) {
		Alarm.free(sensorAlarmID);
		sensorAlarmID = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
	}

	//Delays are needed for alarms to work
	Alarm.delay(10);
}

// *********************************************
// TEXT OUTPUTS
// *********************************************

//Writes "HH:MM:SS - " to serial console
void writeSerialTimestamp() {
	time_t t = now();
	int h = hour(t);
	int m = minute(t);
	int s = second(t);
	
	Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m;
	Serial << ":" << ((s<10)?"0":"") << s << " - ";
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
	int s = second(t);
  
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
		sensorLog << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s;
		sensorLog << "," << sensors.getTemp() << "," << sensors.getHumidity();
		sensorLog << "," << sensors.getLight() << "," << sensors.getEC();
		sensorLog << "," << sensors.getPH() << "," << sensors.getWaterLevel() << endl;
		sensorLog.close();
		//Inform through serial
		if (settings.getSerialDebug()) {
			writeSerialTimestamp();
			Serial << "Succesfully logged sensor data to SD Card." << endl;   
		}
	} else if (settings.getSerialDebug()) {
		writeSerialTimestamp();
		Serial << "Error opening SD file, can't log sensor readings." << endl;  
	}
	//Set next timer
	if (settings.getSDactive())
		Alarm.timerOnce(settings.getSDhour(),settings.getSDminute(),0,logSensorReadings);
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
	sensors.updateMain();
	//We update external module if its present and its not being calibrated
	if ((settings.getReservoirModule()) && (gui.getActScreen() != 14) 
		&& (gui.getActScreen() != 15) && (gui.getActScreen() != 16)) {
		sensors.updateReservoir();
	}
	if (settings.getSerialDebug()) {
		writeSerialTimestamp();
		Serial << "Sensors read, data updated." << endl;
	}
	//Set next timer
	sensorAlarmID = Alarm.timerOnce(0,0,settings.getSensorSecond(),updateSensors);
}

//Adjusts EC sensor readings to temperature and sets next timer
void adjustECtemp() {
	//Don't adjust if sensor is being calibrated
	if (gui.getActScreen() != 16) {
		sensors.adjustECtemp();
		if (settings.getSerialDebug()) {
			writeSerialTimestamp();
			Serial << "EC sensor readings adjusted for temperature." << endl;
		}
	}
	//Set next timer
	Alarm.timerOnce(0,1,0,adjustECtemp);
}

//These handle beeping when an alarm is triggered
void beepOn() {
	const int onSecs = 1;
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

//TIMED WATERING ROUTINES
void startWatering() {
	//If theres enough water to activate pump
	if (sensors.getWaterLevel() >= settings.getPumpProtectionLvl()) {
		settings.setWateringPlants(true);
		led.setColour(BLUE);
		//Refresh main screen if needed
		if (gui.getActScreen() == 0)
		gui.updateMainScreen();
			
		//Inform through serial
		if (settings.getSerialDebug()) {
			writeSerialTimestamp();
			Serial << "Huertomato is watering plants" << endl;
		}
			
		digitalWrite(waterPump, HIGH);
		//Creates timer to stop watering
		Alarm.timerOnce(0,settings.getFloodMinute(),0,stopWatering);
	
	//Pump will get damaged - System will NOT water	
	} else {
		writeSerialTimestamp();
		Serial << "Huertomato will NOT water to prevent pump damage" << endl;
	}
}

//If onlyDay is activated and night has come, system will water one last time and wont set more timers.
//Then it will check in main loop for day to come to call again this routine, reactivating timers.
void stopWatering() {
	digitalWrite(waterPump, LOW);
		
	//Set next watering alarm
	//If its night and night watering is disabled we don't set another timer
	//Main loop will be in charge of setting timer again
	if ((!settings.getNightWatering()) && (sensors.getRawLight() < settings.getLightThreshold())) {
		settings.setNightWateringStopped(true);
		if (settings.getSerialDebug()) {
			writeSerialTimestamp();
			Serial << "Watering stopped at nighttime." << endl;
		}
		} else {
		waterAlarmID = Alarm.timerOnce(settings.getWaterHour(),settings.getWaterMinute(),0,startWatering);
		updateNextWateringTime();
	}
	settings.setWateringPlants(false);
}