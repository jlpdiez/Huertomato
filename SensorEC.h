// #############################################################################
//
// # Name       : SensorEC
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 17.11.2015
//
// # Description: Atlas Scientific EC sensor class
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


#ifndef SENSOREC_H_
#define SENSOREC_H_

#include "Sensor.h"

class SensorEC: public Sensor {
	public:
		SensorEC(const int pin = 0);
		SensorEC(const SensorEC&);
		SensorEC& operator=(const SensorEC&);
		~SensorEC();
	
		SensName getType() const;
		void init();
		void update();
		void fastUpdate();
		uint16_t get() const;
		//Returns EC in uSiemens. Reading takes 1000ms
		uint16_t getRaw() const;
		//Sets output to serial
		void setSerialDebug(boolean);
		//This should be set while calibrating to prevent messing up circuits if update() called
		void calibrating(boolean c);
		//EC circuit commands
		void reset();
		void getInfo();
		void setLed(boolean);
		void setContinuous();
		void setStandby();
		void setProbeType();
		void setDry();
		void setTenThousand();
		void setFortyThousand();
		//Adjusts EC sensor readings to given temperature
		void adjustTemp(float);
	
	protected:
		boolean _serialDbg;
		//Stops EC routine if sensor is being calibrated
		boolean _calibratingEc;
		//Smoothing counter
		uint8_t _iSample;
		//Data array
		uint16_t _ecs[_numSamples];
		//Value post-smoothing
		uint16_t _ec;
	
		void smooth();
		
		//Clears incoming buffers
		void clearECbuffer();
		//Output EC circuit's response to serial
		void ecToSerial();
	
};

#endif