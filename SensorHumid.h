// #############################################################################
//
// # Name       : SensorHumid
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 17.11.2015
//
// # Description: DHT11 humidity sensor class
//
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef SENSORHUMID_H_
#define SENSORHUMID_H_

#include "Sensor.h"
#include <DHT11.h>

extern dht11 DHT11;

class SensorHumid: public Sensor {
	public:
		SensorHumid(const int pin);
		SensorHumid(const SensorHumid&);
		SensorHumid& operator=(const SensorHumid&);
		~SensorHumid();
	
		SensName getType() const;
		void init();
		void update();
		void fastUpdate();
		uint8_t get() const;
		uint8_t getRaw() const;
	
	protected:
		//Smoothing counter
		uint8_t _iSample;
		//Data array
		uint8_t _humidities[_numSamples];
		//Value post-smoothing
		uint8_t _humidity;
	
		void smooth();
};

#endif