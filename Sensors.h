// #############################################################################
//
// # Name       : Sensors
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 21.11.2014
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
#include <Arduino.h>
#include <DHT11.h>
#include <DallasTemperature.h>
#include <DS1307RTC.h>
#include <Time.h>

//Pin numbers
// 16 & 17 are Serial2 Tx,Rx used for EC circuit
// 18 & 19 are Serial1 Tx,Rx used for PH circuit
extern const int humidIn;
extern const int lightIn;
extern const int tempIn;
extern const int waterEcho;
extern const int waterTrigger;

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
    uint8_t getLight() const;
    uint8_t getHumidity() const;
    uint16_t getEC() const;
    float getPH() const;
    uint8_t getWaterLevel() const;
	//Poll sensor and get raw data
	uint16_t getRawWaterLevel();
	uint16_t getRawLight();
	//Tests
	boolean ecOffRange();
	boolean phOffRange();
	boolean lvlOffRange();
    //Updates sample arrays with readings from sensors and smoothes data
    void update();
    //Adjusts EC sensor readings to temperature
    void adjustECtemp();
	//Adds a reading to a graph
	void storeGraphPoint();
	//pH Calibration
	/*void setPHcontinuous();
	void setPHstandby();
	void setPHfour();
	void setPHseven();
	void setPHten();
	//EC Calibration
	void setECcontinuous();
	void setECstandby();
	void setECprobeType();
	void setECdry();
	void setECtenThousand();
	void setECfortyThousand();*/
	//RTC adjustment
	void setRTCtime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, int y);

  private:
	Settings *_settings;
	
    //Smoothes readings
    void smoothSensorReadings();
    //These poll hardware and return sensor info
    uint8_t light();
    float temp();
    uint8_t humidity();
    uint8_t waterLevel();
    float ph();
    uint16_t ec();
	// Used for smoothing sensor data.  The higher the number,
	// the more the readings will be smoothed, but the slower the variables will
	// respond to the input.
	static const int _numSamples = 10;
    //Smoothing counter
    uint8_t _iSample;
    //Contain sensor data pre-smoothing
    float _temps[_numSamples];
    uint8_t _lights[_numSamples]; 
    uint8_t _humidities[_numSamples];
    uint16_t _ecs[_numSamples];
    float _phs[_numSamples];
    uint8_t _waterLevels[_numSamples];
    //Contain sensor values post smoothing
    float _temp;
    uint8_t _light;
    uint8_t _humidity;
    uint16_t _ec;
    float _ph;
    uint8_t _waterLevel;
};
#endif
/*
template <class T>; 
class Sensor {
	public:
		//Constructors
		Sensor(Settings *_settings, const int pin);
		Sensor(const Sensor &other);
		Sensor& operator=(const Sensor &other);
		//Destructor
		virtual ~Sensor();
		virtual void init();
		virtual T get() const;	
		virtual void update();	
		//Idea: return false if more than x time passes and no data to auto-disable reservoir module if not present
		virtual boolean isResponsive();
		
	protected:
		int _pin;
		//TODO:To include here Graphic class should only care for data and draw() method there should go to Window class
		Graphic<T> _graph;
		// Used for smoothing sensor data.  The higher the number,
		// the more the readings will be smoothed, but the slower the variables will
		// respond to the input.
		static const uint8_t _numSamples = 10;
		//Smoothing counter
		uint8_t _iSample;
		//Contain sensor data pre-smoothing
		T _raw[_numSamples];
		//Contain sensor values post smoothing
		T _value;
		//Poll hardware, return reading
		T poll();
		//Smooth reading, update _value
		void smooth();
};

class sensorTemp public Sensor<uint8_t> {
}

*/