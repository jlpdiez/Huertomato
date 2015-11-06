/*
 * Sensor.cpp
 *
 * Created: 11/6/2015 7:29:16 PM
 *  Author: HAL
 */ 

#include "Sensor.h"

//Constructors
Sensor::Sensor(Settings *settings, const int pin) : _settings(settings), _pin(pin) {}

Sensor::Sensor(const Sensor &other) {
	_settings = other._settings;
	_pin = other._pin;
}

const Sensor& Sensor::operator=(const Sensor &other) {
	_settings = other._settings;
	_pin = other._pin;
	return *this;
}

//Destructor
Sensor::~Sensor() {}

Sensor::SensName Sensor::getType() const {
 	return Sensor::None;
}