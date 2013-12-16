// #############################################################################
// #
// # Scriptname : sensors.h
// # Author     : Juan L. Pérez Diez <ender.vs.melkor at gmail>
// # Date       : 27.11.2013
// 
// # Description: Library for managing Huertomato's sensors
// # In charge of polling all hardware and smoothing values afterwards
// 
// # Version    : 1.0
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

#ifndef __SENSORS_H
#define __SENSORS_H

#include <Arduino.h>
#include "DHT11.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//class dht11;

//Ideally these variables should be given to the constructor but
//theres no way to declare a OneWire instance and initiate it afterwards that i know of

// Used for smoothing sensor data.  The higher the number,
// the more the readings will be smoothed, but the slower the variables will
// respond to the input.
const uint8_t numSamples = 10;
//These should be read from EEPROM and should be mutable    
const int maxWaterLevel = 16;
const int minWaterLevel = 50;
//Consts can be declared inside objects by using static const bla

//Pin numbers
// 16 & 17 are Serial2 Tx,Rx used for EC circuit
// 18 & 19 are Serial1 Tx,Rx used for PH circuit
const uint8_t humidIn = A13;
const uint8_t lightIn = A15;
const uint8_t tempIn = 42;
const uint8_t waterEcho = 44;
const uint8_t waterTrigger = 45;

static dht11 DHT11;
// Setup a oneWire instance to communicate with DS18B20 temp sensor
static OneWire oneWire(tempIn);
static DallasTemperature temperature(&oneWire);

//Class that handles sensor reading & smoothing
class Sensors {
public:
  //Constructor
  Sensors();

  //Getters
  float getTemp();
  uint8_t getLight();
  uint8_t getHumidity();
  uint32_t getEC();
  float getPH();
  uint8_t getWaterLevel();

  //Updates sample arrays with readings from sensors and smoothes data
  void update();
  //Adjusts EC sensor readings to temperature
  void adjustECtemp();

private:
  //Smoothes readings
  void smoothSensorReadings();

  //These poll hardware and return sensor info
  uint8_t light();
  float temp();
  uint8_t humidity();
  uint8_t waterLevel();
  float ph();
  uint32_t ec();

  //Smoothing counter
  uint8_t _iSample;
  //Contain sensor data pre-smoothing
  float _temps[numSamples];
  uint8_t _lights[numSamples]; 
  uint8_t _humidities[numSamples];
  uint32_t _ecs[numSamples];
  float _phs[numSamples];
  uint8_t _waterLevels[numSamples];

  //Contain sensor values post smoothing
  float _temp;
  uint8_t _light;
  uint8_t _humidity;
  uint32_t _ec;
  float _ph;
  uint8_t _waterLevel;

};

