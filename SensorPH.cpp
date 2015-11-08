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

void SensorPH::init() {
	//Open communication with PH sensor
	Serial2.begin(38400);
	//Set it into reading on-demand mode
	Serial2.print("E\r");
	//Make a reading or fastUpdate() won't work
	Serial2.print("R\r");
}

void SensorPH::update() {
	//if (_settings->getReservoirModule()) {
		_phs[_iSample] = getRaw();
		_iSample++;
		if (_iSample >= _numSamples)
			_iSample = 0;
		smooth();
	//}
}

void SensorPH::fastUpdate() {
	//if (_settings->getReservoirModule()) {
		float p = getRaw();
		for (uint8_t i = 0; i < _numSamples; i++) {
			_phs[i] = p;
		}
		smooth();
	//}

}

float SensorPH::get() const {
	return _ph;
}

//Returns a PH reading. Reading takes 378ms
float SensorPH::getRaw() const {
	if (!_calibratingPh) {
		Serial2.print("R\r");
		if (Serial2.available() > 0) {
			float res = Serial2.parseFloat();
			//Discard carriage return '/r'
			Serial2.read();
			return res;
		}
		//There's some kind of problem communicating with circuit
		return 0;
	} else
	return _ph;
}

/*boolean SensorPH::phOffRange() {
	if ((_ph < _settings->getPHalarmDown()) || (_ph > _settings->getPHalarmUp()))
		return true;
	return false;
}*/

/** TO DO
//This should be set while calibrating to prevent messing up circuits if update() called
void calibratingPH(boolean c);
//pH circuit commands
void resetPH();
void getPHinfo();
void setPHled(boolean);
void setPHcontinuous();
void setPHstandby();
void setPHfour();
void setPHseven();
void setPHten();
//Adjust pH readings to temperature
void adjustPHtemp();
*/

void SensorPH::smooth() {
	float res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _phs[i]; }
	_ph = (float)(res / _numSamples);
}

/* TO-DO
//Clear incoming buffer
void clearPHbuffer();
//Output pH circuit's response to serial
void phToSerial();
*/