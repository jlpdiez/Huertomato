// #############################################################################
//
// # Name       : SensorLight
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 07.11.2015
//
// # Description: Photoresistor light sensor class
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


#ifndef SENSORLIGHT_H_
#define SENSORLIGHT_H_

#include "Sensor.h"

//extern const uint8_t lightIn;

class SensorLight: public Sensor {
	public:
		SensorLight(const int pin);
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
		//Data array
		uint16_t _lights[_numSamples];
		//Value post-smoothing
		uint16_t _light;
		
		void smooth();
};

#endif