////Constructor
//Sensors::Sensors() {                 
//    pinMode(lightIn, INPUT);
//    pinMode(humidIn, INPUT);
//    pinMode(waterTrigger, OUTPUT);
//    pinMode(waterEcho, INPUT);
//    temperature.begin();
//    temperature.setResolution(11);
//    //Open communication with PH sensor
//    Serial1.begin(38400);
//    //Set it into reading on-demand mode
//    Serial1.print("E\r");
//    //EC sensor
//    Serial2.begin(38400);
//    Serial2.print("E\r");
//    //Set to continuous mode (needs 20-25 readings of 1000ms to stabilize reading)
//    Serial2.print("C\r");
//    
//    _iSample = 0;
//    //Initiate data arrays
//    for (int i = 0; i < numSamples; i++) {
//      _temps[i] = 0;
//      _lights[i] = 0;
//      _humidities[i] = 0;
//      _ecs[i] = 0;
//      _phs[i] = 0;
//      _waterLevels[i] = 0;
//    }
//}
//
////Getters
//float Sensors::getTemp() {
//  return _temp;
//}
//
//uint8_t Sensors::getLight() {
//  return _light;
//}
//
//uint8_t Sensors::getHumidity() {
//  return _humidity;
//}
//
//uint32_t Sensors::getEC() {
//  return _ec;
//}
//
//float Sensors::getPH() {
//  return _ph;
//}
//
//uint8_t Sensors::getWaterLevel() {
//  return _waterLevel;
//}
//
////Updates sample arrays with readings from sensors and performs smoothing
//void Sensors::update() {
//  _temps[_iSample] = temp();
//  _lights[_iSample] = light();
//  _humidities[_iSample] = humidity();
//  _ecs[_iSample] = ec();
//  _phs[_iSample] = ph();
//  _waterLevels[_iSample] = waterLevel();
//  
//  _iSample++;
//  if (_iSample >= numSamples) 
//    _iSample =0;
//    
//  smoothSensorReadings();
//}
//
////For each sensor data array calculates the average and stores it
//void Sensors::smoothSensorReadings() {
//  //Temp
//  float resF = 0;
//  for (int i = 0; i < numSamples; i++) { resF += _temps[i]; }
//  _temp = (float)(resF / numSamples);
//  //Light
//  uint32_t res = 0;
//  for (int i = 0; i < numSamples; i++) { res += _lights[i]; }
//  _light = (uint8_t)(res / numSamples);
//  //Humidity
//  res = 0;
//  for (int i = 0; i < numSamples; i++) { res += _humidities[i]; }
//  _humidity = (uint8_t)(res / numSamples);
//  //Electroconductivity
//  res = 0;
//  for (int i = 0; i < numSamples; i++) {  res += _ecs[i]; }
//  _ec = (uint32_t)(res / numSamples);
//  //PH
//  resF = 0;
//  for (int i = 0; i < numSamples; i++) { resF += _phs[i]; }
//  _ph = (float)(resF / numSamples);
//  //Water level
//  res = 0;
//  for (int i = 0; i < numSamples; i++) { res += _waterLevels[i]; }
//  _waterLevel = (uint8_t)(res / numSamples);  
//}
//
////Returns light level 0~100%
//uint8_t Sensors::light() {
//  int adc = analogRead(lightIn); 
//  return map(adc, 0, 1023, 0, 100); 
///* Code from Billie
//  lightADCReading = analogRead(lightSensor);
//  // Calculating the voltage of the ADC for light
//  lightInputVoltage = 5.0 * ((double)lightADCReading / 1024.0);
//  // Calculating the resistance of the photoresistor in the voltage divider
//  lightResistance = (10.0 * 5.0) / lightInputVoltage - 10.0;
//  // Calculating the intensity of light in lux       
//  currentLightInLux = 255.84 * pow(lightResistance, -10/9);*/
//}
//
////Returns temp in Celsius
//float Sensors::temp() {
//  temperature.requestTemperatures();
//  return temperature.getTempCByIndex(0);
//}
//
////Returns humidity percentage (-1 for error)
//uint8_t Sensors::humidity() {
//  if (DHT11.read(humidIn) == DHTLIB_OK) 
//    return DHT11.humidity; 
//  else
//    return 0; 
//}
//
////Returns water reservoir % level
//uint8_t Sensors::waterLevel() {
//  int duration, distance;
//  digitalWrite(waterTrigger, LOW);
//  delayMicroseconds(2);
//  digitalWrite(waterTrigger, HIGH);
//  delayMicroseconds(5);
//  digitalWrite(waterTrigger, LOW);
//  duration = pulseIn(waterEcho, HIGH);
//  distance = (duration / 29) / 2;
//  //Serial << "Distance: " << distance << endl;
//  distance = constrain(distance, maxWaterLevel, minWaterLevel);
//  return map(distance, maxWaterLevel, minWaterLevel, 100, 0); 
//}
//
////Returns PH level 
//float Sensors::ph() {   
//  //Temp-adjusted reading
//  //Convert temperature from float to char*
//  char tempArray[4];
//  dtostrf(_temp,4,2,tempArray);
//  String command = (String)tempArray + "\r";
//  //Serial << "Ph command: " << command << endl;
//  Serial1.print(command);
//  //Normal reading
//  //Serial1.print("R\r");
//  if (Serial1.available() > 0) {
//    String sensorString = ""; 
//    sensorString.reserve(30);
//    //Read data from sensor
//    while (Serial1.peek() != '\r') {
//      char inchar = (char)Serial1.read();                            
//       sensorString += inchar;           
//    }
//    //Discard <CR>
//    Serial1.read(); 
//    //Convert string to float and return
//    char sensorArray[sensorString.length() + 1];
//    sensorString.toCharArray(sensorArray, sizeof(sensorArray));
//    //Serial << "PH read: " << sensorArray << endl;
//    return atof(sensorArray);
//  } 
//  else { return 0.0; }
//}
//
////TODO: Error checking should be better implemented
////Returns EC in uSiemens
//uint32_t Sensors::ec() {
//  //As EC readings are continuous we can get two types of errors when reading from arduino
//  //We can have a string with more than 2 commas and we can have a number too large to be valid data 
//  if (Serial2.available() > 0) {
//    //Format is: uSiemens,PPM,Salinity<CR>
//    String sensorString = ""; 
//    sensorString.reserve(30);
//    //Read data from sensor
//    char inchar;
//    while (Serial2.peek() != '\r') {
//      inchar = (char)Serial2.read();    
//      sensorString += inchar;           
//    }
//    //Serial << "String given by EC: " << sensorString << endl;
//    //Discard <CR>
//    Serial2.read();
//    //Count number of ","
//    int comma = 0;
//    int index = 0;
//    while (sensorString.indexOf(',',index+1) != -1) {
//      index = sensorString.indexOf(',',index+1);
//      comma++;  
//    }
//    //If comma != 2 we have read buffer while sensor was sending data
//    if (comma == 2) {
//      //uSiemens
//      int firstComma = sensorString.indexOf(',');
//      String uSiemens = sensorString.substring(0,firstComma);
//      //PPM / TDS
//      //int secondComma = sensorString.indexOf(',',firstComma+1);
//      //String ppm = sensorString.substring(firstComma+1,secondComma);
//      //Salinity
//      //String salinity = sensorString.substring(secondComma+1,sensorString.length());
//      //Convert string to int and return
//      char uSiemensArray[uSiemens.length() + 1];
//      uSiemens.toCharArray(uSiemensArray, sizeof(uSiemensArray));      
//      //Serial << "EC in uS: " << atol(uSiemensArray) << endl;
//      uint32_t uS = atol(uSiemensArray);
//      //we dont accept readings that are clearly off charts
//      if (uS > 10000) {
//        //Serial << "Number too big: " << sensors.ec << endl;
//        return _ec;
//      }
//      //We received valid data
//      else
//        return atol(uSiemensArray);
//    } 
//    else { 
//      //Serial << "Too many commas: " << sensors.ec << endl;
//      return _ec; 
//    }
//  } 
//  //We have read buffer before sensor sent any data
//  else { 
//    //Serial << "Buffer empty: " << sensors.ec << endl;  
//    return _ec; 
//  }
//}
//
////Sends command to EC sensor to adjust readings to temperature
//void Sensors::adjustECtemp() {
//  //Convert temp from float to char*
//  //Serial << "Adjusting EC to temp: ";
//  if (_temp != 0) {
//    char tempArray[4];
//    dtostrf(_temp,4,2,tempArray);
//    String command = (String)tempArray + ",C\r";
//    //Serial << command << endl;
//    Serial2.print(command); 
//  }
//}

#endif

