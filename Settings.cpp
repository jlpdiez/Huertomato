#include "Settings.h"

//TODO: Only load if there is data:
//http://playground.arduino.cc/Code/EEPROMLoadAndSaveSettings

//Constructors
Settings::Settings() {
	//Status variables - Not read from EEPROM
	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
  
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
	_maxWaterLvl = other._maxWaterLvl;
	_minWaterLvl= other._minWaterLvl;
	_pumpProtectionLvl = other._pumpProtectionLvl;
	  
	//Controller Settings
	_sensorSecond = other._sensorSecond;
	_sdActive = other._sdActive;
	_sdHour = other._sdHour;
	_sdMinute = other._sdMinute;
	_sound = other._sound;
	_serialDebug = other._serialDebug;
	_reservoirModule = other._reservoirModule;
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
	_maxWaterLvl = other._maxWaterLvl;
	_minWaterLvl= other._minWaterLvl;
	_pumpProtectionLvl = other._pumpProtectionLvl;
		  
	//Controller Settings
	_sensorSecond = other._sensorSecond;
	_sdActive = other._sdActive;
	_sdHour = other._sdHour;
	_sdMinute = other._sdMinute;
	_sound = other._sound;
	_serialDebug = other._serialDebug;
	_reservoirModule = other._reservoirModule;
	
	return *this;
}

//Destructor
Settings::~Settings() {}
	
//Sets EEPROM addresses for all variables
void Settings::setEEPROMaddresses() {
	_addressWaterTimed = EEPROM.getAddress(sizeof(byte));
	_addressWaterHour = EEPROM.getAddress(sizeof(byte));
	_addressWaterMinute = EEPROM.getAddress(sizeof(byte));
	_addressFloodMinute = EEPROM.getAddress(sizeof(byte));
	_addressPHalarmUp = EEPROM.getAddress(sizeof(float));
	_addressPHalarmDown = EEPROM.getAddress(sizeof(float));
	_addressECalarmUp = EEPROM.getAddress(sizeof(int));
	_adressECalarmDown = EEPROM.getAddress(sizeof(int));
	_addressWaterAlarm = EEPROM.getAddress(sizeof(byte));
	_addressNightWatering = EEPROM.getAddress(sizeof(byte));
	_addressSensorSecond = EEPROM.getAddress(sizeof(byte));
	_addressSDactive = EEPROM.getAddress(sizeof(byte));
	_addressSDhour = EEPROM.getAddress(sizeof(byte));
	_addressSDminute = EEPROM.getAddress(sizeof(byte));
	_addressSound = EEPROM.getAddress(sizeof(byte));
	_addressSerialDebug = EEPROM.getAddress(sizeof(byte));
	_addressLightThreshold = EEPROM.getAddress(sizeof(int));
	_addressReservoirModule = EEPROM.getAddress(sizeof(byte));
	_addressMaxWaterLvl = EEPROM.getAddress(sizeof(int));
	_addressMinWaterLvl = EEPROM.getAddress(sizeof(int));
	_addressPumpProtectionLvl = EEPROM.getAddress(sizeof(byte));
	//_addressVersion == EEPROM.getAddress((sizeof(char[4])));
}

