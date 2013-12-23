// #############################################################################
// #
// # Name       : Huertomato
// # Version    : 1.2.0
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 23.12.2013
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
// # UTFT & UTouch http://www.henningkarlsen.com/electronics/library.php

//#include "Other.h"
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
const int humidIn = A13;
const int lightIn = A15;
const int tempIn = 42;
const int waterEcho = 44;
const int waterTrigger = 45;
//ACTUATORS
const int buzzPin = 47;
const int flushValve = 48;
const int waterPump = 49;
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

UTFT LCD(ITDB32WD, lcdRS,lcdWR,lcdCS,lcdRST);
UTouch Touch(lcdTCLK,lcdTCS,lcdTDIN,lcdTDOUT,lcdIRQ);

Settings settings;
Sensors sensors;

GUI gui(&LCD,&Touch,&sensors,&settings);

// *********************************************
// SETUP
// *********************************************
void setup() {  
	led.setOn();
	//TODO:Here goes splash Screen 
	
	//Actuators
	pinMode(buzzPin, OUTPUT);
	pinMode(waterPump, OUTPUT);
	pinMode(flushValve, OUTPUT);

	Serial.begin(115200);
	Serial << endl << ".::[ Huertomato ]::." << endl;
	Serial << "By: Juan L. Perez Diez" << endl << endl;
  
	setupRTC();
	//setupSD();
	  
	LCD.InitLCD();
	LCD.clrScr();
	LCD.fillScr(VGA_WHITE);
	Touch.InitTouch();
	Touch.setPrecision(PREC_MEDIUM);
	
	setupAlarms();
	initMusic();
	 
	Alarm.delay(1000);
	gui.drawMainScreen();
}


//Initiates system time from RTC
void setupRTC() {
  setSyncProvider(RTC.get); 
  if(timeStatus() != timeSet) {
     if (settings.getSerialDebug()) 
       Serial.println("RTC Missing!!"); 
       //TODO: Warn in init screen??
  } 
}

//Inits SD card if needed
//void setupSD() {
//  pinMode(SDCardSS, OUTPUT);
//  if (activateSD) {
//    if (!SD.begin(SDCardSS)) {
//      if (activateSerial)
//        Serial << "SD missing!! Please insert one into the system." << endl; 
//        //TODO: Warn init screen
//    }
//  }
//}


////Initiates alarms and timers
void setupAlarms() { 
  //Every 5 secs we poll sensors and smooth the reading
  Alarm.timerRepeat(5, updateSensors);
  //Every 5mins we adjust EC circuit readings to temperature
  //Alarm.timerRepeat(0, 5, 0, adjustECtemp);
  //Every 10mins we log sensor data to SD if needed
  //if (activateSD)
    //Alarm.timerRepeat(0, 10, 0,logStats);
  //Every 5secs we send sensor status to Serial if needed
  //if (activateSerial) 
    //Alarm.timerRepeat(5, showStatsSerial);   
    
  //Timer for plant watering. Will be set again in waterPlants() in case timers change
  //Alarm.timerOnce(wHour, wMinute, 0, waterPlants);
  //updateNextWateringTime();
}
//
////Updates variables used for displaying next watering time
//void updateNextWateringTime() {
//  time_t t = now();
//  nextWhour = hour(t) + wHour;
//  nextWminute = minute(t) + wMinute;
//  if (nextWminute >= 60) {
//    nextWminute -= 60;
//    nextWhour += 1;
//  }
//  if (nextWhour >= 24) 
//    nextWhour -= 24;
//}
//
////Plays Close Encounters of the Third Kind theme music
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
	//Alarm
	if (settings.getAlarmTriggered()) {
		led.setColour(RED);
		//Sound alarm in main screen only
		if (gui.getActScreen() == 0 && settings.getSound()) 
			tone(buzzPin, 880.00, 250);
	} else
		led.setColour(GREEN);
    
    gui.processTouch();
	
	//Refresh main screen
    if (gui.getActScreen() == 0)
     gui.updateMainScreen();
   
