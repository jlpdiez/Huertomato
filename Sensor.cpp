#include "Sensor.h"

//Constructors
Sensor::Sensor(const int pin) : _pin(pin) {}

Sensor::Sensor(const Sensor &other) {
	_pin = other._pin;
}

const Sensor& Sensor::operator=(const Sensor &other) {
	_pin = other._pin;
	return *this;
}

//Destructor
Sensor::~Sensor() {}

Sensor::SensName Sensor::getType() const {
 	return Sensor::None;
}