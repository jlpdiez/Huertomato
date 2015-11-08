#include "SensorTemp.h"

SensorTemp::SensorTemp(Settings *settings, const int pin)
: Sensor(settings,pin) {
	
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_temps[i] = 0;
	}
}

SensorTemp::SensorTemp(const SensorTemp &other) : Sensor(other) {
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_temps[i] = other._temps[i];
	}
	_temp = other._temp;
}

SensorTemp& SensorTemp::operator =(const SensorTemp &other) {
	_settings = other._settings;
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_temps[i] = other._temps[i];
	}
	_temp = other._temp;
	return *this;
}

SensorTemp::~SensorTemp() {}

Sensor::SensName SensorTemp::getType() const {
	return Sensor::Temperature;
}

void SensorTemp::init() {
	temperature.begin();
	temperature.setResolution(11);
}

void SensorTemp::update() {
	_temps[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
	_iSample = 0;
	smooth();
}

void SensorTemp::fastUpdate() {
	float t = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_temps[i] = t;
	}
	smooth();

}

float SensorTemp::get() const {
	return _temp;
}

float SensorTemp::getRaw() const {
	temperature.requestTemperatures();
	if (_settings->getCelsius())
		return temperature.getTempCByIndex(0);
	else
		return temperature.getTempFByIndex(0);
}

void SensorTemp::smooth() {
	float res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _temps[i]; }
	_temp = (float)(res / _numSamples);
}