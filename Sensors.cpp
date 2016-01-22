#include "Sensors.h"

//Constructors
Sensors::Sensors(Settings *settings) 
: _settings(settings), _humidity(humidIn), _light(lightIn), _water(waterTrigger,waterEcho) {
	
	//Init reservoir status
	_reservoir = _settings->getReservoirModule();
	boolean serialDbg = _settings->getSerialDebug();
	//Init & config sensor instances 
	_ec.init();
	_ec.setSerialDebug(serialDbg);
	_humidity.init();
	_light.init();
	_ph.init();
	_ph.setSerialDebug(serialDbg);
	_temp.init();
	_temp.setCelsius(_settings->getCelsius());
	_water.setMax(_settings->getMaxWaterLvl());
	_water.setMin(_settings->getMinWaterLvl());
	_water.init();		
}

Sensors::Sensors(const Sensors &other) 
: _humidity(humidIn), _light(lightIn), _water(waterTrigger,waterEcho) {
	
	_settings = other._settings;
	_temp = other._temp;
	_light = other._light;
	_humidity = other._humidity;
	_ec = other._ec;
	_ph = other._ph;
	_water = other._water;
	_reservoir = other._reservoir;
}

Sensors& Sensors::operator=(const Sensors &other) {
	_temp = other._temp;
	_light = other._light;
	_humidity = other._humidity;
	_ec = other._ec;
	_ph = other._ph;
	_water = other._water;
	_reservoir = other._reservoir;
		
	return *this;	
}

Sensors::~Sensors() {}

//Getters
float Sensors::getTemp() const { 
	return _temp.get(); 
}

uint16_t Sensors::getLight() const { 
	return _light.get(); 
}

uint8_t Sensors::getHumidity() const { 
	return _humidity.get(); 
}

uint16_t Sensors::getEC() const { 
	return _ec.get(); 
}

float Sensors::getPH() const { 
	return _ph.get(); 
}

uint8_t Sensors::getWaterLevel() const { 
	return _water.get(); 
}
	
//Polls sonic range sensor and returns raw reading in cm
uint16_t Sensors::getRawWaterLevel() {
	_water.getRaw();
}

//Returns light in lux
uint16_t Sensors::getRawLightLevel() {
	_light.getRaw();
}

//Sets internal states
//Serial debug affects EC & pH while calibrating
void Sensors::setSerialDebug(boolean d) {
	_ec.setSerialDebug(d);
	_ph.setSerialDebug(d);
}

//This affects if all sensors are updated() and used or not
void Sensors::setReservoir(boolean r) {
	_reservoir = r;
	fastUpdate();
}

//Sets temperature mode
void Sensors::setCelsius(boolean c) {
	_temp.setCelsius(c);
}

//Set max nutrient level
void Sensors::setMaxLvl(uint16_t m) {
	_water.setMax(m);
}

void Sensors::setMinLvl(uint16_t m) {
	_water.setMin(m);
}


//These are always off if module is deactivated
boolean Sensors::ecOffRange() {
	if (_reservoir) {
		if ((_ec.get() < _settings->getECalarmDown())
			|| (_ec.get() > _settings->getECalarmUp())) {
			return true;
		}
	}
	return false;
}

boolean Sensors::phOffRange() {
	if (_reservoir) {
		if ((_ph.get() < _settings->getPHalarmDown()) 
			|| (_ph.get() > _settings->getPHalarmUp())) {
			return true;
		}
	}
	return false;
}

boolean Sensors::lvlOffRange() {
	if (_reservoir) {
		if (_water.get() < _settings->getWaterAlarm()) {
			return true;
		}
	}
	return false;
}

//Updates sample arrays with readings from sensors and performs smoothing
void Sensors::update() {
	_humidity.update();
	_light.update();
	_temp.update();
	if (_reservoir) {
		_ph.update();
		_ec.update();
		_water.update();
	}
}

//Reads once from each sensor and fills the array with this measurement
//Used at setup() to have some sensor data to display at init
void Sensors::fastUpdate() {
	_humidity.fastUpdate();
	_light.fastUpdate();
	_temp.fastUpdate();
	if (_reservoir) {
		_ph.fastUpdate();
		_ec.fastUpdate();
		_water.fastUpdate();
	}
}

//This should be set while calibrating to prevent messing up circuits if update() or adjustTemp() called
void Sensors::calibratingPH(boolean c) {
	_ph.calibrating(c);
}

void Sensors::calibratingEC(boolean c) {
	_ec.calibrating(c);
}

//pH circuit commands
void Sensors::resetPH() {
	_ph.resetToFactory();
}

void Sensors::getPHinfo() {
	_ph.getInfo();
	_ph.getStatus();
}

void Sensors::setPHled(boolean state) {
	_ph.setLed(state);
}

void Sensors::setPHcontinuous() {
	_ph.setContinuous();
}

void Sensors::setPHstandby() {
	_ph.setStandby();
}

void Sensors::setPHfour() {
	_ph.setFour();
}

void Sensors::setPHseven() {
	_ph.setSeven();
}

void Sensors::setPHten() {
	_ph.setTen();
}

//Sends command to pH sensor to adjust readings to temperature only if sensor not being calibrated
void Sensors::adjustPHtemp() {
	_ph.adjustTemp(temperature.getTempCByIndex(0));
}

//EC circuit commands
void Sensors::resetEC() {
	_ec.resetToFactory();
}

void Sensors::getECinfo() {
	_ec.getInfo();
	_ec.getStatus();
}

void Sensors::setECled(boolean state) {
	_ec.setLed(state);
}

void Sensors::setECcontinuous() {
	_ec.setContinuous();
}

void Sensors::setECstandby() {
	_ec.setStandby();
}

void Sensors::setECprobeType() {
	_ec.setProbeType();
}

void Sensors::setECdry() {
	_ec.setDry();
}

void Sensors::setEChighCalib() {
	_ec.setHighCalib();
}

void Sensors::setEClowCalib() {
	_ec.setLowCalib();
}

//Sends command to EC sensor to adjust readings to temperature if not calibrating sensor
void Sensors::adjustECtemp() {
	_ec.adjustTemp(temperature.getTempCByIndex(0));
}