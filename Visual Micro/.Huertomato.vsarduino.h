/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
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
void updateNextWateringTime();
void initMusic();
//
void writeSerialTimestamp();
void logSensorReadings();
void updateSensors();
void adjustECtemp();
void waterPlants();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Huertomato.ino"
#include "C:\Users\eNDeR\Desktop\Huertomato\Buttons.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Buttons.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\GUI.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\GUI.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Other.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Other.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\RGBled.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\RGBled.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Sensors.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Sensors.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\Settings.cpp"
#include "C:\Users\eNDeR\Desktop\Huertomato\Settings.h"
#include "C:\Users\eNDeR\Desktop\Huertomato\alarm126.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\fonts.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\hand126.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\logos64.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\moon126.c"
#include "C:\Users\eNDeR\Desktop\Huertomato\plant126.c"
