// #############################################################################
//
// # Name       : SerialComms
// # Version    : 1.4
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 14.01.2016
//
// # Description: Class in charge of attending communication with input serial commands
// # Works at 115200. Commands must end with a carriage return to work properly.
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
#include <Time.h>  
#include <MemoryFree.h>
#include <ctype.h>

extern const float versionNumber;

//General strings
const char welcome0[] PROGMEM = ".::[ Huertomato ]::.";
const char welcome1[] PROGMEM = "www.TheGreenAutomation.com";
const char serialRdyTxt[] PROGMEM = "Serial I/O ready.";
const char serialOffTxt[] PROGMEM = "Deactivating serial communications.";

const char helpTxt0[] PROGMEM = "> Huertomato version ";
const char helpTxt1[] PROGMEM = "Type <help name> to find out more about the function <name>.";
const char memHelpTxt[] PROGMEM = "Displays current system's free memory.";
const char statusHelpTxt[] PROGMEM = "Displays system status and sensor info.";
const char commandsTxT[] PROGMEM = "Available commands are:";
const char sensorsTxT[] PROGMEM = "Available sensors are:";
const char settingsTxt[] PROGMEM = "Available settings are:";

const char memoryTxt[] PROGMEM = "> Available memory: ";
const char memoryTxt1[] PROGMEM = " bytes";
const char dateTxt[] PROGMEM = "> Date: ";
const char timeTxt[] PROGMEM = "> Time: ";
const char tempTxt[] PROGMEM = "> Temp: ";
const char celsTxt[] PROGMEM = "C";
const char fahrTxt[] PROGMEM = "F";
const char humidTxt[] PROGMEM = "> Humidity: ";
const char lightTxt[] PROGMEM = "> Light level: ";
const char luxTxt[] PROGMEM = " lux";
const char elecTxt[] PROGMEM = "> EC: ";
const char ecUnitsTxt[] PROGMEM = "mS";
const char pihTxt[] PROGMEM = "> pH: ";
const char levelTxt[] PROGMEM = "> Water level: ";

const char boolTxt[] PROGMEM = "Expected a 'true' or 'false'";
const char hourTxt[] PROGMEM = "Expected an hour 0 .. 23";
const char minSecTxt[] PROGMEM = "Expected 0 .. 59";
const char phTxt[] PROGMEM = "Expected pH 0.00 .. 14.00";
const char ecTxt[] PROGMEM = "Expected ec 0 .. 20.0";
const char percentTxT[] PROGMEM = "Expected 0 .. 100";
const char lvlTxt[] PROGMEM = "Expected 0 .. 1024";
const char innerTxt[] PROGMEM = "Inner var not to be changed";

const char successTxt[] PROGMEM = " successfuly updated to: ";
const char noHelp[] PROGMEM = "> No help found for command <";
const char noReservoir[] PROGMEM = "> Reservoir module is deactivated";
const char lineDeco[] PROGMEM = "> ";
const char textSeparator[] PROGMEM = ": ";
const char timeDots[] PROGMEM = ":";
const char timeStampSeparator[] PROGMEM = " - ";
const char dateSlash[] PROGMEM = "/";
const char percentTxt[] PROGMEM = "%";

//Command Strings
const char commandStr0[] PROGMEM = "help";
const char commandStr1[] PROGMEM = "sensors";
const char commandStr2[] PROGMEM = "settings";
const char commandStr3[] PROGMEM = "memory";
const char commandStr4[] PROGMEM = "status";
static const int nCommands = 5;
const char* const commands[] PROGMEM = { commandStr0, commandStr1, commandStr2,
	commandStr3, commandStr4 };

//Sensor commands strings
const char sensorStr0[] PROGMEM = "list";
const char sensorStr1[] PROGMEM = "get";
static const int nSensorsC = 2;
const char* const sensorCommands[] PROGMEM = { sensorStr0, sensorStr1 };

//Settings commands strings
const char settingsStr2[] PROGMEM = "set";
static const int nSettingsC = 3;
const char* const settingsCommands[] PROGMEM = { sensorStr0, sensorStr1, settingsStr2 };

//Sensor strings
const char sensorNameStr0[] PROGMEM = "Temperature";
const char sensorNameStr1[] PROGMEM = "Humidity";
const char sensorNameStr2[] PROGMEM = "Light";
const char sensorNameStr3[] PROGMEM = "Ec";
const char sensorNameStr4[] PROGMEM = "Ph";
const char sensorNameStr5[] PROGMEM = "Level";
static const int nSensors = 6;
const char* const sensorsNames[] PROGMEM = { sensorNameStr0, sensorNameStr1, sensorNameStr2,
sensorNameStr3, sensorNameStr4, sensorNameStr5 };

