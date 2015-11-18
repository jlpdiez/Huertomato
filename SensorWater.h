// #############################################################################
//
// # Name       : SensorWater
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 18.11.2015
//
// # Description: HC-SR04 sonar sensor class
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

#ifndef SENSORWATER_H_
#define SENSORWATER_H_

#include "Sensor.h"
#include <NewPing.h>

class SensorWater: public Sensor {
	public:
		SensorWater(const int pinTrigger, const int pinEcho);
		SensorWater(const SensorWater&);
		SensorWater& operator=(const SensorWater&);
		~SensorWater();
		
		SensName getType() const;
		void init();
		void update();
		void fastUpdate();
		uint8_t get() const;
		uint16_t getRaw();
		uint8_t getPercent();
		void setMax(uint16_t);
		void setMin(uint16_t);
	
	protected:
		int _pinTrigger;
		int _pinEcho;
		//Sonar object
		NewPing _sonar;
		//Holds min and max distance to get percent
		uint16_t _max;
		uint16_t _min;
		//Smoothing counter
		uint8_t _iSample;
		//Data array
		uint8_t _waterLevels[_numSamples];
		//Value post-smoothing
		uint8_t _waterLevel;
	
		void smooth();
};


#endif