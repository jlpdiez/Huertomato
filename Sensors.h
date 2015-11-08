// #############################################################################
//
// # Name       : Sensors
// # Version    : 1.8
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.10.2015
// 
// # Description: Library for managing Huertomato's sensors
// # In charge of polling all hardware and smoothing values afterwards
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

#ifndef SENSORS_H
#define SENSORS_H

#include "Settings.h"
#include "Sensor.h"
#include "SensorEC.h"
#include "SensorHumid.h"
#include "SensorLight.h"
#include "SensorPH.h"
#include "SensorTemp.h"
#include "SensorWater.h"
#include <Arduino.h>
#include <DHT11.h>
#include <DallasTemperature.h>


//Pin numbers
// 16 & 17 are Serial2 Tx,Rx used for pH circuit
// 18 & 19 are Serial1 Tx,Rx used for EC circuit
extern const uint8_t humidIn;
extern const uint8_t lightIn;
extern const uint8_t tempIn;
extern const uint8_t waterEcho;
extern const uint8_t waterTrigger;

extern DallasTemperature temperature;
extern dht11 DHT11;

//Class that handles sensor reading & smoothing
class Sensors {
  public:
	enum Sensor {
		None,
		Temperature,
		Humidity,
		Light,
		Ec,
		Ph,
		Level
	};
    //Constructors
    Sensors(Settings *_settings);
	Sensors(const Sensors &other);
	Sensors& operator=(const Sensors &other);
	//Destructor
	~Sensors(); 
    //Getters
    float getTemp() const;
    uint16_t getLight() const;
    uint8_t getHumidity() const;
    uint16_t getEC() const;
    float getPH() const;
    uint8_t getWaterLevel() const;
	//Poll sensor and get raw data
	uint16_t getRawWaterLevel();
	uint16_t getRawLightLevel();
	//Tests
	boolean ecOffRange();
	boolean phOffRange();
	boolean lvlOffRange();
    //Updates sample arrays with readings from sensors and smoothes data
    void update();
	//Reads once from each sensor, fills the array with this measurement and smoothes
	void fastUpdate();
	
	//This should be set while calibrating to prevent messing up circuits if update() called
	void calibratingPH(boolean c);
	void calibratingEC(boolean c);
	//pH circuit commands
	void resetPH();
	void getPHinfo();
	void setPHled(boolean);
	void setPHcontinuous();
	void setPHstandby();
	void setPHfour();
	void setPHseven();
	void setPHten();
	//Adjust pH readings to temperature
	void adjustPHtemp();
	//EC circuit commands
	void resetEC();
	void getECinfo();
	void setECled(boolean);
	void setECcontinuous();
	void setECstandby();
	void setECprobeType();
	void setECdry();
	void setECtenThousand();
	void setECfortyThousand();
	//Adjusts EC sensor readings to temperature
	void adjustECtemp();

  private:
	const Sensor *_sensores[6];
	Settings *_settings;
	//To stop EC & pH routines if sensors are being calibrated
	boolean _calibratingPh;
	boolean _calibratingEc;
	
    //Smoothes readings
    void smoothSensorReadings();
    //These poll hardware and return sensor info
    uint16_t light();
    float temp();
    uint8_t humidity();
    uint8_t waterLevel();
    float ph();
    uint16_t ec();
	
	//Clears incoming buffers
	void clearPHbuffer();
	void clearECbuffer();
	//Output EC circuit's response to serial
	void ecToSerial();
	void phToSerial();
	
	// Used for smoothing sensor data.  The higher the number,
	// the more the readings will be smoothed, but the slower the variables will
	// respond to the input.
	static const uint8_t _numSamples = 10;
    //Smoothing counter
    uint8_t _iSample;
    //Contain sensor data pre-smoothing
    float _temps[_numSamples];
    uint16_t _lights[_numSamples]; 
    uint8_t _humidities[_numSamples];
    uint16_t _ecs[_numSamples];
    float _phs[_numSamples];
    uint8_t _waterLevels[_numSamples];
    //Contain sensor values post smoothing
    float _temp;
    uint16_t _light;
    uint8_t _humidity;
    uint16_t _ec;
    float _ph;
    uint8_t _waterLevel;
};

#endif
