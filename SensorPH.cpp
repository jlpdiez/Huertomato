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


//This should be set while calibrating to prevent messing up circuits if update() called
void SensorPH::calibratingPH(boolean c) {
	_calibratingPh = c;	
}

//pH circuit commands
void SensorPH::resetPH() {
	clearPHbuffer();
	Serial2.print("X\r");
	//Leave some time for circuit reset and for data to be received
	delay(100);
	//phToSerial();	
}

void SensorPH::getPHinfo() {
	clearPHbuffer();
	Serial2.print("I\r");
	//Leave some time for data to be received
	delay(10);
	//phToSerial();
}

void SensorPH::setPHled(boolean state) {
	if (state)
		Serial2.print("L1\r");
	else
		Serial2.print("L0\r");	
}

void SensorPH::setPHcontinuous() {
	Serial2.print("C\r");	
}

void SensorPH::setPHstandby() {
	Serial2.print("E\r");	
}

void SensorPH::setPHfour() {
	Serial2.print("F\r");
// 	if (_settings->getSerialDebug())
// 		Serial.println(4.00);	
}

void SensorPH::setPHseven() {
// 	Serial2.print("S\r");
// 	if (_settings->getSerialDebug())
// 		Serial.println(7.00);	
}

void SensorPH::setPHten() {
	Serial2.print("T\r");
// 	if (_settings->getSerialDebug())
// 		Serial.println(10.00);	
}

//Adjust pH readings to temperature
void SensorPH::adjustPHtemp() {
	float tempt = temperature.getTempCByIndex(0);
	//Serial.print(tempt);
	if ((tempt != 0) && (!_calibratingPh)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(tempt,4,2,tempArray);
		String command = (String)tempArray + "\r";
		Serial2.print(command);
	}	
}

void SensorPH::smooth() {
	float res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _phs[i]; }
	_ph = (float)(res / _numSamples);
}

//Clear incoming buffer
void SensorPH::clearPHbuffer() {
 	while (Serial2.available() > 0)
 		Serial2.read();
}

//Output pH circuit's response to serial
void SensorPH::phToSerial() {
	/*if (_settings->getSerialDebug()) {
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
	}*/	
}
