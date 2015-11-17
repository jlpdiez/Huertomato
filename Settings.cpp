#include "Settings.h"

//Constructors
Settings::Settings() {
	//Status variables - Not read from EEPROM
	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
	_pumpProtected = false;
  
	setEEPROMaddresses();
	readEEPROMvars();
}

Settings::Settings(const Settings &other) {
	setEEPROMaddresses();

	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
	_pumpProtected = false;
	  
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
	_pumpProtection = other._pumpProtection;
	_pumpProtectionLvl = other._pumpProtectionLvl;
	  
	//Controller Settings
	_sensorSecond = other._sensorSecond;
	_sdActive = other._sdActive;
	_sdHour = other._sdHour;
	_sdMinute = other._sdMinute;
	_sound = other._sound;
	_led = other._led;
	_celsius = other._celsius;
	_serialDebug = other._serialDebug;
	_reservoirModule = other._reservoirModule;
}

Settings& Settings::operator=(const Settings &other) {
	setEEPROMaddresses();
	
	_nightWateringStopped = false;
	_wateringPlants = false;
	_alarmTriggered = false;
	_pumpProtected = false;
		  
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
	_pumpProtection = other._pumpProtection;
	_pumpProtectionLvl = other._pumpProtectionLvl;
		  
	//Controller Settings
	_sensorSecond = other._sensorSecond;
	_sdActive = other._sdActive;
	_sdHour = other._sdHour;
	_sdMinute = other._sdMinute;
	_sound = other._sound;
	_led = other._led;
	_celsius = other._celsius;
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
	_addressLed = EEPROM.getAddress(sizeof(byte));
	_addressCelsius = EEPROM.getAddress(sizeof(byte));
	_addressSerialDebug = EEPROM.getAddress(sizeof(byte));
	_addressLightThreshold = EEPROM.getAddress(sizeof(int));
	_addressReservoirModule = EEPROM.getAddress(sizeof(byte));
	_addressMaxWaterLvl = EEPROM.getAddress(sizeof(int));
	_addressMinWaterLvl = EEPROM.getAddress(sizeof(int));
	_addressPumpProtectionLvl = EEPROM.getAddress(sizeof(byte));
	_addressPumpProtection = EEPROM.getAddress(sizeof(byte));
	_addressLed = EEPROM.getAddress(sizeof(byte));
	_addressVersion = EEPROM.getAddress(sizeof(float));	
}

//Reads settings from EEPROM non-volatile memory and loads vars
void Settings::readEEPROMvars() {
	//If version number isn't the same we reset to default settings
	if (EEPROM.readFloat(_addressVersion) != versionNumber)
		setDefault();	
		
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
	_sound = EEPROM.readByte(_addressSound);
	_led = EEPROM.readByte(_addressLed);
	_celsius = EEPROM.readByte(_addressCelsius);
	_serialDebug = EEPROM.readByte(_addressSerialDebug);
	_lightThreshold = EEPROM.readInt(_addressLightThreshold);
	_reservoirModule = EEPROM.readByte(_addressReservoirModule);
	_maxWaterLvl = EEPROM.readInt(_addressMaxWaterLvl);
	_minWaterLvl = EEPROM.readInt(_addressMinWaterLvl);
	_pumpProtection = EEPROM.readByte(_addressPumpProtection);
	_pumpProtectionLvl = EEPROM.readByte(_addressPumpProtectionLvl);
}

//Setters - These store their value on EEPROM too
void Settings::setDefault() {	
	//Saves current version number to EEPROM
	EEPROM.updateFloat(_addressVersion,versionNumber);
	//System Settings
	EEPROM.updateByte(_addressWaterTimed,1);
	EEPROM.updateByte(_addressWaterHour,1);	
	EEPROM.updateByte(_addressWaterMinute,30);
	EEPROM.updateByte(_addressFloodMinute,1);
	EEPROM.updateFloat(_addressPHalarmUp,14.0);
	EEPROM.updateFloat(_addressPHalarmDown,0.0);
	EEPROM.updateInt(_addressECalarmUp,9990);
	EEPROM.updateInt(_adressECalarmDown,0);
	EEPROM.updateByte(_addressWaterAlarm,0);
	EEPROM.updateByte(_addressNightWatering,1);  
	//Controller Settings
	EEPROM.updateByte(_addressSensorSecond,10);
	EEPROM.updateByte(_addressSDactive,1);
	EEPROM.updateByte(_addressSDhour,1);
	EEPROM.updateByte(_addressSDminute,0);
	EEPROM.updateByte(_addressSound,1);
	EEPROM.updateByte(_addressLed,1);
	EEPROM.updateByte(_addressCelsius,1);
	EEPROM.updateByte(_addressSerialDebug,1);
	EEPROM.updateInt(_addressLightThreshold,30);
	EEPROM.updateByte(_addressReservoirModule,1);
	EEPROM.updateInt(_addressMaxWaterLvl,16);
	EEPROM.updateInt(_addressMinWaterLvl,50);
	EEPROM.updateByte(_addressPumpProtection,0);
	EEPROM.updateByte(_addressPumpProtectionLvl,15);
}

