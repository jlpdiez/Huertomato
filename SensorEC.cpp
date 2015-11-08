#include "SensorEC.h"

#include "SensorLight.h"

SensorEC::SensorEC(const int pin)
: Sensor(pin) {

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
	//if (_settings->getReservoirModule()) {
		_ecs[_iSample] = getRaw();
		_iSample++;
		if (_iSample >= _numSamples)
		_iSample = 0;
		smooth();
	//}
}

void SensorEC::fastUpdate() {
	//if (_settings->getReservoirModule()) {
		uint16_t e = getRaw();
		for (uint8_t i = 0; i < _numSamples; i++) {
			_ecs[i] = e;
		}
		smooth();
	//}

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

//This should be set while calibrating to prevent messing up circuits if update() called
void SensorEC::calibratingEC(boolean c) {
	_calibratingEc = c;
}

//TODO: Fix Serial logic
void SensorEC::resetEC() {
	clearECbuffer();
	Serial1.print("X\r");
	//Give time for reset
	delay(2750);
	//ecToSerial();
}

void SensorEC::getECinfo() {
	clearECbuffer();
	Serial1.print("I\r");
	delay(1450);
	//ecToSerial();	
}

void SensorEC::setECled(boolean state) {
	if (state)
		Serial1.print("L1\r");
	else
		Serial1.print("L0\r");	
}

void SensorEC::setECcontinuous() {
	Serial1.print("C\r");	
}

void SensorEC::setECstandby() {
	Serial1.print("E\r");	
}

void SensorEC::setECprobeType() {
	Serial1.print("P,2\r");
	//if (_settings->getSerialDebug())
	//	Serial.println("k1.0");
}

void SensorEC::setECdry() {
	Serial1.print("Z0\r");
// 	if (_settings->getSerialDebug())
// 		Serial.println("dry cal");	
}

void SensorEC::setECtenThousand() {
	Serial1.print("Z10\r");
// 	if (_settings->getSerialDebug())
// 		Serial.println("10,500 uS cal");
}

void SensorEC::setECfortyThousand() {
	Serial1.print("Z40\r");
// 	if (_settings->getSerialDebug())
// 		Serial.println("40,000 uS cal");
}

//TODO: Breaks encapsulation using temperature!!!
//Adjusts EC sensor readings to temperature
void SensorEC::adjustECtemp() {
	float tempt = temperature.getTempCByIndex(0);
	//Serial.print(tempt);
	if ((tempt != 0) && (!_calibratingEc)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(tempt,4,2,tempArray);
		String command = (String)tempArray + ",C\r";
		Serial1.print(command);
	}
}

void SensorEC::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _ecs[i]; }
	_ec = (uint16_t)(res / _numSamples);
}


//Clears incoming buffers
void SensorEC::clearECbuffer() {
	while (Serial1.available() > 0)
		Serial1.read();	
}

//TODO: Reimplement
//Output EC circuit's response to serial
void SensorEC::ecToSerial() {
	/*if (_settings->getSerialDebug()) {
		if (Serial1.available() > 0) {
			String sensorString = "";
			sensorString.reserve(30);
			//Read data from sensor
			char inchar;
			while (Serial1.peek() != '\r') {
				inchar = (char)Serial1.read();
				sensorString += inchar;
			}
			//Discard <CR>
			Serial1.read();
			Serial.println(sensorString);
		}
	}*/
}