//Reads settings from EEPROM non-volatile memory and loads vars
void Settings::readEEPROMvars() {
	//TODO: Auto load defaults if needed
	// if (EEPROM.read(_addressVersion) != MYVERSION))
	//	setDefault();
	//else 
	_waterTimed = EEPROM.readByte(_addressWaterTimed);
	_waterHour = EEPROM.readByte(_addressWaterHour);
	_waterMinute = EEPROM.readByte(_addressWaterMinute);
	_floodMinute = EEPROM.readByte(_addressFloodMinute);
	_phAlarmUp = EEPROM.readFloat(_addressPHalarmUp);
	_phAlarmDown = EEPROM.readFloat(_addressPHalarmDown);
	_ecAlarmUp = EEPROM.readInt(_addressECalarmUp);
	_ecAlarmDown = EEPROM.readInt(_adressECalarmDown);
	_waterAlarm = EEPROM.readByte(_addressWaterAlarm);
	_nightWatering = EEPROM.readByte(_addressNightWatering);  
	_sensorSecond = EEPROM.readByte(_addressSensorSecond);  
	_sdActive = EEPROM.readByte(_addressSDactive);
	_sdHour = EEPROM.readByte(_addressSDhour);
	_sdMinute = EEPROM.readByte(_addressSDminute);
	_sound = EEPROM.readByte(_addressSound );
	_serialDebug = EEPROM.readByte(_addressSerialDebug);
	_lightThreshold = EEPROM.readInt(_addressLightThreshold);
	_reservoirModule = EEPROM.readByte(_addressReservoirModule);
	_maxWaterLvl = EEPROM.readInt(_addressMaxWaterLvl);
	_minWaterLvl = EEPROM.readInt(_addressMinWaterLvl);
	_pumpProtectionLvl = EEPROM.readByte(_addressPumpProtectionLvl);
}

//Setters - These store their value on EEPROM too
void Settings::setDefault() {	
	//System Settings
	setWaterTimed(true);
	setWaterHour(1);
	setWaterMinute(30);
	setFloodMinute(1);
	setPHalarmUp(14);
	setPHalarmDown(0);
	setECalarmUp(9990);
	setECalarmDown(0);
	setWaterAlarm(0);
	setNightWatering(true);    
	//Controller Settings
	setSensorSecond(2);
	setSDactive(true);
	setSDhour(1);
	setSDminute(0);
	setSound(false);
	setSerialDebug(true);
	setLightThreshold(30);
	setReservoirModule(true);
	setMaxWaterLvl(16);
	setMinWaterLvl(50);
	setPumpProtectionLvl(15);
}

//System Settings
//Also sets _waterModeChanged to true
void Settings::setWaterTimed(const boolean w) { 
	_waterTimed = w; 
	EEPROM.updateByte(_addressWaterTimed,w);
	_waterSettingsChanged = true;
}

void Settings::setWaterHour(const uint8_t w) { 
	_waterHour = w; 
	EEPROM.updateByte(_addressWaterHour,w);	
	_waterSettingsChanged = true;
}

void Settings::setWaterMinute(const uint8_t w) { 
	_waterMinute = w; 
	EEPROM.updateByte(_addressWaterMinute,w);
	_waterSettingsChanged = true;
}

void Settings::setFloodMinute(const uint8_t f) { 
	_floodMinute = f; 
	EEPROM.updateByte(_addressFloodMinute,f);
	_waterSettingsChanged = true;
}

void Settings::setPHalarmUp(const float p) { 
	_phAlarmUp = p; 
	EEPROM.updateFloat(_addressPHalarmUp,p);
}

void Settings::setPHalarmDown(const float p) { 
	_phAlarmDown = p; 
	EEPROM.updateFloat(_addressPHalarmDown,p);
}

void Settings::setECalarmUp(const uint16_t e) { 
	_ecAlarmUp = e; 
	EEPROM.updateInt(_addressECalarmUp,e);
}

void Settings::setECalarmDown(const uint16_t e) { 
	_ecAlarmDown = e; 
	EEPROM.updateInt(_adressECalarmDown,e);
}

void Settings::setWaterAlarm(const uint8_t w) { 
	_waterAlarm = w; 
	EEPROM.updateByte(_addressWaterAlarm,w);
}

void Settings::setNightWatering(const boolean n) { 
	_nightWatering = n; 
	EEPROM.updateByte(_addressNightWatering,n);
}

void Settings::setLightThreshold(const uint16_t l) {
	_lightThreshold = l;
	EEPROM.updateInt(_addressLightThreshold,l);
}

void Settings::setMaxWaterLvl(const uint16_t x) {
	_maxWaterLvl = x;
	EEPROM.updateInt(_addressMaxWaterLvl,x);
}

void Settings::setMinWaterLvl(const uint16_t n) {
	_minWaterLvl = n;
	EEPROM.updateInt(_addressMinWaterLvl,n);
}