//System Settings
//Also sets _waterModeChanged to true
boolean Settings::setWaterTimed(const boolean w) { 
	_waterTimed = w; 
	EEPROM.updateByte(_addressWaterTimed,w);
	_waterSettingsChanged = true;
	return true;
}

boolean Settings::setWaterHour(const uint8_t w) { 
	if ((w >= 0) & (w < 24)) {
		_waterHour = w; 
		EEPROM.updateByte(_addressWaterHour,w);	
		_waterSettingsChanged = true;
		return true;
	} else
		return false;
}

boolean Settings::setWaterMinute(const uint8_t w) { 
	if ((w >= 0) & (w < 60)) {
		_waterMinute = w; 
		EEPROM.updateByte(_addressWaterMinute,w);
		_waterSettingsChanged = true;
		return true;
	} else
		return false;
}

boolean Settings::setFloodMinute(const uint8_t f) { 
	if ((f >= 0) & (f < 60)) {
		_floodMinute = f; 
		EEPROM.updateByte(_addressFloodMinute,f);
		_waterSettingsChanged = true;
		return true;
	} else
		return false;
}

boolean Settings::setPHalarmUp(const float p) { 
	if ((p >= 0) && (p < 14.00)) {
		_phAlarmUp = p; 
		EEPROM.updateFloat(_addressPHalarmUp,p);
		return true;
	} else
		return false;
}

boolean Settings::setPHalarmDown(const float p) { 
	if ((p >= 0) && (p < 14.00)) {
		_phAlarmDown = p; 
		EEPROM.updateFloat(_addressPHalarmDown,p);
		return true;
	} else
		return false;
}

boolean Settings::setECalarmUp(const uint16_t e) { 
	if ((e >= 0) && (e < 9999)) {
		_ecAlarmUp = e; 
		EEPROM.updateInt(_addressECalarmUp,e);
		return true;
	} else
		return false;
}

boolean Settings::setECalarmDown(const uint16_t e) { 
	if ((e >= 0) && (e < 9999)) {
		_ecAlarmDown = e;
		EEPROM.updateInt(_adressECalarmDown,e);
		return true;
	} else
		return false;
}

boolean Settings::setWaterAlarm(const uint8_t w) { 
	if ((w >= 0) && (w < 101)) {
		_waterAlarm = w; 
		EEPROM.updateByte(_addressWaterAlarm,w);
		return true;
	} else
		return false;
}

boolean Settings::setNightWatering(const boolean n) { 
	_nightWatering = n; 
	EEPROM.updateByte(_addressNightWatering,n);
	return true;
}

boolean Settings::setLightThreshold(const uint16_t l) {
	if ((l >= 0) && (l < 9999)) {
		_lightThreshold = l;
		EEPROM.updateInt(_addressLightThreshold,l);
		return true;
	} else
		return false;
}

boolean Settings::setMaxWaterLvl(const uint16_t x) {
	//Not valid if greaer than 3m!
	if ((x >= 0) && (x < 301)) {
		_maxWaterLvl = x;
		EEPROM.updateInt(_addressMaxWaterLvl,x);
		return true;
	} else
		return false;
}

boolean Settings::setMinWaterLvl(const uint16_t n) {
	//Not valid if greaer than 3m!
	if ((n >= 0) && (n < 201)) {
		_minWaterLvl = n;
		EEPROM.updateInt(_addressMinWaterLvl,n);
		return true;
	} else
		return false;
}

boolean Settings::setPumpProtection(const boolean p) {
	_pumpProtection = p;
	EEPROM.updateByte(_addressPumpProtection,p);
	return true;
}

boolean Settings::setPumpProtectionLvl(const uint8_t p) {
	if ((p >= 0) && (p < 101)) {
		_pumpProtectionLvl = p;
		EEPROM.updateByte(_addressPumpProtectionLvl,p);
		return true;
	} else
		return false;
}

