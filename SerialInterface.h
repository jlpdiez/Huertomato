// #############################################################################
//
// # Name       : SerialComms
// # Version    : 1.0
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 19.11.2014
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

static const int nCommands = 4;
static char* commands[nCommands] = {
	"help",
	"sensors",
	"settings",
	"memory"
};

static const int nSensorsC = 2;
static char* sensorCommands[nSensorsC] = {
	"list",
	"get"
};

static const int nSettingsC = 3;
static char* settingsCommands[nSettingsC] = {
	"list",
	"get",
	"set"
};

static const int nSettings = 26;
static char* settingsNames[nSettings] = {
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
static char* sensorsNames[nSensors] = {
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
		SerialInterface(); //Sensors *sensors, Settings *settings);
		SerialInterface(const SerialInterface &other);
		SerialInterface& operator=(const SerialInterface &other);
		~SerialInterface();
		void init();
		void end();
		void processInput();
		void timeStamp(char* txt) const;
		void showAllStats() const;
		
	private:	
		//Static to prevent multiple instances and is also required to handle methods
		static SerialCommand _cmd;
		//Sensors *_sensors;
		//Settings *_settings;
		//Each of these functions are used when certain keywords are found in processInput
		//They need to be static so handler finds them correctly
		static void help();
		static void list(int length, char* names[]);
		static Command interpretCommand(char* keyword);
		static Sensors::Sensor interpretSensor(char* keyword);
		static Settings::Setting interpretSetting(char* keyword);
		static void commandSensors();
		static void getSensor(Sensors::Sensor sens);
		static void commandSettings();
		static void getSetting(Settings::Setting sett);
		//static void listSettings();
		static void commandMemory();
};

#endif