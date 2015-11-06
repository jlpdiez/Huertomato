/*
 * Sensor.h
 *
 * Created: 11/6/2015 6:28:51 PM
 *  Author: HAL
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include "Settings.h"
#include <Arduino.h>
#include <DHT11.h>
#include <DallasTemperature.h>
#include <DS1307RTC.h>
#include <Time.h>

//Pin numbers
// 16 & 17 are Serial2 Tx,Rx used for pH circuit
// 18 & 19 are Serial1 Tx,Rx used for EC circuit
extern const uint8_t humidIn;
//extern const uint8_t lightIn;
extern const uint8_t tempIn;
extern const uint8_t waterEcho;
extern const uint8_t waterTrigger;

extern DallasTemperature temperature;
extern dht11 DHT11;

//template <class TElem>
class Sensor {
	public:
		enum SensName {
			None,
			Temperature,
			Humidity,
			Light,
			Ec,
			Ph,
			Level
		};
		//Constructors
		Sensor(Settings *_settings, const int pin);
		Sensor(const Sensor&);
		const Sensor& operator=(const Sensor&);
		//Destructor
		virtual ~Sensor();
	
		virtual SensName getType() const = 0;
		virtual void init() = 0;
		//virtual TElem get() const = 0;
		virtual void update() = 0;
		virtual void fastUpdate() = 0;
		
		//There should also be a get() and getRaw() function for each one but as output variable type changes we can´t do anything
		//See: https://stackoverflow.com/questions/2354210/can-a-member-function-template-be-virtual
		
		//Idea: return false if more than x time passes and no data to auto-disable reservoir module if not present
		//virtual boolean isResponsive();
	
	protected:
		int _pin;
		// Used for smoothing sensor data.  The higher the number,
		// the more the readings will be smoothed, but the slower the variables will
		// respond to the input.
		static const uint8_t _numSamples = 10;
		//Smoothing counter
		//uint8_t _iSample;
		//Contain sensor data pre-smoothing
		//TElem _raw[_numSamples];
		//Contain sensor values post smoothing
		//TElem _value;

		//There should also be a addPollVal() function that polls sensor hardware and adds it's value to the array
		//But as output variable type changes we can´t declare it before
		//template <class TElem>
		//virtual TElem poll() = 0;
		
		//Smooth reading, update _value
		virtual void smooth() = 0;
	
		Settings *_settings;
};

#endif