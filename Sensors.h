// #############################################################################
//
// # Name       : Sensors
// # Version    : 2.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 17.12.2015
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

//Pin numbers are previously declared in Huertomato.ino
extern const uint8_t humidIn;
extern const uint8_t lightIn;
extern const uint8_t waterEcho;
extern const uint8_t waterTrigger;

//Contains all other instances of sensor classes
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
    Sensors(Settings *settings);
	Sensors(const Sensors &other);
	Sensors& operator=(const Sensors &other);
	//Destructor
	~Sensors(); 
    //Getters
    float getTemp() const;
    uint16_t getLight() const;
    uint8_t getHumidity() const;
    float getEC() const;
    float getPH() const;
    uint8_t getWaterLevel() const;
	//Poll sensor and get raw data
	uint16_t getRawWaterLevel();
	uint16_t getRawLightLevel();
	//Setters
	//Sets different modes. Should be called when settings counterpart gets called.
	void setSerialDebug(boolean);
	void setReservoir(boolean);
	//Set temp mode. Should be called when it's counterpart in setting does too.
	void setCelsius(boolean);
	//Sets min and max waterLevel. Used for calculating % on water tank
	void setMaxLvl(uint16_t);
	void setMinLvl(uint16_t);
	//Alarm tests - True if sensor value is off range.
	boolean ecOffRange();
	boolean phOffRange();
	boolean lvlOffRange();
    //Updates sample arrays with readings from sensors and smoothes data
    void update();
	//Reads once from each sensor, fills the array with this measurement and smoothes
	void fastUpdate();
	
	//Adjust pH readings to temperature
	void adjustPHtemp();

  private:
	Settings *_settings;
	
	SensorEC _ec;
	SensorHumid _humidity;
	SensorLight _light;
	SensorPH _ph;
	SensorTemp _temp;
	SensorWater _water;
	
	//Keeps track of reservoir activation
	boolean _reservoir;

};

#endif
