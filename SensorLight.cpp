#include "SensorLight.h"

SensorLight::SensorLight(Settings *settings, const int pin)
: Sensor(settings,pin) {
	
	_iSample = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_lights[i] = 0;
	}
}
	
SensorLight::SensorLight(const SensorLight &other) : Sensor(other) {
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_lights[i] = other._lights[i];
	}
	_light = other._light;
}

SensorLight& SensorLight::operator =(const SensorLight &other) {
	_settings = other._settings;
	_pin = other._pin;
	_iSample = other._iSample;
	for (uint8_t i = 0; i < _numSamples; i++) {
		_lights[i] = other._lights[i];
	}
	_light = other._light;
	return *this;
}

SensorLight::~SensorLight() {}
	
Sensor::SensName SensorLight::getType() const {
	return Sensor::Light;
}

void SensorLight::init() {
	pinMode(_pin, INPUT);
}

void SensorLight::update() {
	_lights[_iSample] = getRaw();
	_iSample++;
	if (_iSample >= _numSamples)
	_iSample = 0;
	smooth();
}

void SensorLight::fastUpdate() {
	uint16_t l = getRaw();
	for (uint8_t i = 0; i < _numSamples; i++) {
		_lights[i] = l;
	}
	smooth();

}
	
uint16_t SensorLight::get() const { 
	return _light; 
}
	
uint16_t SensorLight::getRaw() const {
	//http://forum.arduino.cc/index.php?topic=141815.0
	const float vcc = 4.3;
	const int resInKohm = 10.0;
	int adc = analogRead(_pin);
	//Vout = Output voltage from potential Divider. [Vout = adc * (Vin / 1024)]
	float vo = adc * (vcc / 1024);
	//Equation to calculate Resistance of LDR, [R-LDR =(R1 (Vin - Vout))/ Vout]
	float rldr = (resInKohm * (vcc - vo)) / vo;
	uint16_t luxes = (500 / rldr);
	return luxes;
}

void SensorLight::smooth() {
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _lights[i]; }
	_light = (uint16_t)(res / _numSamples);
}
