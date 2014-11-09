// #############################################################################
//
// # Name       : Settings
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 23.04.2014
//
// # Description: Settings class for Huertomato
// # Stores all the system's current settings. Its in charge of reading and storing in EEPROM 
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROMEx.h>

extern EEPROMClassEx EEPROM;

// *********************************************
class Settings {
  public:
	//Constructors
    Settings();
	Settings(const Settings &other);
	Settings& operator=(const Settings &other);
	//Destructor
	~Settings();
    
    //Setters - These store settings to EEPROM too
	void setDefault();
    //System Settings
    void setWaterTimed(const boolean);
    void setWaterHour(const uint8_t);
    void setWaterMinute(const uint8_t);
    void setFloodMinute(const uint8_t);
    void setPHalarmUp(const float);
    void setPHalarmDown(const float);
    void setECalarmUp(const uint16_t);
    void setECalarmDown(const uint16_t);
    void setWaterAlarm(const uint8_t);
    void setNightWatering(const boolean);
	void setLightThreshold(const uint16_t);
	void setMaxWaterLvl(const uint16_t);
	void setMinWaterLvl(const uint16_t);
	void setPumpProtectionLvl(const uint8_t);
    
    //Controller Settings
    void setSensorSecond(const uint8_t);
    void setSDactive(const boolean);
    void setSDhour(const uint8_t);
    void setSDminute(const uint8_t);
    void setSound(const boolean);
    void setSerialDebug(const boolean);
	void setReservoirModule(const boolean);
    
    //Status vars - These are not written to EEPROM
    void setNextWhour(const uint8_t);
    void setNextWminute(const uint8_t);
    void setNightWateringStopped(const boolean);
    void setWateringPlants(const boolean);
    void setAlarmTriggered(const boolean);
    
    //Getters
    //System Settings
    boolean getWaterTimed() const;
    uint8_t getWaterHour() const;
    uint8_t getWaterMinute() const;
    uint8_t getFloodMinute() const;
    float getPHalarmUp() const;
    float getPHalarmDown() const;
    uint16_t getECalarmUp() const;
    uint16_t getECalarmDown() const;
    uint8_t getWaterAlarm() const;
    boolean getNightWatering() const;
	uint16_t getLightThreshold() const;
	uint16_t getMaxWaterLvl() const;
	uint16_t getMinWaterLvl() const;
	uint8_t getPumpProtectionLvl() const;
    
    //Controller Settings
    uint8_t getSensorSecond() const;
    boolean getSDactive() const;
    uint8_t getSDhour() const;
    uint8_t getSDminute() const;
    boolean getSound() const;
    boolean getSerialDebug() const;
	boolean getReservoirModule() const;
    
    //Status vars
    uint8_t getNextWhour() const;
    uint8_t getNextWminute() const;
    boolean getNightWateringStopped() const;
    boolean getWateringPlants() const;
    boolean getAlarmTriggered() const;
	
	//These return value and go to false
	boolean systemStateChanged();
	boolean waterSettingsChanged();
	boolean sdSettingsChanged();
	boolean sensorPollingChanged();
	boolean serialDebugChanged();
       
  private:
	void setEEPROMaddresses();
    void readEEPROMvars();
    //System Settings
    //Watering Cycle
    boolean _waterTimed;
    uint8_t _waterHour;
    uint8_t _waterMinute;
    uint8_t _floodMinute;
    //Sensor Alarms
    float _phAlarmUp;
    float _phAlarmDown;
    uint16_t _ecAlarmUp;
    uint16_t _ecAlarmDown;
    uint8_t _waterAlarm;
    //Water at night - Informs the system that watering timers are stopped for the night
    boolean _nightWatering;
	//Sensor Calibrations
	uint16_t _lightThreshold;
	uint16_t _maxWaterLvl;
	uint16_t _minWaterLvl;
	//Pump protection threshold
	uint8_t _pumpProtectionLvl;

    
    //Controller settings
    //Time & Date - Handled outside in RTC Lib
    //Sensor Polling
    uint8_t _sensorSecond;  
    //SD Card
    boolean _sdActive;
    uint8_t _sdHour;
    uint8_t _sdMinute;
    //Sound toggle
    boolean _sound;
    //Serial Debugging
    boolean _serialDebug;
	//Module present
	boolean _reservoirModule;
    
    //Status variables - Not read from EEPROM
    //Time next watering will happen
    uint8_t _nextWhour;
    uint8_t _nextWminute;
    //Informs the system that watering timers are stopped for the night
    boolean _nightWateringStopped;
    //Turned on when plants are being watered
    boolean _wateringPlants;
    //Informs if theres an alarm triggered
    boolean _alarmTriggered;
	//True if state changed
	boolean _systemStateChanged;
	//Tells if water settings have been changed
	boolean _waterSettingsChanged;
	//SD options changed
	boolean _sdSettingsChanged;
	//Sensor polling changed
	boolean _sensorPollingChanged;
	//Serial debug toggle
	boolean _serialDebugChanged;
    
    //EEPROM addresses for all settings
    int _addressWaterTimed;
    int _addressWaterHour;
    int _addressWaterMinute;
    int _addressFloodMinute;
    int _addressPHalarmUp;
    int _addressPHalarmDown;
    int _addressECalarmUp;
    int _adressECalarmDown;
    int _addressWaterAlarm;
    int _addressNightWatering;  
    int _addressSensorMinute;
    int _addressSensorSecond;  
    int _addressSDactive;
    int _addressSDhour;
    int _addressSDminute;
    int _addressSound;
    int _addressSerialDebug;
	int _addressLightThreshold;
	int _addressReservoirModule;
	int _addressMaxWaterLvl;
	int _addressMinWaterLvl;
	int _addressPumpProtectionLvl;
	//int _addressVersion;
  
};

#endif
