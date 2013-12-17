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
    Settings();
    
    //Setters
    void setWaterTimed(boolean);
    void setWaterHour(uint8_t);
    void setWaterMinute(uint8_t);
    void setFloodMinute(uint8_t);
    void setPHalarmUp(float);
    void setPHalarmDown(float);
    void setECalarmUp(uint32_t);
    void setECalarmDown(uint32_t);
    void setWaterAlarm(uint8_t);
    void setNightWatering(boolean);
    void setManualPump(boolean);
    
    void setSensorMinute(uint8_t);
    void setSensorSecond(uint8_t);
    //void setNextWhour(uint8_t);
    //void setNextWminute(uint8_t);
    void setSDactive(boolean);
    void setSDhour(uint8_t);
    void setSDminute(uint8_t);
    void setSound(boolean);
    void setSerialDebug(boolean);
    
    //Getters
    boolean getWaterTimed();
    uint8_t getWaterHour();
    uint8_t getWaterMinute();
    uint8_t getFloodMinute();
    float getPHalarmUp();
    float getPHalarmDown();
    uint32_t getECalarmUp();
    uint32_t getECalarmDown();
    uint8_t getWaterAlarm();
    boolean getNightWatering();
    boolean getManualPump();
    
    uint8_t getSensorMinute();
    uint8_t getSensorSecond();
    //uint8_t getNextWhour();
    //uint8_t getNextWminute();
    boolean getSDactive();
    uint8_t getSDhour();
    uint8_t getSDminute();
    boolean getSound();
    boolean getSerialDebug();
    
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
    uint32_t _ecAlarmUp;
    uint32_t _ecAlarmDown;
    uint8_t _waterAlarm;
    //TODO: Sensor Calibration
    //If light < threshold we assume its night time
    //int _lightThreshold;
    //Water at night - Informs the system that watering timers are stopped for the night
    boolean _nightWatering;
    //Manual Water Pump - Keeps the pump status when manually controling them. Not read from EEPROM
    boolean _manualPump;
    
    //Controller settings
    //Time & Date - Handled outside (RTC Lib)
    //Sensor Polling
    uint8_t _sensorMinute;
    uint8_t _sensorSecond;  
    //Time next watering will occurr. Not read from EEPROM
    //uint8_t _nextWhour;
    //uint8_t _nextWminute;
    //SD Card
    boolean _sdActive;
    uint8_t _sdHour;
    uint8_t _sdMinute;
    //Sound toggle
    boolean _sound;
    //Serial Debugging
    boolean _serialDebug;
    
    
    //Keeps track of other situations
    //boolean wateringPlants = false;
    //boolean _alarmTriggered;

    
//    uint8_t _wHourTMP;
//    uint8_t _wMinuteTMP;
//    boolean _onlyDayTMP;
//    float _phAlarmUtmp;
//    float _phAlarmDtmp;
//    uint32_t _ecAlarmUtmp;
//    uint32_t _ecAlarmDtmp;
//    uint8_t _waterAlarmTMP;
//    //Temp variables used when manually changing time
//    uint8_t hourTMP, minTMP, secTMP, dayTMP, monthTMP;
//    int yearTMP;
    
    //EEPROM addresses for all settings
//    int _addressWhour;
//    int _addressWminute;
//    int _addressFloodM;
//    int _addressOnlyDay;
//    int _addressPHalarmU;
//    int _addressPHalarmD;
//    int _addressECalarmU;
//    int _addressECalarmD;
//    int _addressWaterAlarm;

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
//    int _adressManualPump;    
    int _adressSensorMinute;
    int _adressSensorSecond;  
//    int _adressNextWhour;
//    int _adressNextWminute;
    int _adressSDactive;
    int _adressSDhour;
    int _adressSDminute;
    int _adressSound;
    int _adressSerialDebug;
  
};



#endif
