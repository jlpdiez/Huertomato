#include "Settings.h"

Settings::Settings() {
//  _lightThreshold = 10;
  
  //Status variables - Not read from EEPROM
  _nextWhour = 0;
  _nextWminute = 0;
  _manualPump = false;
  _nightWateringStopped = false;
  _manualPump = false;
  _wateringPlants = false;
  _alarmTriggered = false;
  
  //System Settings
  _waterTimed = true;
  _waterHour = 2 ;
  _waterMinute = 30;
  _floodMinute = 15;
  _phAlarmUp = 7.50;
  _phAlarmDown = 6.25;
  _ecAlarmUp = 3200;
  _ecAlarmDown = 1200;
  _waterAlarm = 33;
  _nightWatering = true;
  _manualPump = false;
  
  //Controller Settings
  _sensorMinute = 0;
  _sensorSecond = 5;  
  _sdActive = true;
  _sdHour = 0;
  _sdMinute = 5;
  _sound = true;
  _serialDebug = true;;
  
//  _adressWaterTimed = EEPROM.getAddress(sizeof(byte));
//  _adressWaterHour = EEPROM.getAddress(sizeof(byte));
//  _adresswaterMinute = EEPROM.getAddress(sizeof(byte));
//  _adressFloodMinute = EEPROM.getAddress(sizeof(byte));
//  _adressPHalarmUp = EEPROM.getAddress(sizeof(float));
//  _adressPHalarmDown = EEPROM.getAddress(sizeof(float));
//  _adressECalarmUp = EEPROM.getAddress(sizeof(long));
//  _adressECalarmDown = EEPROM.getAddress(sizeof(long));
//  _adressWaterAlarm = EEPROM.getAddress(sizeof(byte));
//  _adresssSerialDebug = EEPROM.getAddress(sizeof(byte));
//  _adressActivateSD = EEPROM.getAddress(sizeof(byte));
//  _adressNightWatering = EEPROM.getAddress(sizeof(byte));
//  
//  _adressSensorMinute = EEPROM.getAddress(sizeof(byte));
//  _adressSensorSecond = EEPROM.getAddress(sizeof(byte));  
//  _adressSDactive = EEPROM.getAddress(sizeof(byte));
//  _adressSDhour = EEPROM.getAddress(sizeof(byte));
//  _adressSDminute = EEPROM.getAddress(sizeof(byte));
//  _adressSound = EEPROM.getAddress(sizeof(byte));
//  _adressSerialDebug = EEPROM.getAddress(sizeof(byte));
//  
//  readEEPROMvars();
}

//Reads settings from EEPROM non-volatile memory and loads vars
void Settings::readEEPROMvars() {
  _waterTimed = EEPROM.readByte(_adressWaterTimed);
  _waterHour = EEPROM.readByte(_adressWaterHour);
  _waterMinute = EEPROM.readByte(_adresswaterMinute);
  _floodMinute = EEPROM.readByte(_adressFloodMinute);
  _phAlarmUp = EEPROM.readFloat(_adressPHalarmUp);
  _phAlarmDown = EEPROM.readFloat(_adressPHalarmDown);
  _ecAlarmUp = EEPROM.readLong(_adressECalarmUp);
  _ecAlarmDown = EEPROM.readLong(_adressECalarmDown);
  _waterAlarm = EEPROM.readByte(_adressWaterAlarm);
  _nightWatering = EEPROM.readByte(_adressNightWatering);
  
  _sensorMinute = EEPROM.readByte(_adressSensorMinute);
  _sensorSecond = EEPROM.readByte(_adressSensorSecond);  
  _sdActive = EEPROM.readByte(_adressSDactive);
  _sdHour = EEPROM.readByte(_adressSDhour);
  _sdMinute = EEPROM.readByte(_adressSDminute);
  _sound = EEPROM.readByte(_adressSound );
  _serialDebug = EEPROM.readByte(_adressSerialDebug);
}

