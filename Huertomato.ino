// #############################################################################
// #
// # Name       : Huertomato
// # Version    : 1.0.1
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 27.11.2013
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
#include "sensors.h"
#include "rgbLED.h"
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
//SENSORS - These should be changed in sensors.h
//const int humidIn = A13;
//const int lightIn = A15;
//const int tempIn = 42;
//const int waterEcho = 44;
//const int waterTrigger = 45;
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
// TWEAKABLE PARAMETERS
// *********************************************
boolean activateSerial = false;
boolean activateSD = true;
//If light < threshold we assume its night time
const int lightThreshold = 10;

// *********************************************
// OBJECT DECLARATIONS AND GLOBAL VARIABLES
// *********************************************
rgbLED led(redPin, greenPin, bluePin);
Sensors sensors;

UTFT LCD(ITDB32WD, lcdRS,lcdWR,lcdCS,lcdRST);
UTouch Touch(lcdTCLK,lcdTCS,lcdTDIN,lcdTDOUT,lcdIRQ);

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

/**************************** SETTINGS MENU BUTTONS COORDS *****************************/
const int tAndD[] = {10, 29, 195, 59};       //time & date
const int wTime[] = {10, 69, 195, 99};       //watering times
const int phWarn[] = {10, 109, 195, 139};    //ph warnings
const int ecWarn[] = {10, 149, 195, 179};    //ec warnings
const int watWarn[] = {205, 29, 390, 59};    //water warning
const int mPump[] = {205, 69, 390, 99};      //manual pump control
const int about[] = {205, 109, 390, 139};    //about
//const int eighth[]= {205, 149, 390, 179};
/**************************** TIME AND DATE SCREEN BUTTONS COORDS ***************************/
const int houU[] = {140, 22, 165, 47};       //hour up
const int minU[] = {210, 22, 235, 47};       //min up
const int secU[] = {280, 22, 305, 47};       //sec up
const int houD[] = {140, 73, 165, 96};       //hour down
const int minD[] = {210, 73, 235, 96};       //min down
const int secD[] = {280, 73, 305, 96};      //sec down
const int dayU[] = {140, 112, 165, 137};     //day up
const int monU[] = {210, 112, 235, 137};     //month up
const int yeaU[] = {280, 112, 305, 137};     //year up
const int dayD[] = {140, 162, 165, 187};     //day down
const int monD[] = {210, 162, 235, 187};     //month down
const int yeaD[] = {280, 162, 305, 187};     //year down
/**************************** WATERING TIMERS BUTTONS COORDS ***************************/
const int wHourU[] = {140, 22, 165, 47};     //hour up
const int wHourD[] = {140, 73, 165, 96};     //hour down 
const int wMinU[] = {210, 22, 235, 47};      //minute up
const int wMinD[] = {210, 73, 235, 96};      //minute down
const int floodU[] = {90, 112, 115, 137};   //flooding min up
const int floodD[] = {90, 162, 115, 187};   //flooding min down
const int night[] = {340,140,365,165};       //night only button
/**************************** PH WARNINGS BUTTON COORDS ***************************/
const int phUpperU[] = {333, 48, 358, 73};    //upper section + 
const int phUpperD[] = {225, 48, 250, 73};    //upper section -
const int phLowerU[] = {333, 138, 358, 163};  //lower section +
const int phLowerD[] = {225, 138, 250, 163};  //lower section -
/**************************** EC WARNINGS BUTTON COORDS ***************************/
const int ecUpperU[] = {333, 48, 358, 73};    //same as above
const int ecUpperD[] = {225, 48, 250, 73};
const int ecLowerU[] = {333, 138, 358, 163};
const int ecLowerD[] = {225, 138, 250, 163};
/**************************** WATER WARNING BUTTON COORDS ***************************/
const int wattU[] = {225,96,250,121};
const int wattD[] = {333,96,358,121};
/**************************** MANUAL PUMPS CONTROL BUTTON COORDS ***************************/
const int manIN[] = {225, 35, 325, 85};      //water in toggle
const int manOUT[] = {225, 125, 325, 175};   //water out toggle
/***************************** MISCELLANEOUS BUTTONS COORDS *********************************/
const int back[] = {10, 205, 115, 230};      //BACK
const int save[] = {150, 205, 250, 230};     //SAVE
const int canc[] = {290, 205, 390, 230};     //CANCEL

uint8_t dispScreen = 0;                //0-Main Screen, 1-Options, 2-Clock Setup, 3-Watering Times, 
                                       //4-PH Warnings, 5-EC Warnings, 6-Water Warnings
                                       //7-Manual Pump Control, 8-About

//Time next watering will occurr
uint8_t nextWhour;
uint8_t nextWminute;
//Keeps the pump status when manually controling them
boolean inputValve = false;
boolean outputValve = false;
//Informs the system that watering timers are stopped for the night
boolean nightWateringStopped = false;
//Keeps track of other situations
boolean wateringPlants = false;
boolean alarmTriggered = false;

//Variables for all settings and their temporary for display and editing
uint8_t wHour;
uint8_t wHourTMP;
uint8_t wMinute;
uint8_t wMinuteTMP;
uint8_t floodM;
uint8_t floodMtmp;
boolean onlyDay;
boolean onlyDayTMP;
float phAlarmU;
float phAlarmUtmp;
float phAlarmD;
float phAlarmDtmp; 
uint32_t ecAlarmU;
uint32_t ecAlarmUtmp;
uint32_t ecAlarmD;
uint32_t ecAlarmDtmp;
uint8_t waterAlarm;
uint8_t waterAlarmTMP;

//Temp variables used when manually changing time
uint8_t hourTMP, minTMP, secTMP, dayTMP, monthTMP;
int yearTMP;

//EEPROM addresses for all settings
int addressWhour = EEPROM.getAddress(sizeof(byte));
int addressWminute = EEPROM.getAddress(sizeof(byte));
int addressFloodM = EEPROM.getAddress(sizeof(byte));
int addressOnlyDay = EEPROM.getAddress(sizeof(byte));
int addressPHalarmU = EEPROM.getAddress(sizeof(float));
int addressPHalarmD = EEPROM.getAddress(sizeof(float));
int addressECalarmU = EEPROM.getAddress(sizeof(long));
int addressECalarmD = EEPROM.getAddress(sizeof(long));
int addressWaterAlarm = EEPROM.getAddress(sizeof(byte));

