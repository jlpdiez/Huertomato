#include "SensorPH.h"

SensorPH::SensorPH(const int pin)
: Sensor(pin) {
	_serialDbg = false;
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_phs[i] = 0;
	}
}

SensorPH::SensorPH(const SensorPH &other) : Sensor(other) {
	_serialDbg = other._serialDbg;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_phs[i] = other._phs[i];
	}
	_ph = other._ph;
}

SensorPH& SensorPH::operator =(const SensorPH &other) {
	_serialDbg = other._serialDbg;
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
	Serial.print("RAW: ");
	Serial.print(raw);
	//Analog to mV
	float phValue = (float)raw*5.0/1024/6;
	Serial.print(" | mV: ");
	Serial.print(phValue);
	//mV to pH
	phValue = phValue * 3.5 + offSet;
	Serial.print(" | pH: ");
	Serial.println(phValue);
	return phValue;
}

/*void SensorPH::setSerialDebug(boolean s) {
	_serialDbg = s;
}*/

//This should be set while calibrating to prevent messing up circuits if update() called
/*void SensorPH::calibrating(boolean c) {
	_calibratingPh = c;	
}

//pH circuit commands
void SensorPH::reset() {
	clearBuffer();
	Serial2.print("X\r");
	//Leave some time for circuit reset and for data to be received
	delay(100);
	phToSerial();	
}

void SensorPH::getInfo() {
	clearBuffer();
	Serial2.print("I\r");
	//Leave some time for data to be received
	delay(10);
	phToSerial();
}

void SensorPH::setLed(boolean state) {
	if (state)
		Serial2.print("L1\r");
	else
		Serial2.print("L0\r");	
}

void SensorPH::setContinuous() {
	Serial2.print("C\r");	
}

void SensorPH::setStandby() {
	Serial2.print("E\r");	
}

void SensorPH::setFour() {
	Serial2.print("F\r");
 	if (_serialDbg)
 		Serial.println(4.00);	
}

void SensorPH::setSeven() {
 	Serial2.print("S\r");
 	if (_serialDbg)
 		Serial.println(7.00);	
}

void SensorPH::setTen() {
	Serial2.print("T\r");
 	if (_serialDbg)
 		Serial.println(10.00);	
}*/

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

//Clear incoming buffer
/*void SensorPH::clearBuffer() {
 	while (Serial2.available() > 0)
 		Serial2.read();
}*/

//Output pH circuit's response to serial
/*void SensorPH::phToSerial() {
	if (_serialDbg) {
		if (Serial2.available() > 0) {
			String sensorString = "";
			sensorString.reserve(30);
			//Read data from sensor
			char inchar;
			while (Serial2.peek() != '\r') {
				inchar = (char)Serial2.read();
				sensorString += inchar;
			}
			//Discard <CR>
			Serial2.read();
			Serial.println(sensorString);
		}
	}
}*/
