/*
 * SensorLight.h
 *
 * Created: 11/6/2015 6:26:32 PM
 *  Author: HAL
 */ 


#ifndef SENSORLIGHT_H_
#define SENSORLIGHT_H_

#include "Sensor.h"

//extern const uint8_t lightIn;

class SensorLight: public Sensor {
	public:
		SensorLight(Settings *settings, const int pin);
		SensorLight(const SensorLight&);
		SensorLight& operator=(const SensorLight&);
		~SensorLight();
		
		SensName getType() const;
		void init();
		void update();
		void fastUpdate();
		uint16_t get() const;
		uint16_t getRaw() const;
		
	protected:
		//Smoothing counter
		uint8_t _iSample;
	
		uint16_t _lights[_numSamples];
		uint16_t _light;
		
		void smooth();
};

#endif