//  //TODO: Will warn when initiating as values = 0
//  //Initiate arrays to be lowerLimit < vars < upperLimit
//  //Or activate alarms when X minutes have passed 
	
	//TODO: make funtion with ifs!
	//Checks if any alarm should be triggered
	if ((sensors.getPH() < settings.getPHalarmDown()) || (sensors.getPH() > settings.getPHalarmUp()) 
		|| (sensors.getEC() < settings.getECalarmDown()) || (sensors.getEC() > settings.getECalarmUp()) 
		|| (sensors.getWaterLevel() < settings.getWaterAlarm())) { 
			  
			settings.setAlarmTriggered(true);			
			if (settings.getSerialDebug())
				Serial << "Alarm triggered! System needs human intervention." << endl;

	} else if ((sensors.getPH() >= settings.getPHalarmDown()) && (sensors.getPH() <= settings.getPHalarmUp()) 
		&& (sensors.getEC() >= settings.getECalarmDown()) && (sensors.getEC() <= settings.getECalarmUp()) 
		&& (sensors.getWaterLevel() >= settings.getWaterAlarm())) {   
			
			settings.setAlarmTriggered(false);         
	}
//  
//  //If watering has been stopped for the night and day has come, we start water cycle again
//  if ((nightWateringStopped) && (sensors.getLight() > lightThreshold)) {
//    nightWateringStopped = false;
//    waterPlants(); 
//  }
//  //Delays are needed for alarms to work
	Alarm.delay(10);
}

// *********************************************
// TEXT OUTPUTS
// *********************************************
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

//Log data to SDCard
//File name is : YYYMMDD.txt
//Format is: Date,Time,Temp,Humidity,Light,EC,PH,WaterLevel
//void logStats() {
//  time_t t = now();
//  int d = day(t);
//  int mo = month(t);
//  int y = year(t);
//  int h = hour(t);
//  int m = minute(t);
//  int s = second(t);
//  
//  //Filename must be at MAX 8chars + "." + 3chars
//  //We choose it to be YYYY+MM+DD.txt
//  String fileName = ""; 
//  fileName.reserve(12);
//  fileName = (String)y + (String)mo + (String)d + ".txt";
//  char fileNameArray[fileName.length() + 1];
//  fileName.toCharArray(fileNameArray, sizeof(fileNameArray));
//  File sensorLog = SD.open(fileNameArray, FILE_WRITE); 
//   
//  if (sensorLog) {
//    sensorLog << ((d<10)?"0":"") << d << "-" << ((mo<10)?"0":"") << mo << "-" << y << ",";
//    sensorLog << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s;
//    sensorLog << "," << sensors.getTemp() << "," << sensors.getHumidity();
//    sensorLog << "," << sensors.getLight() << "," << sensors.getEC();
//    sensorLog << "," << sensors.getPH() << "," << sensors.getWaterLevel() << endl;
//    sensorLog.close();
//  } else {
//    if (activateSerial)
//      Serial << "Error opening SD file" << endl;   
//      //TODO: Warn in any other place?
//  }
//}

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

void updateSensors() {
	sensors.update();
}

//void adjustECtemp() {
//  sensors.adjustECtemp(); 
//}

//WATER EBB+FLOW ROUTINE
//Leaves flush valve opened in case it rains so plants wont flood but reservoir might be affected
//If onlyDay is activated and night has come, system will water one last time and wont set more timers.
//Then it will check in main loop for day to come to call again this routine, reactivating timers.
//void waterPlants() {
//  led.setColour(BLUE);
//  wateringPlants = true;
//
//  //Informs through LCD & serial if needed
//  //dispScreen = 0;
//  LCD.clrScr();
//  gui.drawMainScreen();
//  
//  if (activateSerial) {
//    Serial << "Watering plants" << endl << endl;
//  }
//
//  /*
//  //Flood circuit
//  //TODO: Cerrar valvula de salida si la hay
//  //Waits for the time set in floodM through watering settings
//  //Alarm.delay(floodM * 60000);
//  
//  //Flush circuit
//  //digitalWrite(flushValve, HIGH);
//  //Leaves flushValve On in case it rains
//  //Sets manual pump variables to off
//  inputValve = false;
//  outputValve = false;
//  */
//  //TODO: Remove - only for testing purposes
//  Alarm.delay(5000); 
//  
//  //Set next watering alarm and update LCD notification
//  if (onlyDay && (sensors.getLight() < lightThreshold))
//    nightWateringStopped = true;
//  else {
//    Alarm.timerOnce(wHour, wMinute, 0, waterPlants);
//    updateNextWateringTime();
//  }
//  wateringPlants = false;
//}

/*uint32_t toMs(time_t t) {
  return (uint32_t)((((hour(t) * 60) + minute(t)) * 60) + second(t)) * 1000;
}*/