//Settings strings
const char settingsNameStr0[] PROGMEM = "WaterTimed";
const char settingsNameStr1[] PROGMEM = "WaterHour";
const char settingsNameStr2[] PROGMEM = "WaterMinute";
const char settingsNameStr3[] PROGMEM = "FloodMinute";
const char settingsNameStr4[] PROGMEM = "PHalarmUp";
const char settingsNameStr5[] PROGMEM = "PHalarmDown";
const char settingsNameStr6[] PROGMEM = "ECalarmUp";
const char settingsNameStr7[] PROGMEM = "ECalarmDown";
const char settingsNameStr8[] PROGMEM = "WaterAlarm";
const char settingsNameStr9[] PROGMEM = "NightWatering";
const char settingsNameStr10[] PROGMEM = "LightThreshold";
const char settingsNameStr11[] PROGMEM = "MaxWaterLvl";
const char settingsNameStr12[] PROGMEM = "MinWaterLvl";
const char settingsNameStr13[] PROGMEM = "PumpProtectionLvl";
const char settingsNameStr14[] PROGMEM = "SensorSecond";
const char settingsNameStr15[] PROGMEM = "SDactive";
const char settingsNameStr16[] PROGMEM = "SDhour";
const char settingsNameStr17[] PROGMEM = "SDminute";
const char settingsNameStr18[] PROGMEM = "Sound";
const char settingsNameStr19[] PROGMEM = "SerialDebug";
const char settingsNameStr20[] PROGMEM = "ReservoirModule";
const char settingsNameStr21[] PROGMEM = "NextWhour";
const char settingsNameStr22[] PROGMEM = "NextWminute";
const char settingsNameStr23[] PROGMEM = "NightWateringStopped";
const char settingsNameStr24[] PROGMEM = "WateringPlants";
const char settingsNameStr25[] PROGMEM = "AlarmTriggered";
const char settingsNameStr26[] PROGMEM = "Led";
const char settingsNameStr27[] PROGMEM = "Celsius";

static const int nSettings = 28;
const char* const settingsNames[] PROGMEM = { settingsNameStr0, settingsNameStr1, settingsNameStr2,
	settingsNameStr3, settingsNameStr4, settingsNameStr5, settingsNameStr6, settingsNameStr7,
	settingsNameStr8, settingsNameStr9, settingsNameStr10, settingsNameStr11, settingsNameStr12,
	settingsNameStr13, settingsNameStr14, settingsNameStr15, settingsNameStr16, settingsNameStr17,
	settingsNameStr18, settingsNameStr19, settingsNameStr20, settingsNameStr21, settingsNameStr22,
	settingsNameStr23, settingsNameStr24, settingsNameStr25, settingsNameStr26, settingsNameStr27 };

//This makes things static so we can use SerialCommand.addCommand("str",function)
extern Settings settings;
extern Sensors sensors;

static char charBuffer[64];

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
		void timeStamp(const char* txt) const;
		
	private:	
		//Static to prevent multiple instances and is also required to handle methods
		static SerialCommand _cmd;
		
		//Prints number preceeded by a '0' if < 10
		static void printDecNum(const uint8_t num);
		//These are used to read data from PROGMEM and store them into SRAM
		static char* pmChar(const char* pmArray);
		//Printers
		static void printLn(const char* ln, boolean leadingBlankLine = false, boolean trailingBlankLine = false);
		static void list(int length, const char* const names[]);
		static void printName(const char* ln);
		static void printUpdated(const char* ln, const char* arg);
		
		//Each of these functions are used when certain keywords are found in processInput
		//They need to be static so handler finds them correctly
		
		//This gets set as the default handler, and gets called when no other command matches.
		static void notFound();
		//Help command function
		static void help();
		//Uses external FreeMemory library
		static void commandMemory();
		//Sends all sensor data through serial
		static void commandStatus();
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
		static boolean isBoolean(const char* str);
		static boolean getBoolean(const char* str);
		static boolean isUint8_t(const char* str);
		static uint8_t getUint8_t(const char* str);
		static boolean isUint16_t(const char* str);
		static uint16_t getUint16_t(const char* str);
		static boolean isFloat(const char* str);
		static float getFloat(const char* str);
		//Executes the set commands
		static void setSetting(Settings::Setting sett);
};

#endif