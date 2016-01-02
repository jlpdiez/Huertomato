/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Mega w/ ATmega2560 (Mega 2560), Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 167
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 167
#define ARDUINO_AVR_MEGA2560
#define ARDUINO_ARCH_AVR
extern "C" void __cxa_pure_virtual() {;}

//
void setupRTC();
void setupSD();
void setupAlarms();
void setupWaterModes();
void updateNextWateringTime();
void initMusic();
//
void checkLed();
void checkPump();
void checkAlarms();
void checkSoundAlarm();
void checkNightTime();
void checkSettingsChanged();
void checkWater();
void checkSD();
void checkSensors();
void checkSerial();
void logSensorReadings();
void printAlarm();
void printPump();
void updateSensors();
void adjustECtemp();
void adjustPHtemp();
void beepOn();
void beepOff();
void startSerialAlarmTimer();
void stopSerialAlarmTimer();
void startSerialPumpProtTimer();
void stopSerialPumpProtTimer();
void startSDlogTimer();
void stopSDlogTimer();
void startWaterTimer();
void stopWaterTimer();
void startWaterOffTimer();
void stopWaterOffTimer();
void startWatering();
void stopWatering();

#include "C:\Program Files\Arduino\hardware\arduino\avr\variants\mega\pins_arduino.h" 
#include "C:\Program Files\Arduino\hardware\arduino\avr\cores\arduino\arduino.h"
#include <..\Huertomato\Huertomato.ino>
#include <..\Huertomato\Buttons.cpp>
#include <..\Huertomato\Buttons.h>
#include <..\Huertomato\GUI.cpp>
#include <..\Huertomato\GUI.h>
#include <..\Huertomato\RGBled.cpp>
#include <..\Huertomato\RGBled.h>
#include <..\Huertomato\Sensor.cpp>
#include <..\Huertomato\Sensor.h>
#include <..\Huertomato\SensorEC.cpp>
#include <..\Huertomato\SensorEC.h>
#include <..\Huertomato\SensorHumid.cpp>
#include <..\Huertomato\SensorHumid.h>
#include <..\Huertomato\SensorLight.cpp>
#include <..\Huertomato\SensorLight.h>
#include <..\Huertomato\SensorPH.cpp>
#include <..\Huertomato\SensorPH.h>
#include <..\Huertomato\SensorTemp.cpp>
#include <..\Huertomato\SensorTemp.h>
#include <..\Huertomato\SensorWater.cpp>
#include <..\Huertomato\SensorWater.h>
#include <..\Huertomato\Sensors.cpp>
#include <..\Huertomato\Sensors.h>
#include <..\Huertomato\SerialInterface.cpp>
#include <..\Huertomato\SerialInterface.h>
#include <..\Huertomato\Settings.cpp>
#include <..\Huertomato\Settings.h>
#include <..\Huertomato\WinAlarms.cpp>
#include <..\Huertomato\WinAlarms.h>
#include <..\Huertomato\WinControllerMenu.cpp>
#include <..\Huertomato\WinControllerMenu.h>
#include <..\Huertomato\WinControllerMenuTwo.cpp>
#include <..\Huertomato\WinControllerMenuTwo.h>
#include <..\Huertomato\WinEcAlarms.cpp>
#include <..\Huertomato\WinEcAlarms.h>
#include <..\Huertomato\WinEcCalib.cpp>
#include <..\Huertomato\WinEcCalib.h>
#include <..\Huertomato\WinLvlAlarms.cpp>
#include <..\Huertomato\WinLvlAlarms.h>
#include <..\Huertomato\WinLvlCalib.cpp>
#include <..\Huertomato\WinLvlCalib.h>
#include <..\Huertomato\WinMainMenu.cpp>
#include <..\Huertomato\WinMainMenu.h>
#include <..\Huertomato\WinMainScreen.cpp>
#include <..\Huertomato\WinMainScreen.h>
#include <..\Huertomato\WinPhAlarms.cpp>
#include <..\Huertomato\WinPhAlarms.h>
#include <..\Huertomato\WinPhCalib.cpp>
#include <..\Huertomato\WinPhCalib.h>
#include <..\Huertomato\WinPump.cpp>
#include <..\Huertomato\WinPump.h>
#include <..\Huertomato\WinReservoir.cpp>
#include <..\Huertomato\WinReservoir.h>
#include <..\Huertomato\WinSD.cpp>
#include <..\Huertomato\WinSD.h>
#include <..\Huertomato\WinSensorCalib.cpp>
#include <..\Huertomato\WinSensorCalib.h>
#include <..\Huertomato\WinSensorPolling.cpp>
#include <..\Huertomato\WinSensorPolling.h>
#include <..\Huertomato\WinSystemMenu.cpp>
#include <..\Huertomato\WinSystemMenu.h>
#include <..\Huertomato\WinTime.cpp>
#include <..\Huertomato\WinTime.h>
#include <..\Huertomato\WinWater.cpp>
#include <..\Huertomato\WinWater.h>
#include <..\Huertomato\WinWaterNight.cpp>
#include <..\Huertomato\WinWaterNight.h>
#include <..\Huertomato\Window.cpp>
#include <..\Huertomato\Window.h>
#include <..\Huertomato\fonts.c>
#include <..\Huertomato\logo126.c>
#include <..\Huertomato\logos64.c>
