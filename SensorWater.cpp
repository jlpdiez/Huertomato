#include "SensorWater.h"

SensorWater::SensorWater(const int pinTrigger, const int pinEcho)
: Sensor(0), _pinTrigger(pinTrigger), _pinEcho(pinEcho) {
	
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = 0;
	}
}

SensorWater::SensorWater(const SensorWater &other) : Sensor(other) {
	_pinTrigger = other._pinTrigger;
	_pinEcho = other._pinEcho;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = other._waterLevels[i];
	}
	_waterLevel = other._waterLevel;
}

SensorWater& SensorWater::operator =(const SensorWater &other) {
	_pinTrigger = other._pinTrigger;
	_pinEcho = other._pinEcho;
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_waterLevels[i] = other._waterLevels[i];
	}
	_waterLevel = other._waterLevel;
	return *this;
}

SensorWater::~SensorWater() {}

Sensor::SensName SensorWater::getType() const {
	return Sensor::Level;
}

void SensorWater::init() {
	pinMode(_pinTrigger, OUTPUT);
	pinMode(_pinEcho, INPUT);
}

void SensorWater::update() {
	//if (_settings->getReservoirModule()) {
		_waterLevels[_iSample] = getRaw();
		_iSample++;
		if (_iSample >= _numSamples)
		_iSample = 0;
		smooth();
	//}
}

void SensorWater::fastUpdate() {
	//if (_settings->getReservoirModule()) {
		uint8_t w = getRaw();
		for (uint8_t i = 0; i < _numSamples; i++) {
			_waterLevels[i] = w;
		}
		smooth();
	//}

}

uint8_t SensorWater::get() const {
	return _waterLevel;
}

/** PROBLEM! - Normal and raw differ!
//Returns water reservoir % level
uint8_t Sensors::waterLevel() {
	int duration, distance;
	digitalWrite(waterTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(waterTrigger, HIGH);
	delayMicroseconds(5);
	digitalWrite(waterTrigger, LOW);
	//Calc duration. Timeout of half second
	duration = pulseIn(waterEcho, HIGH,500000);
	distance = (duration / 29) / 2;
	//Serial << "Distance: " << distance << endl;
	distance = constrain(distance, _settings->getMaxWaterLvl(), _settings->getMinWaterLvl());
	return map(distance, _settings->getMaxWaterLvl(), _settings->getMinWaterLvl(), 100, 0);
}
**/

uint8_t SensorWater::getRaw() const {
	digitalWrite(_pinTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrigger, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pinTrigger, LOW);

	return (pulseIn(_pinEcho, HIGH) / 29) / 2;
}

/*boolean SensorWater::lvlOffRange() {
	if (_waterLevel < _settings->getWaterAlarm())
		return true;
	return false;
}*/

void SensorWater::smooth() {
	uint8_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _waterLevels[i]; }
	_waterLevel = (uint8_t)(res / _numSamples);
}