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

void SensorEC::init() {
	pinMode(_pin, INPUT);
}

void SensorEC::update() {
	_ecs[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	smooth();
}

void SensorEC::fastUpdate() {
	float e = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = e;
	}
	smooth();
}

float SensorEC::get() const {
	return _ec;
}

//Returns EC in mSiemens/cm.
float SensorEC::getRaw() const {
	uint16_t rawVolt = analogRead(_pin)*(float)5000/1024;
	//Serial.print("EC -> rawVolt: ");
	//Serial.print(rawVolt);
	float tempCoef = 1.0 + 0.0185*(temperature.getTempCByIndex(0)-25.0);
	//Serial.print(" | temp coef: ");
	//Serial.print(tempCoef);
	float voltCoef = (float)rawVolt/tempCoef;
	//Serial.print(" | voltCoef: ");
	//Serial.println(voltCoef);
	
	//No solution
	if (voltCoef < 150) {
		return 0;
	//Out of range
	} else if (voltCoef > 3300) {
		return 0;
	//1ms/cm<EC<=3ms/cm
	} else if (voltCoef <= 448) {
		return (float)(6.84*voltCoef-64.32)/1000;
	//3ms/cm<EC<=10ms/cm
	} else if (voltCoef <= 1457) {
		return (float)(6.98*voltCoef-127)/1000;	
	//10ms/cm<EC<20ms/cm	
	} else {
		return (float)(5.3*voltCoef+2278)/1000;
	}
}

void SensorEC::smooth() {
	float res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _ecs[i]; }
	_ec = (float)(res / _numSamples);
}