//Setters
//System Settings
void Settings::setWaterTimed(boolean w) {
  _waterTimed = w;
}

void Settings::setWaterHour(uint8_t w) {
  _waterHour = w;
}

void Settings::setWaterMinute(uint8_t w) {
  _waterMinute = w;
}

void Settings::setFloodMinute(uint8_t f) {
  _floodMinute = f;
}

void Settings::setPHalarmUp(float p) {
  _phAlarmUp = p;
}

void Settings::setPHalarmDown(float p) {
  _phAlarmDown = p;
}

void Settings::setECalarmUp(uint32_t e) {
  _ecAlarmUp = e;
}

void Settings::setECalarmDown(uint32_t e) {
  _ecAlarmDown = e;
}

void Settings::setWaterAlarm(uint8_t w) {
  _waterAlarm = w;
}

void Settings::setNightWatering(boolean n) {
  _nightWatering = n;
}

//Controller Settings
void Settings::setSensorMinute(uint8_t s) {
  _sensorMinute = s;
}

void Settings::setSensorSecond(uint8_t s) {
  _sensorSecond = s;
}

void Settings::setSDactive(boolean s) {
  _sdActive = s;
}

void Settings::setSDhour(uint8_t s) {
  _sdHour = s;
}

void Settings::setSDminute(uint8_t s) {
  _sdMinute = s;
}

void Settings::setSound(boolean s) {
  _sound = s;
}

void Settings::setSerialDebug(boolean s) {
  _serialDebug = s;
}

//Status vars
void Settings::setNextWhour(uint8_t n) {
  _nextWhour = n;
}

void Settings::setNextWminute(uint8_t n) {
  _nextWminute = n;
}

void Settings::setManualPump(boolean m) {
  _manualPump = m;
}

void Settings::setNightWateringStopped(boolean n) {
  _nightWateringStopped = n;
}

void Settings::setWateringPlants(boolean w) {
  _wateringPlants = w;
}

void Settings::setAlarmTriggered(boolean a) {
  _alarmTriggered = a;
}

//Getters
//System Settings
boolean Settings::getWaterTimed() {
  return _waterTimed;
}

uint8_t Settings::getWaterHour() {
  return _waterHour;
}

uint8_t Settings::getWaterMinute() {
  return _waterMinute;
}

uint8_t Settings::getFloodMinute() {
  return _floodMinute;
}

float Settings::getPHalarmUp() {
  return _phAlarmUp;
}

float Settings::getPHalarmDown() {
  return _phAlarmDown;
}

uint32_t Settings::getECalarmUp() {
  return _ecAlarmUp;
}

uint32_t Settings::getECalarmDown() {
  return _ecAlarmDown;
}

uint8_t Settings::getWaterAlarm() {
  return _waterAlarm;
}

boolean Settings::getNightWatering() {
  return _nightWatering;
}

//Controller Settings
uint8_t Settings::getSensorMinute() {
  return _sensorMinute;
}

uint8_t Settings::getSensorSecond() {
  return _sensorSecond;
}

boolean Settings::getSDactive() {
  return _sdActive;
}

uint8_t Settings::getSDhour() {
  return _sdHour;
}

uint8_t Settings::getSDminute() {
  return _sdMinute;
}

boolean Settings::getSound() {
  return _sound;
}

boolean Settings::getSerialDebug() {
  return _serialDebug;
}

//Status vars
uint8_t Settings::getNextWhour() {
  return _nextWhour;
}

uint8_t Settings::getNextWminute() {
  return _nextWminute;
}

boolean Settings::getManualPump() {
  return _manualPump;
}

boolean Settings::getNightWateringStopped() {
  return _nightWateringStopped;
}

boolean Settings::getWateringPlants() {
  return _wateringPlants;
}

boolean Settings::getAlarmTriggered() {
  return _alarmTriggered;
}


