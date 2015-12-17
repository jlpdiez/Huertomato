#include "SensorEC.h"

#include "SensorLight.h"

SensorEC::SensorEC(const int pin)
: Sensor(pin) {
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = 0;
	}
}

SensorEC::SensorEC(const SensorEC &other) : Sensor(other) {
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = other._ecs[i];
	}
	_ec = other._ec;
}

SensorEC& SensorEC::operator =(const SensorEC &other) {
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = other._ecs[i];
	}
	_ec = other._ec;
	return *this;
}

SensorEC::~SensorEC() {}

Sensor::SensName SensorEC::getType() const {
	return Sensor::Ec;
}

void SensorEC::init() {}

void SensorEC::update() {
	_ecs[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	smooth();
}

void SensorEC::fastUpdate() {
	uint16_t e = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = e;
	}
	smooth();
}

uint16_t SensorEC::get() const {
	return _ec;
}

//Returns EC in uSiemens/cm.
uint16_t SensorEC::getRaw() const {
	uint16_t rawVolt = analogRead(ecPin)*(float)5000/1024;
	float tempCoef = 1.0 + 0.0185*(temperature.getTempCByIndex(0)-25.0);
	float voltCoef = (float)rawVolt/tempCoef;
	
	//No solution
	if (voltCoef < 150) {
		return 0;
	//Out of range
	} else if (voltCoef > 3300) {
		return 0;
	//1ms/cm<EC<=3ms/cm
	} else if (voltCoef <= 448) {
		return 6.84*voltCoef-64.32;
	//3ms/cm<EC<=10ms/cm
	} else if (voltCoef <= 1457) {
		return 6.98*voltCoef-127;	
	//10ms/cm<EC<20ms/cm	
	} else {
		return 5.3*voltCoef+2278;
	}
	//ECcurrent/=1000;    //convert us/cm to ms/cm
}

void SensorEC::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _ecs[i]; }
	_ec = (uint16_t)(res / _numSamples);
}
