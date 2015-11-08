// #############################################################################
//
// # Name       : SensorTemp
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 07.11.2015
//
// # Description: DS18B20 temperature sensor class
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


#ifndef SENSORTEMP_H_
#define SENSORTEMP_H_

#include "Sensor.h"

extern DallasTemperature temperature;
//TODO: Externally setCelsius should be called to change mode
class SensorTemp: public Sensor {
	public:
		SensorTemp(const int pin = 0);
		SensorTemp(const SensorTemp&);
		SensorTemp& operator=(const SensorTemp&);
		~SensorTemp();
	
		SensName getType() const;
		void init();
		void update();
		void fastUpdate();
		float get() const;
		float getRaw() const;
		void setCelsius(boolean);
	
	protected:
		//Celsius mode if true
		boolean _celss;
		//Smoothing counter
		uint8_t _iSample;
		//Data array
		float _temps[_numSamples];
		//Value post-smoothing
		float _temp;
	
		void smooth();
};

#endif