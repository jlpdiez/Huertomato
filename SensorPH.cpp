#include "SensorPH.h"

SensorPH::SensorPH(const int pin)
: Sensor(pin) {
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_phs[i] = 0;
	}
}

SensorPH::SensorPH(const SensorPH &other) : Sensor(other) {
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_phs[i] = other._phs[i];
	}
	_ph = other._ph;
}

SensorPH& SensorPH::operator =(const SensorPH &other) {
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_phs[i] = other._phs[i];
	}
	_ph = other._ph;
	return *this;
}

SensorPH::~SensorPH() {}

Sensor::SensName SensorPH::getType() const {
	return Sensor::Ph;
}

void SensorPH::init() {}

void SensorPH::update() {
	_phs[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	smooth();
}

void SensorPH::fastUpdate() {
	float p = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_phs[i] = p;
	}
	smooth();
}

float SensorPH::get() const {
	return _ph;
}

//Returns a PH reading.
float SensorPH::getRaw() const {
	uint16_t raw = analogRead(phPin);
	//Analog to mV
	float phValue = (float)raw*5.0/1024;
	//mV to pH
	phValue = phValue * 3.5 + offSet;
	//Return and make sure value is in range
	return constrain(phValue,0.0,14.0);
}

//Adjust pH readings to given temperature
void SensorPH::adjustTemp(float tempt) {
	/*if ((tempt != 0) && (!_calibratingPh)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(tempt,4,2,tempArray);
		String command = (String)tempArray + "\r";
		Serial2.print(command);
	}*/	
}

void SensorPH::smooth() {
	float res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _phs[i]; }
	_ph = (float)(res / _numSamples);
}