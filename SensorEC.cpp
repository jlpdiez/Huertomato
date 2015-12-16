#include "SensorEC.h"

#include "SensorLight.h"

SensorEC::SensorEC(const int pin)
: Sensor(pin) {
	_serialDbg = false;
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = 0;
	}
	//_calibratingEc = false;
}

SensorEC::SensorEC(const SensorEC &other) : Sensor(other) {
	_serialDbg = other._serialDbg;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = other._ecs[i];
	}
	_ec = other._ec;
	//_calibratingEc = false;
}

SensorEC& SensorEC::operator =(const SensorEC &other) {
	_serialDbg = other._serialDbg;
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

void SensorEC::init() {}

void SensorEC::update() {
	_ecs[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	smooth();
}

void SensorEC::fastUpdate() {
	uint16_t e = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_ecs[i] = e;
	}
	smooth();
}

uint16_t SensorEC::get() const {
	return _ec;
}

//Returns EC in uSiemens. Reading takes 1000ms
uint16_t SensorEC::getRaw() const {
	uint16_t raw = analogRead(ecPin);
	Serial.print("RAW: ");
	Serial.print(raw);
	uint16_t rawVolt = raw*(float)5000/1024;
	Serial.print(" | mV: ");
	Serial.print(rawVolt);
	//float tempCoef = 1.0 + 0.0185*(sensors.getTemp()-25.0);
	
}

/*void SensorEC::setSerialDebug(boolean s) {
	_serialDbg = s;
}

//This should be set while calibrating to prevent messing up circuits if update() called
void SensorEC::calibrating(boolean c) {
	_calibratingEc = c;
}

void SensorEC::reset() {
	clearECbuffer();
	Serial1.print("X\r");
	//Give time for reset
	delay(2750);
	ecToSerial();
}

void SensorEC::getInfo() {
	clearECbuffer();
	Serial1.print("I\r");
	delay(1450);
	ecToSerial();	
}

void SensorEC::setLed(boolean state) {
	if (state)
		Serial1.print("L1\r");
	else
		Serial1.print("L0\r");	
}

void SensorEC::setContinuous() {
	Serial1.print("C\r");	
}

void SensorEC::setStandby() {
	Serial1.print("E\r");	
}

void SensorEC::setProbeType() {
	Serial1.print("P,2\r");
	if (_serialDbg)
		Serial.println("k1.0");
}

void SensorEC::setDry() {
	Serial1.print("Z0\r");
 	if (_serialDbg)
 		Serial.println("dry cal");	
}

void SensorEC::setTenThousand() {
	Serial1.print("Z10\r");
 	if (_serialDbg)
 		Serial.println("10,500 uS cal");
}

void SensorEC::setFortyThousand() {
	Serial1.print("Z40\r");
 	if (_serialDbg)
 		Serial.println("40,000 uS cal");
}
*/
//Adjusts EC sensor readings to given temperature
void SensorEC::adjustTemp(float tempt) {
	/*if ((tempt != 0) && (!_calibratingEc)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(tempt,4,2,tempArray);
		String command = (String)tempArray + ",C\r";
		Serial1.print(command);
	}*/
}

void SensorEC::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _ecs[i]; }
	_ec = (uint16_t)(res / _numSamples);
}


//Clears incoming buffers
/*void SensorEC::clearECbuffer() {
	while (Serial1.available() > 0)
		Serial1.read();	
}

//Output EC circuit's response to serial
void SensorEC::ecToSerial() {
	if (_serialDbg) {
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
	}
}*/
