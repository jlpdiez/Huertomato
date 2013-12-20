#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROMex.h>

extern EEPROMClassEx EEPROM;

// *********************************************
// Settings Class
// Stores all the system's current settings. Its in charge of reading and storing in EEPROM 
// *********************************************
//TODO: Add light Threshold
class Settings {
  public:
	//Constructors
    Settings();
	Settings(const Settings &other);
	Settings& operator=(const Settings &other);
	//Destructor
	~Settings();
    
    //Setters
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
    
    //Controller Settings
    void setSensorMinute(const uint8_t);
    void setSensorSecond(const uint8_t);
    void setSDactive(const boolean);
    void setSDhour(const uint8_t);
    void setSDminute(const uint8_t);
    void setSound(const boolean);
    void setSerialDebug(const boolean);
    
    //Status vars
    void setNextWhour(const uint8_t);
    void setNextWminute(const uint8_t);
    void setManualPump(const boolean);
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
    
    //Controller Settings
    uint8_t getSensorMinute() const;
    uint8_t getSensorSecond() const;
    boolean getSDactive() const;
    uint8_t getSDhour() const;
    uint8_t getSDminute() const;
    boolean getSound() const;
    boolean getSerialDebug() const;
    
    //Status vars
    uint8_t getNextWhour() const;
    uint8_t getNextWminute() const;
    boolean getManualPump() const;
    boolean getNightWateringStopped() const;
    boolean getWateringPlants() const;
    boolean getAlarmTriggered() const;
       
  private:
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
    //TODO: Sensor Calibration
    //If light < threshold we assume its night time
    //int _lightThreshold;
    //Water at night - Informs the system that watering timers are stopped for the night
    boolean _nightWatering;

    
    //Controller settings
    //Time & Date - Handled outside (RTC Lib)
    //Sensor Polling
    uint8_t _sensorMinute;
    uint8_t _sensorSecond;  
    //SD Card
    boolean _sdActive;
    uint8_t _sdHour;
    uint8_t _sdMinute;
    //Sound toggle
    boolean _sound;
    //Serial Debugging
    boolean _serialDebug;
    
    //Status variables - Not read from EEPROM
    //Time next watering will occurr
    uint8_t _nextWhour;
    uint8_t _nextWminute;
    //Manual Water Pump - Keeps the pump status when manually controling them.
    boolean _manualPump;
    //Informs the system that watering timers are stopped for the night
    boolean _nightWateringStopped;
    //Turned on when plants are beign watered
    boolean _wateringPlants;
    //Informs if theres an alarm triggered
    boolean _alarmTriggered;
    
    //EEPROM addresses for all settings
    int _adressWaterTimed;
    int _adressWaterHour;
    int _adresswaterMinute;
    int _adressFloodMinute;
    int _adressPHalarmUp;
    int _adressPHalarmDown;
    int _adressECalarmUp;
    int _adressECalarmDown;
    int _adressWaterAlarm;
    int _adresssSerialDebug;
    int _adressActivateSD;
    int _adressNightWatering;  
    int _adressSensorMinute;
    int _adressSensorSecond;  
    int _adressSDactive;
    int _adressSDhour;
    int _adressSDminute;
    int _adressSound;
    int _adressSerialDebug;
  
};

#endif

// Dew point temp
// delta max = 0.6544 wrt dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
/*double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity/100);
  double Td = (b * temp) / (a - temp);
  return Td;
}*/