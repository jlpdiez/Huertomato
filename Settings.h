// #############################################################################
//
// # Name       : Settings
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 15.11.2014
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
	boolean moduleChanged();
       
  private:
	void setEEPROMaddresses();
    void readEEPROMvars();
	//Everything static so we prevent duplicate info [Singleton]
    //System Settings
    //Watering Cycle
    static boolean _waterTimed;
    static uint8_t _waterHour;
    static uint8_t _waterMinute;
    static uint8_t _floodMinute;
    //Sensor Alarms
    static float _phAlarmUp;
    static float _phAlarmDown;
    static uint16_t _ecAlarmUp;
    static uint16_t _ecAlarmDown;
    static uint8_t _waterAlarm;
    //Water at night - Informs the system that watering timers are stopped for the night
    static boolean _nightWatering;
	//Sensor Calibrations
	static uint16_t _lightThreshold;
	static uint16_t _maxWaterLvl;
	static uint16_t _minWaterLvl;
	//Pump protection threshold
	static uint8_t _pumpProtectionLvl;
    
    //Controller settings
    //Time & Date - Handled outside in RTC Lib
    //Sensor Polling
    static uint8_t _sensorSecond;  
    //SD Card
    static boolean _sdActive;
    static uint8_t _sdHour;
    static uint8_t _sdMinute;
    //Sound toggle
    static boolean _sound;
    //Serial Debugging
    static boolean _serialDebug;
	//Module present
	static boolean _reservoirModule;
    
    //Status variables - Not read from EEPROM
    //Time next watering will happen
    static uint8_t _nextWhour;
    static uint8_t _nextWminute;
    //Informs the system that watering timers are stopped for the night
    static boolean _nightWateringStopped;
    //Turned on when plants are being watered
    static boolean _wateringPlants;
    //Informs if theres an alarm triggered
    static boolean _alarmTriggered;
	//True if state changed
	static boolean _systemStateChanged;
	//Tells if water settings have been changed
	static boolean _waterSettingsChanged;
	//SD options changed
	static boolean _sdSettingsChanged;
	//Sensor polling changed
	static boolean _sensorPollingChanged;
	//Serial debug toggle
	static boolean _serialDebugChanged;
	//Module config changed
	static boolean _moduleChanged;
    
    //EEPROM addresses for all settings
    static int _addressWaterTimed;
    static int _addressWaterHour;
    static int _addressWaterMinute;
    static int _addressFloodMinute;
    static int _addressPHalarmUp;
    static int _addressPHalarmDown;
    static int _addressECalarmUp;
    static int _adressECalarmDown;
    static int _addressWaterAlarm;
    static int _addressNightWatering;  
    static int _addressSensorMinute;
    static int _addressSensorSecond;  
    static int _addressSDactive;
    static int _addressSDhour;
    static int _addressSDminute;
    static int _addressSound;
    static int _addressSerialDebug;
	static int _addressLightThreshold;
	static int _addressReservoirModule;
	static int _addressMaxWaterLvl;
	static int _addressMinWaterLvl;
	static int _addressPumpProtectionLvl;
	//static int _addressVersion;
  
};

#endif
