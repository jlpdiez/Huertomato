#include "SensorHumid.h"

SensorHumid::SensorHumid(const int pin)
: Sensor(pin) {
	
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_humidities[i] = 0;
	}
}

SensorHumid::SensorHumid(const SensorHumid &other) : Sensor(other) {
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_humidities[i] = other._humidities[i];
	}
	_humidity = other._humidity;
}

SensorHumid& SensorHumid::operator =(const SensorHumid &other) {
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_humidities[i] = other._humidities[i];
	}
	_humidity = other._humidity;
	return *this;
}

SensorHumid::~SensorHumid() {}

Sensor::SensName SensorHumid::getType() const {
	return Sensor::Humidity;
}

void SensorHumid::init() {
	pinMode(_pin, INPUT);
}

void SensorHumid::update() {
	_humidities[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	smooth();
}

void SensorHumid::fastUpdate() {
	uint8_t h = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_humidities[i] = h;
	}
	smooth();

}

uint8_t SensorHumid::get() const {
	return _humidity;
}

uint8_t SensorHumid::getRaw() const {
	if (DHT11.read(humidIn) == DHTLIB_OK)
		return DHT11.humidity;
	else
		return 0;
}

void SensorHumid::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _humidities[i]; }
	_humidity = (uint8_t)(res / _numSamples);
}
