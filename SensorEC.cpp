#include "SensorEC.h"

#include "SensorLight.h"

SensorEC::SensorEC(Settings *settings, const int pin)
: Sensor(settings,pin) {

	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = 0;
	}
	_calibratingEc = false;
}

SensorEC::SensorEC(const SensorEC &other) : Sensor(other) {
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = other._ecs[i];
	}
	_ec = other._ec;
	_calibratingEc = false;
}

SensorEC& SensorEC::operator =(const SensorEC &other) {
	_settings = other._settings;
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
	//Open communication
	Serial1.begin(38400);
	Serial1.print("E\r");
	//Set to continuous mode (needs 20-25 readings of 1000ms to stabilize reading)
	Serial1.print("C\r");
}

void SensorEC::update() {
	if (_settings->getReservoirModule()) {
		_ecs[_iSample] = getRaw();
		_iSample++;
		if (_iSample >= _numSamples)
		_iSample = 0;
		smooth();
	}
}

void SensorEC::fastUpdate() {
	if (_settings->getReservoirModule()) {
		uint16_t e = getRaw();
		for (uint8_t i = 0; i < _numSamples; i++) {
			_ecs[i] = e;
		}
		smooth();
	}

}

uint16_t SensorEC::get() const {
	return _ec;
}

//Returns EC in uSiemens. Reading takes 1000ms
uint16_t SensorEC::getRaw() const {
	//If sensors being calibrated we return previous values
	if (!_calibratingEc) {
		if (Serial1.available() > 0) {
			uint16_t res = Serial1.parseInt();
			//Clear buffer of remaining messages
			while (Serial1.available() > 0)
			Serial1.read();
			return res;
		}
		//Buffer has been emptied before and circuit still hasn't put data into it again
		return 0;
	} else
	return _ec;
}

boolean SensorEC::ecOffRange() {
	if ((_ec < _settings->getECalarmDown()) || (_ec > _settings->getECalarmUp()))
		return true;
	return false;
}

/** TO DO:
void resetEC();
void getECinfo();
void setECled(boolean);
void setECcontinuous();
void setECstandby();
void setECprobeType();
void setECdry();
void setECtenThousand();
void setECfortyThousand();
//Adjusts EC sensor readings to temperature
void adjustECtemp();
//This should be set while calibrating to prevent messing up circuits if update() called
void calibratingEC(boolean c);
*/

void SensorEC::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _ecs[i]; }
	_ec = (uint16_t)(res / _numSamples);
}

/** TO DO
//Clears incoming buffers
void clearECbuffer();
//Output EC circuit's response to serial
void ecToSerial();
*/