void Settings::setPumpProtectionLvl(const uint8_t p) {
	_pumpProtectionLvl = p;
	EEPROM.updateByte(_addressPumpProtectionLvl,p);
}

//Controller Settings
void Settings::setSensorSecond(const uint8_t s) { 
	_sensorSecond = s; 
	EEPROM.updateByte(_addressSensorSecond,s);
	_sensorPollingChanged = true;
}

void Settings::setSDactive(const boolean s) { 
	_sdActive = s; 
	EEPROM.updateByte(_addressSDactive,s);
	_sdSettingsChanged = true;
}

void Settings::setSDhour(const uint8_t s) { 
	_sdHour = s; 
	EEPROM.updateByte(_addressSDhour,s);
	_sdSettingsChanged = true;
}

void Settings::setSDminute(const uint8_t s) { 
	_sdMinute = s; 
	EEPROM.updateByte(_addressSDminute,s);
	_sdSettingsChanged = true;
}

void Settings::setSound(const boolean s) { 
	_sound = s; 
	EEPROM.updateByte(_addressSound,s);
}

void Settings::setSerialDebug(const boolean s) { 
	_serialDebug = s; 
	EEPROM.updateByte(_addressSerialDebug,s);
	_serialDebugChanged = true;
}

void Settings::setReservoirModule(const boolean r) {
	_reservoirModule = r;
	EEPROM.updateByte(_addressReservoirModule,r);
}

//Status vars - These are not stored in EEPROM
void Settings::setNextWhour(const uint8_t n) { _nextWhour = n; }

void Settings::setNextWminute(const uint8_t n) { _nextWminute = n; }

void Settings::setNightWateringStopped(const boolean n) { 
	_nightWateringStopped = n; 
	_systemStateChanged = true;
}

void Settings::setWateringPlants(const boolean w) { 
	_wateringPlants = w; 
	_systemStateChanged = true;
}

void Settings::setAlarmTriggered(const boolean a) { 
	_alarmTriggered = a; 
	_systemStateChanged = true;
}

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

uint16_t Settings::getLightThreshold() const { return _lightThreshold; }
	
uint16_t Settings::getMaxWaterLvl() const { return _maxWaterLvl; }
	
uint16_t Settings::getMinWaterLvl() const { return _minWaterLvl; }

uint8_t Settings::getPumpProtectionLvl() const { return _pumpProtectionLvl; }

//Controller Settings
uint8_t Settings::getSensorSecond() const { return _sensorSecond; }

boolean Settings::getSDactive() const { return _sdActive; }

uint8_t Settings::getSDhour() const { return _sdHour; }

uint8_t Settings::getSDminute() const { return _sdMinute; }

boolean Settings::getSound() const { return _sound; }

boolean Settings::getSerialDebug() const { return _serialDebug; }
	
boolean Settings::getReservoirModule() const { return _reservoirModule; }

//Status vars
uint8_t Settings::getNextWhour() const { return _nextWhour; }

uint8_t Settings::getNextWminute() const { return _nextWminute; }

boolean Settings::getNightWateringStopped() const { return _nightWateringStopped; }

boolean Settings::getWateringPlants() const { return _wateringPlants; }

boolean Settings::getAlarmTriggered() const { return _alarmTriggered; }
	
boolean Settings::systemStateChanged() {
	boolean res = _systemStateChanged;
	_systemStateChanged = false;
	return res;
}

boolean Settings::waterSettingsChanged() {
	boolean res = _waterSettingsChanged;
	_waterSettingsChanged = false;
	return res;
}

boolean Settings::sdSettingsChanged() {
	boolean res = _sdSettingsChanged;
	_sdSettingsChanged = false;
	return res;
}

boolean Settings::sensorPollingChanged() {
	boolean res = _sensorPollingChanged;
	_sensorPollingChanged = false;
	return res;
}

boolean Settings::serialDebugChanged() {
	boolean res = _serialDebugChanged;
	_serialDebugChanged = false;
	return res;
}
