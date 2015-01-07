// #############################################################################
//
// # Name       : SerialComms
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 21.11.2014
//
// # Description: Class in charge of attending communication with input serial commands
// # Works at 115200. Commands must en with a carriage return to work properly.
// # You initiate it and then just have to call processSerialCommand() from outside
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

#ifndef SERIALINTERFACE_H_
#define SERIALINTERFACE_H_

#include "Sensors.h"
#include "Settings.h"
#include <SerialCommand.h>
#include <Streaming.h>
#include <Time.h>  
#include <MemoryFree.h>

static char* commandsTxT PROGMEM = "Available commands are:";
static char* sensorsTxT PROGMEM = "Available sensors are:";
static char* settingsTxt PROGMEM = "Available settings are:";
static char* doneTxt PROGMEM = "Done.";
static char* boolTxt PROGMEM = "Expected a 'true' or 'false'";
static char* hourTxt PROGMEM = "Expected an hour 0 .. 23";
static char* minSecTxt PROGMEM = "Expected 0 .. 59";
static char* phTxt PROGMEM = "Expected pH 0.00 .. 14.00";
static char* ecTxt PROGMEM = "Expected ec 0 .. 9999";
static char* percentTxT PROGMEM = "Expected 0 .. 100";
static char* lvlTxt PROGMEM = "Expected 0 .. 1024";

static const int nCommands = 5;
static char* commands[nCommands] PROGMEM = {
	"help",
	"sensors",
	"settings",
	"memory",
	"status"
};

static const int nSensorsC = 2;
static char* sensorCommands[nSensorsC] PROGMEM = {
	"list",
	"get"
};

static const int nSettingsC = 3;
static char* settingsCommands[nSettingsC] PROGMEM = {
	"list",
	"get",
	"set"
};

static const int nSettings = 26;
static char* settingsNames[nSettings] PROGMEM = {
	"WaterTimed",
	"WaterHour",
	"WaterMinute",
	"FloodMinute",
	"PHalarmUp",
	"PHalarmDown",
	"ECalarmUp",
	"ECalarmDown",
	"WaterAlarm",
	"NightWatering",
	"LightThreshold",
	"MaxWaterLvl",
	"MinWaterLvl",
	"PumpProtectionLvl",
	"SensorSecond",
	"SDactive",
	"SDhour",
	"SDminute",
	"Sound",
	"SerialDebug",
	"ReservoirModule",
	"NextWhour",
	"NextWminute",
	"NightWateringStopped",
	"WateringPlants",
	"AlarmTriggered"
};

static const int nSensors = 6;
static char* sensorsNames[nSensors] PROGMEM = {
	"Temperature",
	"Humidity",
	"Light",
	"Ec",
	"Ph",
	"Level"
};

//This makes settings static so we can use SerialCommand.addCommand("str",function)
extern Settings settings;
extern Sensors sensors;

class SerialInterface {
	public:
		enum Command {
			Invalid = 0,
			List = 1,
			Get = 2, 
			Set = 3
		};
		SerialInterface();
		SerialInterface(const SerialInterface &other);
		SerialInterface& operator=(const SerialInterface &other);
		~SerialInterface();
		//Adds commands keywords to corresponding processing functions
		void init();
		//Ends serial communication
		void end();
		//Checks in buffers and calls to handle
		void processInput();
		//Writes "HH:MM:SS - <Text>" to serial console if serial debugging is on
		void timeStamp(char* txt) const;
		
	private:	
		//Static to prevent multiple instances and is also required to handle methods
		static SerialCommand _cmd;
		
		//Each of these functions are used when certain keywords are found in processInput
		//They need to be static so handler finds them correctly
		
		//This gets set as the default handler, and gets called when no other command matches.
		static void help();
		//Uses external FreeMemory library
		static void commandMemory();
		//Sends all sensor data through serial
		static void commandStatus();
		//Printers
		static void printLn(char* ln);
		static void list(int length, char* names[]);
		//Returns enum contained in input keyword or Invalid/Nobne
		static Command interpretCommand(char* keyword);
		static Sensors::Sensor interpretSensor(char* keyword);
		static Settings::Setting interpretSetting(char* keyword);
		//Gets called when "sensors" detected
		static void commandSensors();
		//Executes get command
		static void getSensor(Sensors::Sensor sens);
		//Gets called when "settings" detected
		static void commandSettings();
		//Executes get command
		static void getSetting(Settings::Setting sett);
		//Checks if inputs are valid and convert methods
		static boolean isBoolean(char* str);
		static boolean getBoolean(char* str);
		static boolean isUint8_t(char* str);
		static uint8_t getUint8_t(char* str);
		static boolean isUint16_t(char* str);
		static uint16_t getUint16_t(char* str);
		static boolean isFloat(char* str);
		static float getFloat(char* str);
		//Executes the set commands
		static void setSetting(Settings::Setting sett);
};

#endif