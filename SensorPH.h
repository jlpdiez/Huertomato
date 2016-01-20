// #############################################################################
//
// # Name       : SensorPH
// # Version    : 0.5
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 17.12.2015
//
// # Description: DFRobot pH sensor class
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

#ifndef SENSORPH_H_
#define SENSORPH_H_

#include "Sensor.h"

//extern const uint8_t phPin;
const float offSet = 0.09;

class SensorPH: public Sensor {
	public:
		SensorPH(const int pin = 0);
		SensorPH(const SensorPH&);
		SensorPH& operator=(const SensorPH&);
		~SensorPH();
	
		SensName getType() const;
		void init();
		void update();
		void fastUpdate();
		float get() const;
		//Returns a PH reading. Reading takes 378ms
		float getRaw() const;
		//Adjust pH readings to given temperature
		void adjustTemp(float);
	
	protected:
		//Smoothing counter
		uint8_t _iSample;
		//Data array
		float _phs[_numSamples];
		//Value post-smoothing
		float _ph;
	
		void smooth();
};

#endif