// *********************************************
// SETUP
// *********************************************
void setup() {  
  led.setOn();

  //Actuators
  pinMode(buzzPin, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(flushValve, OUTPUT);

  if (activateSerial) {
    Serial.begin(115200);
    Serial << endl << ".::[ Huertomato ]::." << endl;
    Serial << "By: Juan L. Perez Diez" << endl << endl;
  }
  
  setupRTC();
  setupSD();
  
  LCD.InitLCD();
  LCD.clrScr();
  Touch.InitTouch();
  Touch.setPrecision(PREC_MEDIUM);
  
  readEEPROMvars();
  setupAlarms();
  initMusic();
  
  Alarm.delay(2500);
  drawMain();
}

//Initiates system time from RTC
void setupRTC() {
  setSyncProvider(RTC.get); 
  if(timeStatus() != timeSet) {
     if (activateSerial) 
       Serial.println("RTC Missing!!"); 
       //TODO: Warn in init screen??
  } 
}

//Inits SD card if needed
void setupSD() {
  pinMode(SDCardSS, OUTPUT);
  if (activateSD) {
    if (!SD.begin(SDCardSS)) {
      if (activateSerial)
        Serial << "SD missing!! Please insert one into the system." << endl; 
        //TODO: Warn init screen
    }
  }
}

//Reads settings from EEPROM non-volatile memory and loads temp vars
void readEEPROMvars() {
  wHour = EEPROM.readByte(addressWhour);
  wMinute = EEPROM.readByte(addressWminute);
  floodM = EEPROM.readByte(addressFloodM);
  onlyDay = EEPROM.readByte(addressOnlyDay);
  phAlarmU = EEPROM.readFloat(addressPHalarmU);
  phAlarmD = EEPROM.readFloat(addressPHalarmD);
  ecAlarmU = EEPROM.readLong(addressECalarmU);
  ecAlarmD = EEPROM.readLong(addressECalarmD);
  waterAlarm = EEPROM.readByte(addressWaterAlarm);
  
  wHourTMP = wHour;
  wMinuteTMP = wMinute;
  floodMtmp = floodM;
  onlyDayTMP = onlyDay;
  phAlarmUtmp = phAlarmU;
  phAlarmDtmp = phAlarmD; 
  ecAlarmUtmp = ecAlarmU;
  ecAlarmDtmp = ecAlarmD;
  waterAlarmTMP = waterAlarm;
}

//Initiates alarms and timers
void setupAlarms() { 
  //Every 5 secs we poll sensors and smooth the reading
  Alarm.timerRepeat(5, updateSensors);
  //Every 5mins we adjust EC circuit readings to temperature
  Alarm.timerRepeat(0, 5, 0, adjustECtemp);
  //Every 10mins we log sensor data to SD if needed
  if (activateSD)
    Alarm.timerRepeat(0, 10, 0,logStats);
  //Every 5secs we send sensor status to Serial if needed
  if (activateSerial) 
    Alarm.timerRepeat(5, showStatsSerial);   
    
  //Timer for plant watering. Will be set again in waterPlants() in case timers change
  Alarm.timerOnce(wHour, wMinute, 0, waterPlants);
  updateNextWateringTime();
}

//Updates variables used for displaying next watering time
void updateNextWateringTime() {
  time_t t = now();
  nextWhour = hour(t) + wHour;
  nextWminute = minute(t) + wMinute;
  if (nextWminute >= 60) {
    nextWminute -= 60;
    nextWhour += 1;
  }
  if (nextWhour >= 24) 
    nextWhour -= 24;
}

//Plays Close Encounters of the Third Kind theme music
void initMusic() {
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

// *********************************************
// LOOP
// *********************************************
void loop() {
  if (alarmTriggered) {
    led.setColor(RED);
    //Sound alarm in main screen only
    if (dispScreen == 0)
      tone(buzzPin, 880.00, 250);
  } else
    led.setColor(GREEN);
    
  processTouch();
  if (dispScreen == 0)
    updateMain();
  
  //TODO: Will warn when initiating as values = 0
  //Initiate arrays to be lowerLimit < vars < upperLimit
  //Or activate alarms when X minutes have passed 
  
  //Checks if any alarm should be triggered
  if ((sensors.getWaterLevel() < waterAlarm) || (sensors.getPH() < phAlarmD) || (sensors.getPH() > phAlarmU) ||
         (sensors.getEC() < ecAlarmD) || (sensors.getEC() > ecAlarmU)) {   
    alarmTriggered = true;
    if (activateSerial)
      Serial << "Alarm triggered! System needs human intervention." << endl;
  }
  else if ((sensors.getWaterLevel() >= waterAlarm) && (sensors.getPH() >= phAlarmD) && (sensors.getPH() <= phAlarmU) &&
         (sensors.getEC() >= ecAlarmD) && (sensors.getEC() <= ecAlarmU)) {   
    alarmTriggered = false;         
  }
  
  //If watering has been stopped for the night and day has come, we start water cycle again
  if ((nightWateringStopped) && (sensors.getLight() > lightThreshold)) {
    nightWateringStopped = false;
    waterPlants(); 
  }
  //Delays are needed for alarms to work
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
void logStats() {
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
  } else {
    if (activateSerial)
      Serial << "Error opening SD file" << endl;   
      //TODO: Warn in any other place?
  }
}

//Sends sensor data through serial
void showStatsSerial() { 
  time_t t = now();
  int d = day(t);
  int mo = month(t);
  int y = year(t);
  int h = hour(t);
  int m = minute(t);
  int s = second(t);
  //Serial << "Uptime: " << elapsed(initTime) << endl;
  Serial << "Available memory: " << freeMemory() << " bytes"<< endl << endl;
  Serial << "Date: " << ((d<10)?"0":"") << d << "-" << ((mo<10)?"0":"") << mo << "-" << y << endl;
  Serial << "Time: " << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s << endl;
  Serial << "Temp: " << sensors.getTemp() << "C" << endl;
  Serial << "Humidity: " << sensors.getHumidity() << "%" << endl;
  Serial << "Light level: " << sensors.getLight() << "%" << endl;
  Serial << "EC: " << sensors.getEC() << endl;
  Serial << "pH: " << sensors.getPH() << endl;
  Serial << "Water level: " << sensors.getWaterLevel() << "%" << endl << endl;
}

// *********************************************
// OTHER
// *********************************************

void updateSensors() {
  sensors.update();
}

void adjustECtemp() {
  sensors.adjustECtemp(); 
}
//WATER EBB+FLOW ROUTINE
//Leaves flush valve opened in case it rains so plants wont flood but reservoir might be affected
//If onlyDay is activated and night has come, system will water one last time and wont set more timers.
//Then it will check in main loop for day to come to call again this routine, reactivating timers.
void waterPlants() {
  led.setColor(BLUE);
  wateringPlants = true;

  //Informs through LCD & serial if needed
  dispScreen = 0;
  LCD.clrScr();
  drawMain();
  
  if (activateSerial) {
    Serial << "Watering plants" << endl << endl;
  }

  /*
  //Flood circuit
  //TODO: Cerrar valvula de salida si la hay
  //Waits for the time set in floodM through watering settings
  //Alarm.delay(floodM * 60000);
  
  //Flush circuit
  //digitalWrite(flushValve, HIGH);
  //Leaves flushValve On in case it rains
  //Sets manual pump variables to off
  inputValve = false;
  outputValve = false;
  */
  //TODO: Remove - only for testing purposes
  Alarm.delay(5000); 
  
  //Set next watering alarm and update LCD notification
  if (onlyDay && (sensors.getLight() < lightThreshold))
    nightWateringStopped = true;
  else {
    Alarm.timerOnce(wHour, wMinute, 0, waterPlants);
    updateNextWateringTime();
  }
  wateringPlants = false;
}

/*uint32_t toMs(time_t t) {
  return (uint32_t)((((hour(t) * 60) + minute(t)) * 60) + second(t)) * 1000;
}*/

// Dew point temp
// delta max = 0.6544 wrt dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
/*double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity/100);
  double Td = (b * temp) / (a - temp);
  return Td;
}*/

// *********************************************
// LCD DISPLAY AND TOUCH HANDLING
// *********************************************
//Draws window frame, headline and bottom divider
void printWindow(char* headline) {
  LCD.setFont(SmallFont); 
  //Header
  LCD.setColor(175,125,0);
  LCD.fillRect (0,0,399,14);
  LCD.setBackColor(175,125,0);
  LCD.setColor(255,255,255);
  LCD.print(headline, CENTER, 0);   
  //Bottom divider
  LCD.setColor(64, 64, 64);
  LCD.drawRect(0, 196, 399, 194);
}

//Prints a rounded button on given coords with given text
void printButton(char* text, int x1, int y1, int x2, int y2) {
  int stl = strlen(text);
  int fx, fy;
  
  LCD.setColor(0,115,0);
  LCD.fillRoundRect (x1, y1, x2, y2);
  LCD.setColor(255, 255, 255);
  LCD.drawRoundRect (x1, y1, x2, y2);
 
  LCD.setBackColor(0,115,0);
  fx = x1+(((x2 - x1)-(stl*8))/2);
  fy = y1+(((y2 - y1-1)-8)/2);
  LCD.print(text, fx, fy);
}

// Draws a red frame while a button is touched and prevents multi-presses
void waitForIt(int x1, int y1, int x2, int y2) 
{
  LCD.setColor(150, 0, 0);
  LCD.drawRoundRect (x1, y1, x2, y2);
  while (Touch.dataAvailable()) {
    Touch.read(); 
  }
  LCD.setColor(255, 255, 255);
  LCD.drawRoundRect (x1, y1, x2, y2);
}

//Draws main screen
//dispScreen = 0
void drawMain() {
  int maxY = 239;
  int maxX = 399;
  LCD.setFont(SmallFont);
  LCD.setColor(0,115,0);
  //Frame
  LCD.drawRect(0,0,maxX,maxY);
  //Top status box
  LCD.fillRect(0,0,maxX,14);
  LCD.setBackColor(0,115,0);
  LCD.setColor(255,255,255);
  LCD.print("Huertomato Hydroponics Controller v.1.0",CENTER,1);
  //Bottom status box
  LCD.setColor(0,115,0);
  LCD.fillRect(0,maxY-14,maxX,maxY);
  
  //Time
  time_t t = now();
  LCD.setColor(255,255,255);
  LCD.printNumI(hour(t),325,227,2,'0');
  LCD.print(":",341,227);
  LCD.printNumI(minute(t),349,227,2,'0');
  LCD.print(":",365,227);
  LCD.printNumI(second(t),373,227,2,'0');

  //Date
  LCD.printNumI(day(t),15,227,2,'0');
  LCD.print("/",31,227);
  LCD.printNumI(month(t),39,227,2,'0');
  LCD.print("/",55,227);
  LCD.printNumI(year(t),63,227,4);
  
  //Screen splitting lines |
  LCD.setColor(0,115,0);
  LCD.drawLine(maxX/2-1,12,maxX/2-1,maxY);
  LCD.drawLine(maxX/2+1,12,maxX/2+1,maxY/2-1);
  LCD.drawLine(maxX/2+1,maxY/2+1,maxX/2+1,maxY);
  //Split screen lines -
  LCD.drawLine(maxX/2+1,maxY/2-1,maxX,maxY/2-1);
  LCD.drawLine(maxX/2+1,maxY/2+1,maxX,maxY/2+1);
  
  //Top right frame
  drawNotificationFrame();
  //Bottom right frame
  drawAlarmsFrame();  
  //Left frame
  drawStatusFrame();
}

//TODO: Finish & make header
void updateMain() {
  LCD.setColor(255,255,255);
  LCD.setBackColor(0,115,0);
  //Time
  time_t t = now();
  LCD.printNumI(hour(t),325,227,2,'0');
  LCD.print(":",341,227);
  LCD.printNumI(minute(t),349,227,2,'0');
  LCD.print(":",365,227);
  LCD.printNumI(second(t),373,227,2,'0');

  //Date
  LCD.printNumI(day(t),15,227,2,'0');
  LCD.print("/",31,227);
  LCD.printNumI(month(t),39,227,2,'0');
  LCD.print("/",55,227);
  LCD.printNumI(year(t),63,227,4);
  
  //Top right frame
  drawNotificationFrame();
  //Bottom right frame
  drawAlarmsFrame();  
  //Left frame
  drawStatusFrame();
}

//Draws notifications frame (contained in main screen)
//Will change between watering plants, manual control or normal operation
void drawNotificationFrame() {
  LCD.setFont(BigFont);
  LCD.setBackColor(VGA_BLACK);  
  //Clears contents of frame
  LCD.setColor(VGA_BLACK);
  LCD.fillRect(210,20,390,90);
 
  if (wateringPlants) {
    LCD.setColor(0,0,150);
    LCD.print("System is",225,30);
    LCD.print("watering",235,75);
  }
  //Warn when in/out pumps are on
  else if ((inputValve) || (outputValve)) {
    LCD.setColor(150,0,0);
    if (inputValve) {
      LCD.print("Flooding ON",210,30);
    }
    if (outputValve) {
      LCD.print("Flushing ON",210,75);
    }
  //Normal operation - next watering time
  } else {
    LCD.setColor(0,115,0);
    //Warn if stopping watering at night
    if (nightWateringStopped) {
      LCD.print("No water",235,30);
      LCD.print("at night",235,75);
    } else {
      LCD.print("Next water:",215,30);
      LCD.printNumI(nextWhour,255,75,2,'0');
      LCD.print(":",287,75);
      LCD.printNumI(nextWminute,303,75,2,'0');
    }
  }
}

//Draws system status frame with sensor data(contained in main screen)
void drawStatusFrame() {
  LCD.setFont(SmallFont);
  LCD.setBackColor(VGA_BLACK);
  LCD.setColor(VGA_WHITE);
  LCD.print("- SYSTEM STATUS -",25,30);
  
  LCD.print("Temperature:",10,64);
  LCD.printNumF(sensors.getTemp(),2,114,64,'.',4);
  LCD.print("C",154,64);
  
  LCD.print("Humidity:",10,82);
  LCD.printNumI(sensors.getHumidity(),90,82,2);
  LCD.print("%",106,82);
  
  LCD.print("Light:",10,100);
  LCD.printNumI(sensors.getLight(),66,100,2);
  LCD.print("%",82,100);
  
  LCD.print("pH:",10,118);
  LCD.printNumF(sensors.getPH(),2,42,118,'.',4);
 
  LCD.print("EC:",10,136);
  LCD.printNumI(sensors.getEC(),42,136,4);
  LCD.print("uS",74,136);
  
  LCD.print("Solution level:",10,154);
  LCD.printNumI(sensors.getWaterLevel(),130,154,3);
  LCD.print("%",154,154);
}

//Draws alarms frame (contained in main screen)
//Will warn user of alarms triggered changing text to red color
void drawAlarmsFrame() {
  LCD.setFont(BigFont);
  //If theres an alarm, we color the text red, if not its green
  if (alarmTriggered) {
    LCD.setColor(150,0,0); 
    LCD.print("Alarm!",250,125);
  } else {
    LCD.setColor(0,115,0);
    LCD.print("Alarms",250,125); 
  }
  
  LCD.setFont(SmallFont);
  int listX = 210;
  
  (sensors.getPH() > phAlarmU) ? LCD.setColor(150,0,0) : LCD.setColor(0,115,0);
  LCD.print("* pH > ",listX,150);
  LCD.printNumF(phAlarmU,2,listX+56,150,'.',4);

  (sensors.getPH() < phAlarmD) ? LCD.setColor(150,0,0) : LCD.setColor(0,115,0);  
  LCD.print("* pH < ",listX,165);
  LCD.printNumF(phAlarmD,2,listX+56,165,'.',4);
  
  (sensors.getEC() > ecAlarmU) ? LCD.setColor(150,0,0) : LCD.setColor(0,115,0);
  LCD.print("* EC > ",listX,180);
  LCD.printNumI(ecAlarmU,listX+56,180,4);

  (sensors.getEC() < ecAlarmD) ? LCD.setColor(150,0,0) : LCD.setColor(0,115,0);  
  LCD.print("* EC < ",listX,195);
  LCD.printNumI(ecAlarmD,listX+56,195,4);
  
  (sensors.getWaterLevel() < waterAlarm) ? LCD.setColor(150,0,0) : LCD.setColor(0,115,0);
  LCD.print("* Solution < ",listX,210);
  LCD.printNumI(waterAlarm,listX+104,210,3);
  LCD.print("%",listX+128,210);
}

//Draws settings screen
//dispScreen = 1
void drawSettings() {
  printWindow("System Settings");  
  
  printButton("Time and Date", tAndD[0], tAndD[1], tAndD[2], tAndD[3]);
  printButton("Watering Timers", wTime[0], wTime[1], wTime[2], wTime[3]);
  printButton("pH Warnings", phWarn[0], phWarn[1], phWarn[2], phWarn[3]);
  printButton("EC Warnings", ecWarn[0], ecWarn[1], ecWarn[2], ecWarn[3]);    
  printButton("Water Level Warning", watWarn[0], watWarn[1], watWarn[2], watWarn[3]);
  printButton("Manual Pump Controls", mPump[0], mPump[1], mPump[2], mPump[3]);
  printButton("About", about[0], about[1], about[2], about[3]);
  //printButton("Eighth", eighth[0], eighth[1], eighth[2], eighth[3]);

  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Draws time adjustment screen
//dispScreen = 2
void drawTime() {
  //Get actual time
  time_t t = now();
  hourTMP = hour(t);
  minTMP = minute(t);
  secTMP = second(t);
  dayTMP = day(t);
  monthTMP = month(t);
  yearTMP = year(t);
  
  printWindow("Time and Date Settings");
  //Middle horizontal divider
  LCD.setColor(64, 64, 64);
  LCD.drawLine(0, 104, 399, 104);
  
  LCD.setFont(BigFont);
  printButton ("+", houU[0], houU[1], houU[2], houU[3]);     //hour up
  printButton ("+", minU[0], minU[1], minU[2], minU[3]);     //min up
  printButton ("-", houD[0], houD[1], houD[2], houD[3]);     //hour down
  printButton ("-", minD[0], minD[1], minD[2], minD[3]);     //min down
  printButton ("+", secU[0], secU[1], secU[2], secU[3]);     //sec up    
  printButton ("-", secD[0], secD[1], secD[2], secD[3]);    //sec down    
        
  printButton ("+", monU[0], monU[1], monU[2], monU[3]);     //month up
  printButton ("+", dayU[0], dayU[1], dayU[2], dayU[3]);     //day up
  printButton ("+", yeaU[0], yeaU[1], yeaU[2], yeaU[3]);     //year up
  printButton ("-", monD[0], monD[1], monD[2], monD[3]);     //month down
  printButton ("-", dayD[0], dayD[1], dayD[2], dayD[3]);     //day down
  printButton ("-", yeaD[0], yeaD[1], yeaD[2], yeaD[3]);     //year down
   
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  
  //TIME 
  int yTime = 52; 
  LCD.setFont(BigFont);  
  LCD.print("Time", 20, yTime);    
  LCD.setFont(SmallFont);
  LCD.print("(HH:MM:SS)", 13, yTime+18);  
  LCD.setFont(BigFont); 
  
  LCD.printNumI(hourTMP,houU[0]-3,yTime,2,'0');
  LCD.print(":",houU[0]+39,yTime);
  LCD.printNumI(minTMP,minU[0]-3,yTime,2,'0');
  LCD.print(":",minU[0]+39,yTime);
  LCD.printNumI(secTMP,secU[0]-3,yTime,2,'0');

  //DATE 
  int yDate = 142;
  LCD.print("Date", 20, yDate);    
  LCD.setFont(SmallFont);
  LCD.print("(DD/MM/YYYY)", 5, yDate+18);  
  LCD.setFont(BigFont); 

  LCD.printNumI(dayTMP, dayU[0]-3, yDate,2,'0');
  LCD.print("/", dayU[0]+39, yDate);
  LCD.printNumI(monthTMP, monU[0]-3, yDate,2,'0');
  LCD.print("/", monU[0]+39, yDate);
  LCD.printNumI(yearTMP, yeaU[0]-3, yDate,4);
  
  LCD.setFont(SmallFont);
  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("SAVE", save[0], save[1], save[2], save[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Refreshes window view when a + or - button is pressed
void updateTime() {
  LCD.setFont(BigFont);
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  //Time
  int yTime = 52; 
  LCD.printNumI(hourTMP,houU[0]-3,yTime,2,'0');
  LCD.printNumI(minTMP,minU[0]-3,yTime,2,'0');
  LCD.printNumI(secTMP,secU[0]-3,yTime,2,'0');
  //Date
  int yDate = 142;
  LCD.printNumI(dayTMP, dayU[0]-3, yDate,2,'0');
  LCD.printNumI(monthTMP, monU[0]-3, yDate,2,'0');
  LCD.printNumI(yearTMP, yeaU[0]-3, yDate,4);
  
  //Alarm.delay(25);
}

//Draws watering times screen
//dispScreen = 3
void drawWaterTimes() {
  printWindow("Watering Timers");
  //Middle horizontal divider
  LCD.setColor(64, 64, 64);
  LCD.drawLine(0, 104, 399, 104);
 
  LCD.setFont(BigFont);
  printButton ("+", wHourU[0], wHourU[1], wHourU[2], wHourU[3]);     //hour up
  printButton ("+", wMinU[0], wMinU[1], wMinU[2], wMinU[3]);     //min up
  printButton ("-", wHourD[0], wHourD[1], wHourD[2], wHourD[3]);     //hour down
  printButton ("-", wMinD[0], wMinD[1], wMinD[2], wMinD[3]);     //min down   
        
  printButton ("+", floodU[0], floodU[1], floodU[2], floodU[3]);     //min up
  printButton ("-", floodD[0], floodD[1], floodD[2], floodD[3]);     //min down
   
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  LCD.setFont(BigFont);  
  
  LCD.print("Every", 20, 52); 
  LCD.print("hours", 270, 52);
    
  LCD.printNumI(wHourTMP,wHourU[0]-3,52,2,'0');
  LCD.print(":",wHourU[0]+39,52);
  LCD.printNumI(wMinuteTMP,wMinU[0]-3,52,2,'0');

  LCD.print("for", 20, 142);  
  LCD.print("m.",140,142);
  LCD.printNumI(floodMtmp, floodU[0]-3, 142, 2,'0');
  
  LCD.print("Day only",190,142);
  if (onlyDayTMP)
    printButton("X",night[0],night[1],night[2],night[3]);
  else {
    LCD.setColor(64,64,64);
    LCD.fillRoundRect(night[0],night[1],night[2],night[3]);
    LCD.setColor(255, 255, 255);
    LCD.drawRoundRect(night[0],night[1],night[2],night[3]);
  }
  
  LCD.setFont(SmallFont);
  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("SAVE", save[0], save[1], save[2], save[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Refreshes window view when a + or - button is pressed
void updateWaterTimes() {
  LCD.setFont(BigFont);
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  //Every...
  LCD.printNumI(wHourTMP,wHourU[0]-3,52,2,'0');
  LCD.printNumI(wMinuteTMP,wMinU[0]-3,52,2,'0');
  //For...
  LCD.printNumI(floodMtmp, floodU[0]-3, 142, 2,'0');
  //Day only
  if (onlyDayTMP)
    printButton("X",night[0],night[1],night[2],night[3]);
  else {
    LCD.setColor(64,64,64);
    LCD.fillRoundRect(night[0],night[1],night[2],night[3]);
    LCD.setColor(255, 255, 255);
    LCD.drawRoundRect(night[0],night[1],night[2],night[3]);
  }
}

//Draws PH warnings screen
//dispScreen = 4
void drawPHWarns() {
  printWindow("pH Warnings");
  //Middle horizontal divider
  LCD.setColor(64, 64, 64);
  LCD.drawLine(0, 104, 399, 104);
  
  LCD.setFont(BigFont);  
  LCD.setColor(VGA_WHITE);
  LCD.setBackColor(VGA_BLACK);
  LCD.print("Upper Limit:", 20, 52);    
  LCD.print("Lower Limit:", 20, 142);   
  
  //Upper controls
  printButton("-",phUpperD[0],phUpperD[1],phUpperD[2],phUpperD[3]);
  printButton("+",phUpperU[0],phUpperU[1],phUpperU[2],phUpperU[3]);  
  LCD.setBackColor(VGA_BLACK);
  LCD.printNumF(phAlarmUtmp,2,260,52,'.',4);
  
  //Lower controls
  printButton("-",phLowerD[0],phLowerD[1],phLowerD[2],phLowerD[3]);
  printButton("+",phLowerU[0],phLowerU[1],phLowerU[2],phLowerU[3]);  
  LCD.setBackColor(VGA_BLACK);
  LCD.printNumF(phAlarmDtmp,2,260,142,'.',4);
  
  LCD.setFont(SmallFont);
  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("SAVE", save[0], save[1], save[2], save[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Refreshes window view when a + or - button is pressed
void updatePHWarns() {
  LCD.setFont(BigFont);
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  //Upper limit
  LCD.printNumF(phAlarmUtmp,2,260,52,'.',4);
  //Lower limit
  LCD.printNumF(phAlarmDtmp,2,260,142,'.',4);
}

//Draws EC warnings screen
//dispScreen = 5
void drawECWarns() { 
  printWindow("EC Warnings");
  //Middle horizontal divider
  LCD.setColor(64, 64, 64);
  LCD.drawLine(0, 104, 399, 104);
  
  LCD.setFont(BigFont);  
  LCD.setColor(VGA_WHITE);
  LCD.setBackColor(VGA_BLACK);
  LCD.print("Upper Limit:", 20, 52);    
  LCD.print("Lower Limit:", 20, 142);   
  
  //Upper controls
  printButton("-",ecUpperD[0],ecUpperD[1],ecUpperD[2],ecUpperD[3]);
  printButton("+",ecUpperU[0],ecUpperU[1],ecUpperU[2],ecUpperU[3]); 
  LCD.setBackColor(VGA_BLACK);
  LCD.printNumI(ecAlarmUtmp,260,52,4);
  
  //Lower controls
  printButton("-",ecLowerD[0],ecLowerD[1],ecLowerD[2],ecLowerD[3]);
  printButton("+",ecLowerU[0],ecLowerU[1],ecLowerU[2],ecLowerU[3]);
  LCD.setBackColor(VGA_BLACK);
  LCD.printNumI(ecAlarmDtmp,260,142,4);
  
  LCD.setFont(SmallFont);
  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("SAVE", save[0], save[1], save[2], save[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Refreshes window view when a + or - button is pressed
void updateECWarns() {
  LCD.setFont(BigFont);
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  //Upper limit
  LCD.printNumI(ecAlarmUtmp,260,52,4);
  //Lower limit
  LCD.printNumI(ecAlarmDtmp,260,142,4);
}

//Draws water warning screen
//dispScreen = 6
void drawWatWarns() {
  printWindow("Water Level Warning");
  LCD.setFont(BigFont);  
  LCD.setColor(VGA_WHITE);
  LCD.setBackColor(VGA_BLACK);  
  LCD.print("Lower Limit:", 20, 100);   
  
  printButton("-",wattD[0],wattD[1],wattD[2],wattD[3]);
  printButton("+",wattU[0],wattU[1],wattU[2],wattU[3]); 
  LCD.setBackColor(VGA_BLACK);
  LCD.printNumI(waterAlarmTMP,263,100,3);
  LCD.print("%",308,100);
  
  LCD.setFont(SmallFont);
  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("SAVE", save[0], save[1], save[2], save[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Refreshes window view when a + or - button is pressed
void updateWatWarns() {
  LCD.setFont(BigFont);
  LCD.setColor(VGA_WHITE);  
  LCD.setBackColor(VGA_BLACK);
  LCD.printNumI(waterAlarmTMP,263,100,3);
}

//Draws manual pump controls screen
//dispScreen = 7
void drawManualPump() {
  printWindow("Manual Pump Controls");
  //Middle horizontal divider
  LCD.setColor(64, 64, 64);
  LCD.drawLine(0, 104, 399, 104);
  
  LCD.setFont(BigFont);  
  LCD.setColor(VGA_WHITE);
  LCD.setBackColor(VGA_BLACK);
  LCD.print("Water In:", 50, 52);    
  LCD.print("Water Out:", 50, 142);   
  
  //Displays state according to global variables
  if (inputValve)
    printButton("ON  ",manIN[0],manIN[1],manIN[2],manIN[3]);
  else {
    LCD.setColor(64,64,64);
    LCD.fillRoundRect(manIN[0],manIN[1],manIN[2],manIN[3]);
    LCD.setColor(255, 255, 255);
    LCD.drawRoundRect(manIN[0],manIN[1],manIN[2],manIN[3]);    
  }  
  if (outputValve)
    printButton("ON  ",manOUT[0],manOUT[1],manOUT[2],manOUT[3]);
  else {
    LCD.setColor(64,64,64);
    LCD.fillRoundRect(manOUT[0],manOUT[1],manOUT[2],manOUT[3]);
    LCD.setColor(255, 255, 255);
    LCD.drawRoundRect(manOUT[0],manOUT[1],manOUT[2],manOUT[3]);
  }

  LCD.setFont(SmallFont);
  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);
}

//Draws about screen
//dispScreen = 8
void drawAbout() { 
  printWindow("Huertomato Hydroponics Controller v.1.0");

  LCD.setBackColor(VGA_BLACK);
  LCD.setColor(0,255,0);
  LCD.print("Release Date: August 2013", CENTER, 20);
  LCD.print("Written by Juan L. Perez Diez", CENTER, 32);
  
  LCD.print("GUI Look & Feel based on", CENTER, 52);
  LCD.print("Jarduino by Jamie Jardin", CENTER, 64);
  LCD.print("http://code.google.com/p/", CENTER, 78);
  LCD.print("/jarduino-aquarium-controller/", CENTER, 90);
  
  LCD.print("Thanks to Tom de Bie's Hydroponic Controller", CENTER,110);
  LCD.print("https://github.com/BillieBricks/", CENTER,122);
  
  LCD.print("Special Thanks: El Guetto,", CENTER, 146); 
  LCD.print("all the World Tinkerer Community,", CENTER, 158);
  LCD.print("my Family, and my dear Girlfriend", CENTER, 170);

  printButton("<< BACK", back[0], back[1], back[2], back[3]);
  printButton("CANCEL", canc[0], canc[1], canc[2], canc[3]);  
} 

//Revert back temp vars to the value they had before
//Used when cancel or back buttons are pressed
void resetTempVars() {
  if (dispScreen == 3) {
    wHourTMP = wHour;
    wMinuteTMP = wMinute;
    floodMtmp = floodM;
    onlyDayTMP = onlyDay;
       
  } else if (dispScreen == 4) {
     phAlarmUtmp = phAlarmU;
     phAlarmDtmp = phAlarmD;
     
   } else if (dispScreen == 5) {
     ecAlarmUtmp = ecAlarmU;
     ecAlarmDtmp = ecAlarmD;   
     
   } else if (dispScreen == 6) {
     waterAlarmTMP = waterAlarm;
   }
}

//Save temp vars to global vars and store in EEPROM
//Used when a save button is pressed
void saveTempVars() {
   if (dispScreen == 2) {
     tmElements_t t;
     t.Hour = hourTMP;
     t.Minute = minTMP;
     t.Second = secTMP;
     t.Day = dayTMP;
     t.Month = monthTMP;
     //year argument is offset from 1970
     t.Year = yearTMP - 1970;
     time_t time = makeTime(t);
     setTime(time);
     RTC.set(time);
     
   } else if (dispScreen == 3) {     
      wHour = wHourTMP;
      wMinute = wMinuteTMP;
      floodM = floodMtmp;
      onlyDay = onlyDayTMP;
      //Store Watering timers in EEPROM
      EEPROM.updateByte(addressWhour, wHour);
      EEPROM.updateByte(addressWminute, wMinute);
      EEPROM.updateByte(addressFloodM, floodM);
      EEPROM.updateByte(addressOnlyDay, onlyDay);
      
   } else if (dispScreen == 4) {
     phAlarmU = phAlarmUtmp;
     phAlarmD = phAlarmDtmp;
     //Store PH warnings in EEPROM
     EEPROM.updateFloat(addressPHalarmU, phAlarmU);
     EEPROM.updateFloat(addressPHalarmD, phAlarmD);
     
   } else if (dispScreen == 5) {
     ecAlarmU = ecAlarmUtmp;
     ecAlarmD = ecAlarmDtmp;
     //Store EC warnings in EEPROM
     EEPROM.updateLong(addressECalarmU, ecAlarmU);
     EEPROM.updateLong(addressECalarmD, ecAlarmD);
  
   } else if (dispScreen == 6) {
     waterAlarm = waterAlarmTMP;
     //Store water warning threshold in EEPROM
     EEPROM.updateByte(addressWaterAlarm, waterAlarm);
   }
}

//Processes pending touch actions
//dispScreen == 0-Main Screen, 1-Options, 2-Clock Setup, 
//3-Watering Times, 4-PH Warnings, 5-EC Warnings,
//6-Water Warnings, 7-Manual Pump Control, 8-About
void processTouch() {
 if (Touch.dataAvailable()) {
   Touch.read();
   int x = Touch.getX();
   int y = Touch.getY();
   //--------------------- CANCEL BUTTON ------------------------------------
   if ((x >= canc[0]) && (x <= canc[2]) && (y >= canc[1]) && (y <= canc[3]) && (dispScreen != 0)) {
     waitForIt(canc[0], canc[1], canc[2], canc[3]);
     resetTempVars();
     dispScreen = 0;
     LCD.clrScr();
     drawMain();     
   //--------------------- BACK BUTTON ------------------------------------
   } else if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3]) && (dispScreen != 0)) {
     waitForIt(back[0], back[1], back[2], back[3]);
     resetTempVars();
     dispScreen = 1;
     LCD.clrScr();
     drawSettings();     
   //--------------------- SAVE BUTTON ------------------------------------
   } else if ((x>=save[0]) && (x<=save[2]) && (y>=save[1]) && (y<=save[3]) && (dispScreen != 0)) {
     waitForIt(save[0], save[1], save[2], save[3]);
     saveTempVars();
     //Warn the user changing save button
     LCD.setFont(BigFont);
     LCD.setColor(150,0,0);
     LCD.setBackColor(0,115,0);
     LCD.print("SAVED", save[0]+12, save[1]+5);
     
   //--------------------- OTHER BUTTONS ------------------------------------  
   } else {
     switch(dispScreen) {
       //--------------------- MAIN SCREEN ----------------------
       case 0:     
         dispScreen = 1;
         LCD.clrScr();
         drawSettings();
         break;
       //--------------------- OPTIONS --------------------------  
       case 1:     
         processTouchOptions(x,y);
         break;
       //--------------------- TIME & DATE ----------------------
       case 2:     
         processTouchTimeAndDate(x,y);
         break;
       //------------------- WATERING TIMES ---------------------
       case 3:     
         processTouchWateringTimes(x,y);
         break;
       //--------------------- PH WARNINGS ----------------------
       case 4:     
         processTouchPHwarnings(x,y);
         break;
       //--------------------- EC WARNINGS ----------------------
       case 5:     
         processTouchECwarnings(x,y);
         break;
       //-------------------- WATER WARNING --------------------
       case 6:      
         processTouchWaterWarning(x,y);
         break;
       //--------------------- MANUAL PUMPS ---------------------
       case 7:     
         processTouchManualPumps(x,y);
         break;
       //------------------------- ABOUT ------------------------
       case 8:     
       default:
         break;
     }
   }
 }
}

//Process touch in options screen
void processTouchOptions(int x, int y) {
  //First column
   if ((x >= tAndD[0]) && (x <= tAndD[2])) {
     //Pressed Time and Date
     if ((y >= tAndD[1]) && (y <= tAndD[3])) {
       waitForIt(tAndD[0], tAndD[1], tAndD[2], tAndD[3]);
       dispScreen = 2;
       LCD.clrScr();
       drawTime();
     }
     //Pressed Watering Times
     else if ((y >= wTime[1]) && (y <= wTime[3])) {
       waitForIt(wTime[0], wTime[1], wTime[2], wTime[3]);
       dispScreen = 3;
       LCD.clrScr();
       drawWaterTimes();
     }
     //Pressed PH Warnings
     else if ((y >= phWarn[1]) && (y <= phWarn[3])) {
       waitForIt(phWarn[0], phWarn[1], phWarn[2], phWarn[3]);
       dispScreen = 4;
       LCD.clrScr();
       drawPHWarns();
     }
     //Pressed EC Warnings
     else if ((y >= ecWarn[1]) && (y <= ecWarn[3])) {
       waitForIt(ecWarn[0], ecWarn[1], ecWarn[2], ecWarn[3]);
       dispScreen = 5;
       LCD.clrScr();
       drawECWarns();
     }          
   } 
   //Second column
   else if ((x >= watWarn[0]) && (x <= watWarn[2])) {
     //Pressed Water Level Warnings
     if ((y >= watWarn[1]) && (y <= watWarn[3])) {
       waitForIt(watWarn[0], watWarn[1], watWarn[2], watWarn[3]);
       dispScreen = 6;
       LCD.clrScr();
       drawWatWarns();
     }
     //Pressed Manual Pump Controls
     else if ((y >= mPump[1]) && (y <= mPump[3])) {
       waitForIt(mPump[0], mPump[1], mPump[2], mPump[3]);
       dispScreen = 7;
       LCD.clrScr();
       drawManualPump();
     }
     //Pressed About
     else if ((y >= about[1]) && (y <= about[3])) {
       waitForIt(about[0], about[1], about[2], about[3]);
       dispScreen = 8;
       LCD.clrScr();
       drawAbout();
     }
   }
}

//Process touch in time & date settings screen
void processTouchTimeAndDate(int x, int y) {
  //Pressed plus sign -> hour, min or sec
   if ((y >= houU[1]) && (y <= houU[3])) {
     //+ hour
     if ((x >= houU[0]) && (x <= houU[2])) {
       //waitForIt(houU[0], houU[1], houU[2], houU[3]);
       hourTMP++;
       (hourTMP > 23) ? hourTMP=0 : 0;
       updateTime();
     }
     //+ minute
     else if ((x >= minU[0]) && (x <= minU[2])) {
       //waitForIt(minU[0], minU[1], minU[2], minU[3]);
       minTMP++;
       (minTMP > 59) ? minTMP=0 : 0;
       updateTime();
     }
     //+ secs
     else if ((x >= secU[0]) && (x <= secU[2])) {
       //waitForIt(secU[0], secU[1], secU[2], secU[3]);
       secTMP++;
       (secTMP > 59) ? secTMP=0 : 0;
       updateTime();
     }
   }
   //Pressed minus sign -> hour, min or sec
   else if ((y >= houD[1]) && (y <= houD[3])) {
     //- hour
     if ((x >= houD[0]) && (x <= houD[2])) {
       //waitForIt(houD[0], houD[1], houD[2], houD[3]);  
       //We cant just -- and compare with <0 as it overflows to 255
       (hourTMP <= 0) ? hourTMP=23 : hourTMP--;
       updateTime();
     }
     //- minute
     else if ((x >= minD[0]) && (x <= minD[2])) {
       //waitForIt(minD[0], minD[1], minD[2], minD[3]);
       (minTMP <= 0) ? minTMP=59 : minTMP--;
       updateTime();
     }
     //- secs
     else if ((x >= secD[0]) && (x <= secD[2])) {
       //waitForIt(secD[0], secD[1], secD[2], secD[3]);
       (secTMP <= 0) ? secTMP=59 : secTMP--;
       updateTime();
     }
   }
   //Pressed plus sign -> day, month or year
   else if ((y >= dayU[1]) && (y <= dayU[3])) {
     //+ day
     if ((x >= dayU[0]) && (x <= dayU[2])) {
       //waitForIt(dayU[0], dayU[1], dayU[2], dayU[3]);
       dayTMP++;
       (dayTMP > 31) ? dayTMP=1 : 0;
       updateTime();
     }
     //+ month
     else if ((x >= monU[0]) && (x <= monU[2])) {
       //waitForIt(monU[0], monU[1], monU[2], monU[3]);
       monthTMP++;
       (monthTMP > 12) ? monthTMP=1 : 0;
       updateTime();
     }
     //+ year
     else if ((x >= yeaU[0]) && (x <= yeaU[2])) {
       //waitForIt(yeaU[0], yeaU[1], yeaU[2], yeaU[3]);
       yearTMP++;
       (yearTMP > 2100) ? yearTMP=1970 : 0;
       updateTime();
     }
   }
   //Pressed minus sign -> day, month or year
   else if ((y >= dayD[1]) && (y <= dayD[3])) {
     //- day
     if ((x >= dayD[0]) && (x <= dayD[2])) {
       //waitForIt(dayD[0], dayD[1], dayD[2], dayD[3]);
       dayTMP--;
       (dayTMP < 1) ? dayTMP=31 : 0;
       updateTime();
     }
     //- month
     else if ((x >= monD[0]) && (x <= monD[2])) {
       //waitForIt(monD[0], monD[1], monD[2], monD[3]);
       monthTMP--;
       (monthTMP < 1) ? monthTMP=12 : 0;
       updateTime();
     }
     //- year
     else if ((x >= yeaD[0]) && (x <= yeaD[2])) {
       //waitForIt(yeaD[0], yeaD[1], yeaD[2], yeaD[3]);
       yearTMP--;
       (yearTMP < 1970) ? yearTMP=2100 : 0;
       updateTime();
     }
   }
}

//Process touch in watering timers settings screen
void processTouchWateringTimes(int x, int y) {
  //watering hour
   if ((x >= wHourU[0]) && (x <= wHourU[2])) {
     //+ sign
     if ((y >= wHourU[1]) && (y <= wHourU[3])) {
       //waitForIt(wHourU[0], wHourU[1], wHourU[2], wHourU[3]);
       wHourTMP++;
       (wHourTMP > 23) ? wHourTMP=0 : 0;
       updateWaterTimes();
     }
     //- sign
     else if ((y >= wHourD[1]) && (y <= wHourD[3])) {
       //waitForIt(wHourD[0], wHourD[1], wHourD[2], wHourD[3]);
       (wHourTMP <= 0) ? wHourTMP=23 : wHourTMP--;
       updateWaterTimes();
     }             
   } 
   //watering minute
   else if ((x >= wMinU[0]) && (x <= wMinU[2])) {
     //+ sign
     if ((y >= wMinU[1]) && (y <= wMinU[3])) {
       //waitForIt(wMinU[0], wMinU[1], wMinU[2], wMinU[3]);
       wMinuteTMP++;
       (wMinuteTMP > 59) ? wMinuteTMP=0 : 0;
       updateWaterTimes();
     }
     //- sign
     else if ((y >= wMinD[1]) && (y <= wMinD[3])) {
       //waitForIt(wMinD[0], wMinD[1], wMinD[2], wMinD[3]);
       (wMinuteTMP <= 0) ? wMinuteTMP=59 : wMinuteTMP--;
       updateWaterTimes();
     }     
   }
   //flooding minute
   else if ((x >= floodU[0]) && (x <= floodU[2])) {
     //+ sign
     if ((y >= floodU[1]) && (y <= floodU[3])) {
       //waitForIt(floodU[0], floodU[1], floodU[2], floodU[3]);
       floodMtmp++;
       (floodMtmp > 59) ? floodMtmp=0 : 0;
       updateWaterTimes();
     }
     //- sign
     else if ((y >= floodD[1]) && (y <= floodD[3])) {
       //waitForIt(floodD[0], floodD[1], floodD[2], floodD[3]);
       (floodMtmp <= 0) ? floodMtmp=59 : floodMtmp--;
       updateWaterTimes();
     }
   }
   //night only button
   else if ((x >= night[0]) && (x <= night[2]) && (y >= night[1]) && (y <= night[3])) {
     waitForIt(night[0], night[1], night[2], night[3]);  
     onlyDayTMP = !onlyDayTMP;
     updateWaterTimes();     
   }  
}

//Process touch in ph warnings settings screen
void processTouchPHwarnings(int x, int y) {
  //Pressing a plus button
   if ((x >= phUpperU[0]) && (x <= phUpperU[2])) {    
     //Pressed upper plus sign
     if ((y >= phUpperU[1]) && (y <= phUpperU[3])) {
       //waitForIt(phUpperU[0], phUpperU[1], phUpperU[2], phUpperU[3]);
       phAlarmUtmp += 0.05;
       (phAlarmUtmp > 14) ? phAlarmUtmp=0 : 0;
       updatePHWarns();
     }
     //Pressed lower plus sign
     else if ((y >= phLowerU[1]) && (y <= phLowerU[3])) {
       //waitForIt(phLowerU[0], phLowerU[1], phLowerU[2], phLowerU[3]);
       phAlarmDtmp += 0.05;
       (phAlarmDtmp > 14) ? phAlarmDtmp=0 : 0;
       updatePHWarns();
     }
   } 
   //Pressing a minus button
   else if ((x >= phUpperD[0]) && (x <= phUpperD[2])) { 
     //Pressed upper minus sign
     if ((y >= phUpperD[1]) && (y <= phUpperD[3])) {
       //waitForIt(phUpperD[0], phUpperD[1], phUpperD[2], phUpperD[3]);
       phAlarmUtmp -= 0.05;
       (phAlarmUtmp < 0) ? phAlarmUtmp=14 : 0;
       updatePHWarns();
     }
     //Pressed lower minus sign
     else if ((y >= phLowerD[1]) && (y <= phLowerD[3])) {
       //waitForIt(phLowerD[0], phLowerD[1], phLowerD[2], phLowerD[3]);
       phAlarmDtmp -= 0.05;
       (phAlarmDtmp < 0) ? phAlarmDtmp=14 : 0;
       updatePHWarns();
     }
   }
}

//Process touch in ec warnings settings screen
void processTouchECwarnings(int x, int y) {
  //Pressing a plus button
   if ((x >= ecUpperU[0]) && (x <= ecUpperU[2])) {    
     //Pressed upper plus sign
     if ((y >= ecUpperU[1]) && (y <= ecUpperU[3])) {
       //waitForIt(ecUpperU[0], ecUpperU[1], ecUpperU[2], ecUpperU[3]);
       ecAlarmUtmp += 10;
       (ecAlarmUtmp > 9999) ? ecAlarmUtmp=1300 : 0;
       updateECWarns();
     }
     //Pressed lower plus sign
     else if ((y >= ecLowerU[1]) && (y <= ecLowerU[3])) {
       //waitForIt(ecLowerU[0], ecLowerU[1], ecLowerU[2], ecLowerU[3]);
       ecAlarmDtmp += 10;
       (ecAlarmDtmp > 9999) ? ecAlarmDtmp=1300 : 0;
       updateECWarns();
     }
   } 
   //Pressing a minus button
   else if ((x >= ecUpperD[0]) && (x <= ecUpperD[2])) { 
     //Pressed upper minus sign
     if ((y >= ecUpperD[1]) && (y <= ecUpperD[3])) {
       //waitForIt(ecUpperD[0], ecUpperD[1], ecUpperD[2], ecUpperD[3]);
       ecAlarmUtmp -= 10;
       (ecAlarmUtmp < 1300) ? ecAlarmUtmp=9999 : 0;
       updateECWarns();
     }
     //Pressed lower minus sign
     else if ((y >= ecLowerD[1]) && (y <= ecLowerD[3])) {
       //waitForIt(ecLowerD[0], ecLowerD[1], ecLowerD[2], ecLowerD[3]);
       ecAlarmDtmp -= 10;
       (ecAlarmDtmp < 1300) ? ecAlarmDtmp=9999 : 0;
       updateECWarns();
     }
   }  
}

//Process touch in water level warning settings screen
void processTouchWaterWarning(int x, int y) {
  if ((y >= wattU[1]) && (y <= wattU[3])) {
     //Plus sign
     if ((x >= wattU[0]) && (x <= wattU[2])) {
       //waitForIt(wattU[0], wattU[1], wattU[2], wattU[3]);
       waterAlarmTMP += 1;
       (waterAlarmTMP > 100) ? waterAlarmTMP=0 : 0;
       updateWatWarns();
     } 
     //Minus sign
     else if ((x >= wattD[0]) && (x <= wattD[2])) {
       //waitForIt(wattD[0], wattD[1], wattD[2], wattD[3]);
       (waterAlarmTMP <= 0) ? waterAlarmTMP=100 : waterAlarmTMP--;
       updateWatWarns();
     }
   }  
}

//Process touch in manual pump control screen
void processTouchManualPumps(int x, int y) {
  if ((x >= manIN[0]) && (x <= manIN[2])) {
     //Toggle input valve
     if ((y >= manIN[1]) && (y <= manIN[3])) {
         waitForIt(manIN[0], manIN[1], manIN[2], manIN[3]);
         toggleInputValve();
     }
     //Toggle output valve
     else if ((y >= manOUT[1]) && (y <= manOUT[3])) {
         waitForIt(manOUT[0], manOUT[1], manOUT[2], manOUT[3]);
         toggleOutputValve();
     }
   }
}

//Toggles input of water into circuit
//Used for manual control
void toggleInputValve() {
  inputValve = !inputValve;
  LCD.setFont(BigFont);
  if (inputValve) {
    printButton("ON  ",manIN[0],manIN[1],manIN[2],manIN[3]);
    digitalWrite(waterPump, HIGH);
  } else {
    LCD.setColor(64,64,64);
    LCD.fillRoundRect(manIN[0],manIN[1],manIN[2],manIN[3]);
    LCD.setColor(255, 255, 255);
    LCD.drawRoundRect(manIN[0],manIN[1],manIN[2],manIN[3]);
    digitalWrite(waterPump, LOW);
  }
  LCD.setFont(SmallFont);
}

//Toggles output of water from circuit
//Used for manual control
void toggleOutputValve() {
  outputValve = !outputValve;
  LCD.setFont(BigFont);
  if (outputValve) {
    printButton("ON  ",manOUT[0],manOUT[1],manOUT[2],manOUT[3]);
    digitalWrite(flushValve, HIGH);
  } else {
    LCD.setColor(64,64,64);
    LCD.fillRoundRect(manOUT[0],manOUT[1],manOUT[2],manOUT[3]);
    LCD.setColor(255, 255, 255);
    LCD.drawRoundRect(manOUT[0],manOUT[1],manOUT[2],manOUT[3]);
    digitalWrite(flushValve, LOW);
  }
  LCD.setFont(SmallFont);
}
