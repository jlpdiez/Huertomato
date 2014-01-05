#include "Settings.h"

//TODO: Only load if there is data:
//http://playground.arduino.cc/Code/EEPROMLoadAndSaveSettings

//Constructors
Settings::Settings() {
	//TODO: add waterMin/waterMax to settings and menus
	//Status variables - Not read from EEPROM
	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
	
	//These two are set in the main setup()
	//_nextWhour = 20;
	//_nextWminute = 5; 
	//Init config - Maybe useful to program new hardwares?
	/*_waterTimed = true;
	_waterHour = 2 ;
	_waterMinute = 30;
	_floodMinute = 15;
	_phAlarmUp = 7.50;
	_phAlarmDown = 6.25;
	_ecAlarmUp = 3200;
	_ecAlarmDown = 1200;
	_waterAlarm = 33;
	_nightWatering = true;
	_lightThreshold = 10;
  
	//Controller Settings
	_sensorSecond = 5;  
	_sdActive = true;
	_sdHour = 0;
	_sdMinute = 5;
	_sound = false;
	_serialDebug = true;*/
  
	setEEPROMaddresses();
	readEEPROMvars();
}

Settings::Settings(const Settings &other) {
	setEEPROMaddresses();

	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
	  
	//System Settings
	_waterTimed = other._waterTimed;
	_waterHour = other._waterHour;
	_waterMinute = other._waterMinute;
	_floodMinute = other._floodMinute;
	_phAlarmUp = other._phAlarmUp;
	_phAlarmDown = other._phAlarmDown;
	_ecAlarmUp = other._ecAlarmUp;
	_ecAlarmDown = other._ecAlarmDown;
	_waterAlarm = other._waterAlarm;
	_nightWatering = other._nightWatering;
	_lightThreshold = other._lightThreshold;
	  
	//Controller Settings
	_sensorSecond = other._sensorSecond;
	_sdActive = other._sdActive;
	_sdHour = other._sdHour;
	_sdMinute = other._sdMinute;
	_sound = other._sound;
	_serialDebug = other._serialDebug;
}

Settings& Settings::operator=(const Settings &other) {
	setEEPROMaddresses();
	
	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
		  
	//System Settings
	_waterTimed = other._waterTimed;
	_waterHour = other._waterHour;
	_waterMinute = other._waterMinute;
	_floodMinute = other._floodMinute;
	_phAlarmUp = other._phAlarmUp;
	_phAlarmDown = other._phAlarmDown;
	_ecAlarmUp = other._ecAlarmUp;
	_ecAlarmDown = other._ecAlarmDown;
	_waterAlarm = other._waterAlarm;
	_nightWatering = other._nightWatering;
	_lightThreshold = other._lightThreshold;
		  
	//Controller Settings
	_sensorSecond = other._sensorSecond;
	_sdActive = other._sdActive;
	_sdHour = other._sdHour;
	_sdMinute = other._sdMinute;
	_sound = other._sound;
	_serialDebug = other._serialDebug;
	
	return *this;
}

//Destructor
Settings::~Settings() {}
	
//Sets EEPROM addresses for all variables
void Settings::setEEPROMaddresses() {
	_adressWaterTimed = EEPROM.getAddress(sizeof(byte));
	_adressWaterHour = EEPROM.getAddress(sizeof(byte));
	_adressWaterMinute = EEPROM.getAddress(sizeof(byte));
	_adressFloodMinute = EEPROM.getAddress(sizeof(byte));
	_adressPHalarmUp = EEPROM.getAddress(sizeof(float));
	_adressPHalarmDown = EEPROM.getAddress(sizeof(float));
	_adressECalarmUp = EEPROM.getAddress(sizeof(int));
	_adressECalarmDown = EEPROM.getAddress(sizeof(int));
	_adressWaterAlarm = EEPROM.getAddress(sizeof(byte));
	_adressNightWatering = EEPROM.getAddress(sizeof(byte));
	_adressSensorSecond = EEPROM.getAddress(sizeof(byte));
	_adressSDactive = EEPROM.getAddress(sizeof(byte));
	_adressSDhour = EEPROM.getAddress(sizeof(byte));
	_adressSDminute = EEPROM.getAddress(sizeof(byte));
	_adressSound = EEPROM.getAddress(sizeof(byte));
	_adressSerialDebug = EEPROM.getAddress(sizeof(byte));
	_adressLightThreshold = EEPROM.getAddress(sizeof(byte));
}

//Reads settings from EEPROM non-volatile memory and loads vars
void Settings::readEEPROMvars() {
	_waterTimed = EEPROM.readByte(_adressWaterTimed);
	_waterHour = EEPROM.readByte(_adressWaterHour);
	_waterMinute = EEPROM.readByte(_adressWaterMinute);
	_floodMinute = EEPROM.readByte(_adressFloodMinute);
	_phAlarmUp = EEPROM.readFloat(_adressPHalarmUp);
	_phAlarmDown = EEPROM.readFloat(_adressPHalarmDown);
	_ecAlarmUp = EEPROM.readInt(_adressECalarmUp);
	_ecAlarmDown = EEPROM.readInt(_adressECalarmDown);
	_waterAlarm = EEPROM.readByte(_adressWaterAlarm);
	_nightWatering = EEPROM.readByte(_adressNightWatering);  
	_sensorSecond = EEPROM.readByte(_adressSensorSecond);  
	_sdActive = EEPROM.readByte(_adressSDactive);
	_sdHour = EEPROM.readByte(_adressSDhour);
	_sdMinute = EEPROM.readByte(_adressSDminute);
	_sound = EEPROM.readByte(_adressSound );
	_serialDebug = EEPROM.readByte(_adressSerialDebug);
	_lightThreshold = EEPROM.readByte(_adressLightThreshold);
}