//Controller Settings
boolean Settings::setSensorSecond(const uint8_t s) { 
	if ((s >= 0) & (s < 60)) {
		_sensorSecond = s; 
		EEPROM.updateByte(_addressSensorSecond,s);
		_sensorPollingChanged = true;
		return true;
	} else
		return false;
}

boolean Settings::setSDactive(const boolean s) { 
	_sdActive = s; 
	EEPROM.updateByte(_addressSDactive,s);
	_sdSettingsChanged = true;
	return true;
}

boolean Settings::setSDhour(const uint8_t s) { 
	if ((s >= 0) & (s < 24)) {
		_sdHour = s;
		EEPROM.updateByte(_addressSDhour,s);
		_sdSettingsChanged = true;
		return true;
	} else
		return false;
}

boolean Settings::setSDminute(const uint8_t s) { 
	if ((s >= 0) & (s < 60)) {
		_sdMinute = s;
		EEPROM.updateByte(_addressSDminute,s);
		_sdSettingsChanged = true;
		return true;
	} else
		return false;
}

boolean Settings::setSound(const boolean s) { 
	_sound = s; 
	EEPROM.updateByte(_addressSound,s);
	return true;
}

boolean Settings::setLed(const boolean l) {
	_led = l;
	EEPROM.updateByte(_addressLed,l);
	return true;
}

boolean Settings::setCelsius(const boolean c) {
	_celsius = c;
	EEPROM.updateByte(_addressCelsius,c);
	return true;
}

boolean Settings::setSerialDebug(const boolean s) { 
	_serialDebug = s; 
	EEPROM.updateByte(_addressSerialDebug,s);
	_serialDebugChanged = true;
	return true;
}

boolean Settings::setReservoirModule(const boolean r) {
	_reservoirModule = r;
	EEPROM.updateByte(_addressReservoirModule,r);
	_moduleChanged = true;
	return true;
}

//Status vars - These are not stored in EEPROM
void Settings::setNextWhour(const uint8_t n) { _nextWhour = n; }

void Settings::setNextWminute(const uint8_t n) { _nextWminute = n; }

void Settings::setNightWateringStopped(const boolean n) { 
	_nightWateringStopped = n; 
	_systemStateChanged = true;
	_waterSettingsChanged = true;
}

void Settings::setWateringPlants(const boolean w) { 
	_wateringPlants = w; 
	_systemStateChanged = true;
}

void Settings::setAlarmTriggered(const boolean a) { 
	_alarmTriggered = a; 
	_systemStateChanged = true;
}

void Settings::setPumpProtected(const boolean p) {
	_pumpProtected = p;
	_systemStateChanged = true;	
	_waterSettingsChanged = true;
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
	
boolean Settings::getPumpProtection() const { return _pumpProtection; }

uint8_t Settings::getPumpProtectionLvl() const { return _pumpProtectionLvl; }

//Controller Settings
uint8_t Settings::getSensorSecond() const { return _sensorSecond; }

boolean Settings::getSDactive() const { return _sdActive; }

uint8_t Settings::getSDhour() const { return _sdHour; }

uint8_t Settings::getSDminute() const { return _sdMinute; }

boolean Settings::getSound() const { return _sound; }
	
boolean Settings::getLed() const { return _led; }
	
boolean Settings::getCelsius() const { return _celsius; }

boolean Settings::getSerialDebug() const { return _serialDebug; }
	
boolean Settings::getReservoirModule() const { return _reservoirModule; }

//Status vars
uint8_t Settings::getNextWhour() const { return _nextWhour; }

uint8_t Settings::getNextWminute() const { return _nextWminute; }

boolean Settings::getNightWateringStopped() const { return _nightWateringStopped; }

boolean Settings::getWateringPlants() const { return _wateringPlants; }

boolean Settings::getAlarmTriggered() const { return _alarmTriggered; }
	
boolean Settings::getPumpProtected() const { return _pumpProtected; }
	
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

boolean Settings::moduleChanged() {
	boolean res = _moduleChanged;
	_moduleChanged = false;
	return res;
}

//Updates RTC internal time
void Settings::setRTCtime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, int y) {
	tmElements_t t;
	t.Hour = h;
	t.Minute = m;
	t.Second = s;
	t.Day = d;
	t.Month = mo;
	//year argument is offset from 1970
	t.Year = y - 1970;
	time_t time = makeTime(t);
	setTime(time);
	RTC.set(time);
}