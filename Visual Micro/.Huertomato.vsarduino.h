/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
void setupSerial();
void setupRTC();
void setupSD();
void setupAlarms();
void setupWaterModes();
void updateNextWateringTime();
void initMusic();
//
void checkAlarms();
void checkNightTime();
void checkSettingsChanged();
void checkWater();
void checkSD();
void checkSensors();
void checkSerial();
void timestampToSerial(String str);
void logSensorReadings();
void updateSensors();
void adjustECtemp();
void beepOn();
void beepOff();
void startWatering();
void stopWatering();

#include "C:\arduino-1.0.5-r2\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\arduino-1.0.5-r2\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Huertomato.ino"
#include "C:\Users\eNDeR\Desktop\Huertomato\Buttons.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Buttons.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\GUI.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\GUI.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\RGBled.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\RGBled.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Sensors.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Sensors.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Settings.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Settings.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\alarm126.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\fonts.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\logo126.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\logos64.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\moon126.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\plant126.c"