//Setters - These store their value on EEPROM too
//System Settings
void Settings::setWaterTimed(const boolean w) { 
	_waterTimed = w; 
	EEPROM.updateByte(_adressWaterTimed,w);
}

void Settings::setWaterHour(const uint8_t w) { 
	_waterHour = w; 
	EEPROM.updateByte(_adressWaterHour,w);	
}

void Settings::setWaterMinute(const uint8_t w) { 
	_waterMinute = w; 
	EEPROM.updateByte(_adressWaterMinute,w);
}

void Settings::setFloodMinute(const uint8_t f) { 
	_floodMinute = f; 
	EEPROM.updateByte(_adressFloodMinute,f);
}

void Settings::setPHalarmUp(const float p) { 
	_phAlarmUp = p; 
	EEPROM.updateFloat(_adressPHalarmUp,p);
}

void Settings::setPHalarmDown(const float p) { 
	_phAlarmDown = p; 
	EEPROM.updateFloat(_adressPHalarmDown,p);
}

void Settings::setECalarmUp(const uint16_t e) { 
	_ecAlarmUp = e; 
	EEPROM.updateInt(_adressECalarmUp,e);
}

void Settings::setECalarmDown(const uint16_t e) { 
	_ecAlarmDown = e; 
	EEPROM.updateInt(_adressECalarmDown,e);
}

void Settings::setWaterAlarm(const uint8_t w) { 
	_waterAlarm = w; 
	EEPROM.updateByte(_adressWaterAlarm,w);
}

void Settings::setNightWatering(const boolean n) { 
	_nightWatering = n; 
	EEPROM.updateByte(_adressNightWatering,n);
}

void Settings::setLightThreshold(const uint8_t l) {
	_lightThreshold = l;
	EEPROM.updateByte(_adressLightThreshold,l);
}

//Controller Settings
void Settings::setSensorSecond(const uint8_t s) { 
	_sensorSecond = s; 
	EEPROM.updateByte(_adressSensorSecond,s);
}

void Settings::setSDactive(const boolean s) { 
	_sdActive = s; 
	EEPROM.updateByte(_adressSDactive,s);
}

void Settings::setSDhour(const uint8_t s) { 
	_sdHour = s; 
	EEPROM.updateByte(_adressSDhour,s);
}

void Settings::setSDminute(const uint8_t s) { 
	_sdMinute = s; 
	EEPROM.updateByte(_adressSDminute,s);
}

void Settings::setSound(const boolean s) { 
	_sound = s; 
	EEPROM.updateByte(_adressSound,s);
}

void Settings::setSerialDebug(const boolean s) { 
	_serialDebug = s; 
	EEPROM.updateByte(_adressSerialDebug,s);
}

//Status vars - These are not stored in EEPROM
void Settings::setNextWhour(const uint8_t n) { _nextWhour = n; }

void Settings::setNextWminute(const uint8_t n) { _nextWminute = n; }

//void Settings::setManualPump(const boolean m) {	_manualPump = m; }

void Settings::setNightWateringStopped(const boolean n) { _nightWateringStopped = n; }

void Settings::setWateringPlants(const boolean w) { _wateringPlants = w; }

void Settings::setAlarmTriggered(const boolean a) { _alarmTriggered = a; }

//Getters
//System Settings
boolean Settings::getWaterTimed() const { return _waterTimed; }

uint8_t Settings::getWaterHour() const { return _waterHour; }

uint8_t Settings::getWaterMinute() const { return _waterMinute; }

uint8_t Settings::getFloodMinute() const { return _floodMinute; }

float Settings::getPHalarmUp() const { return _phAlarmUp; }

float Settings::getPHalarmDown() const { return _phAlarmDown; }

uint16_t Settings::getECalarmUp() const { return _ecAlarmUp; }

uint16_t Settings::getECalarmDown() const { return _ecAlarmDown; }

uint8_t Settings::getWaterAlarm() const { return _waterAlarm; }

boolean Settings::getNightWatering() const { return _nightWatering; }

uint8_t Settings::getLightThreshold() const { return _lightThreshold; }

//Controller Settings
uint8_t Settings::getSensorSecond() const { return _sensorSecond; }

boolean Settings::getSDactive() const { return _sdActive; }

uint8_t Settings::getSDhour() const { return _sdHour; }

uint8_t Settings::getSDminute() const { return _sdMinute; }

boolean Settings::getSound() const { return _sound; }

boolean Settings::getSerialDebug() const { return _serialDebug; }

//Status vars
uint8_t Settings::getNextWhour() const { return _nextWhour; }

uint8_t Settings::getNextWminute() const { return _nextWminute; }

//boolean Settings::getManualPump() const { return _manualPump; }

boolean Settings::getNightWateringStopped() const { return _nightWateringStopped; }

boolean Settings::getWateringPlants() const { return _wateringPlants; }

boolean Settings::getAlarmTriggered() const { return _alarmTriggered; }


