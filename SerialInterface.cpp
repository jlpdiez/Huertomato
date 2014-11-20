#include "SerialInterface.h"

SerialCommand SerialInterface::_cmd(SerialCommand::SERIAL0);

//Constructors
SerialInterface::SerialInterface() {} //Sensors *sensors, Settings *settings)
//: _sensors(sensors), _settings(settings) {}

SerialInterface::SerialInterface(const SerialInterface &other) {
	//_sensors = other._sensors;
	//_settings = other._settings;
	_cmd = other._cmd;
}

SerialInterface& SerialInterface::operator=(const SerialInterface &other) {
	//_sensors = other._sensors;
	//_settings = other._settings;
	_cmd = other._cmd;
	return *this;
}

SerialInterface::~SerialInterface() {}

//Adds commands keywords to corresponding processing functions
void SerialInterface::init() {
	if (settings.getSerialDebug()) {
		Serial.begin(115200);
		Serial << endl << ".::[ Huertomato ]::." << endl;
		Serial << "www.TheGreenAutomation.com" << endl << endl;
		// Setup callbacks for SerialCommand commands
		_cmd.addCommand(commands[0],SerialInterface::help);
		_cmd.addCommand(commands[1],SerialInterface::commandSettings);
		_cmd.addCommand(commands[2],SerialInterface::commandSensors);
		_cmd.addCommand(commands[3],SerialInterface::commandMemory);
		// Handler for command that isn't matched
		_cmd.addDefaultHandler(help);
		timeStamp("Serial input ready");
	}
	  
}

void SerialInterface::end() {
	timeStamp("Deactivating serial communications");
	Serial.end();
}

//Checks in buffers and calls to handle
void SerialInterface::processInput() {
	_cmd.readSerial();
}

//Writes "HH:MM:SS - <Text>" to serial console if serial debugging is on
void SerialInterface::timeStamp(char* txt) const {
	if (settings.getSerialDebug()) {
		time_t t = now();
		uint8_t h = hour(t);
		uint8_t m = minute(t);
		uint8_t s = second(t);
		Serial << ((h<10)?"0":"") << h << ":" << ((m<10)?"0":"") << m << ":" << ((s<10)?"0":"") << s;
		Serial  << " - " << txt << endl;
	}
	//Serial.flush();
}

//Sends sensor data through serial
void SerialInterface::showAllStats() const { 
	if (settings.getSerialDebug()) {
		Serial << "Available memory: " << freeMemory() << " bytes"<< endl;
		Serial << "Temp: " << sensors.getTemp() << "C" << endl;
		Serial << "Humidity: " << sensors.getHumidity() << "%" << endl;
		Serial << "Light level: " << sensors.getLight() << "%" << endl;
		if (settings.getReservoirModule()) {
			Serial << "EC: " << sensors.getEC() << endl;
			Serial << "pH: " << sensors.getPH() << endl;
			Serial << "Water level: " << sensors.getWaterLevel() << "%" << endl;
		}
	}
}

//Lists an array with blank lines before and after and a ">" symbol first in each line
void SerialInterface::list(int length, char* names[]) {
	Serial << endl;
	for (int i = 0; i < length; i++) {
		Serial << "> " << names[i] << endl;
	}
	Serial <<endl;
}

//This gets set as the default handler, and gets called when no other command matches.
void SerialInterface::help() {
	//Serial.flush();
	//Read argument following "help"
	char *arg = _cmd.next();
	//Not more words
	if (arg == NULL) {
		Serial << endl;
		Serial << "> Huertomato UI, version 1.0" << endl;
		Serial << "> These commands are defined internally.  Type <help> to see this list." << endl;
		Serial << "> Type <help name> to find out more about the function <name>." << endl << endl;
		for (int i = 0; i < nCommands; i++) {
			Serial << "> " << commands[i] << endl;
		}
		Serial << endl;
	//help sensors
	//if arg == commands[1]
	} else if (strcmp(arg,commands[1]) == 0)
		list(nSensorsC,sensorCommands);
	//help settings
	else if (strcmp(arg,commands[2]) == 0)
		list(nSettingsC,settingsCommands);
	//help memory
	else if (strcmp(arg,commands[3]) == 0)
		Serial << endl << "> Displays current system's free memory." << endl << endl;
	//not recognized
	else 
		Serial << endl << "> No help found for command <" << arg << ">" << endl << endl;
}

//Returns command contained in input keyword or Invalid
SerialInterface::Command SerialInterface::interpretCommand(char* keyword) {
	if (keyword == NULL)
		return Invalid;
	else if (strcmp(keyword,settingsCommands[0]) == 0)
		return List;
	else if (strcmp(keyword,settingsCommands[1]) == 0)
		return Get;
	else if (strcmp(keyword,settingsCommands[2]) == 0)
		return Set;
	else
		return Invalid;
}

//Returns sensor contained in input keyword or None
Sensors::Sensor SerialInterface::interpretSensor(char* keyword) {
	if (keyword == NULL)
		return Sensors::None;
	else if (strcmp(keyword,sensorsNames[0]) == 0)
		return Sensors::Temperature;
	else if (strcmp(keyword,sensorsNames[1]) == 0)
		return Sensors::Humidity;
	else if (strcmp(keyword,sensorsNames[2]) == 0)
		return Sensors::Light;
	else if (strcmp(keyword,sensorsNames[3]) == 0)
		return Sensors::Ec;
	else if (strcmp(keyword,sensorsNames[4]) == 0)
		return Sensors::Ph;
	else if (strcmp(keyword,sensorsNames[5]) == 0)
		return Sensors::Level;
	//Word unrecognised
	else
		return Sensors::None;
}

//Returns setting contained in input keyword or None
Settings::Setting SerialInterface::interpretSetting(char* keyword) {
	if (keyword == NULL)
		return Settings::None;
	else if (strcmp(keyword,settingsNames[0]) == 0)
		return Settings::WaterTimed;
	else if (strcmp(keyword,settingsNames[1]) == 0)
		return Settings::WaterHour;
	else if (strcmp(keyword,settingsNames[2]) == 0)
		return Settings::WaterMinute;
	else if (strcmp(keyword,settingsNames[3]) == 0)
		return Settings::FloodMinute;
	else if (strcmp(keyword,settingsNames[4]) == 0)
		return Settings::PHalarmUp;
	else if (strcmp(keyword,settingsNames[5]) == 0)
		return Settings::PHalarmDown;
	else if (strcmp(keyword,settingsNames[6]) == 0)
		return Settings::ECalarmUp;
	else if (strcmp(keyword,settingsNames[7]) == 0)
		return Settings::ECalarmDown;
	else if (strcmp(keyword,settingsNames[8]) == 0)
			return Settings::WaterAlarm;
	else if (strcmp(keyword,settingsNames[9]) == 0)
		return Settings::NightWatering;
	else if (strcmp(keyword,settingsNames[10]) == 0)
		return Settings::LightThreshold;
	else if (strcmp(keyword,settingsNames[11]) == 0)
		return Settings::MaxWaterLvl;
	else if (strcmp(keyword,settingsNames[12]) == 0)
		return Settings::MinWaterLvl;
	else if (strcmp(keyword,settingsNames[13]) == 0)
		return Settings::PumpProtectionLvl;
	else if (strcmp(keyword,settingsNames[14]) == 0)
		return Settings::SensorSecond;
	else if (strcmp(keyword,settingsNames[15]) == 0)
		return Settings::SDactive;
	else if (strcmp(keyword,settingsNames[16]) == 0)
		return Settings::SDhour;
	else if (strcmp(keyword,settingsNames[17]) == 0)
		return Settings::SDminute;
	else if (strcmp(keyword,settingsNames[18]) == 0)
		return Settings::Sound;
	else if (strcmp(keyword,settingsNames[19]) == 0)
		return Settings::SerialDebug;
	else if (strcmp(keyword,settingsNames[20]) == 0)
		return Settings::ReservoirModule;
	else if (strcmp(keyword,settingsNames[21]) == 0)
		return Settings::NextWhour;
	else if (strcmp(keyword,settingsNames[22]) == 0)
		return Settings::NextWminute;
	else if (strcmp(keyword,settingsNames[23]) == 0)
		return Settings::NightWateringStopped;
	else if (strcmp(keyword,settingsNames[24]) == 0)
		return Settings::WateringPlants;
	else if (strcmp(keyword,settingsNames[25]) == 0)
		return Settings::AlarmTriggered;
	//Word unrecognised
	else
		return Settings::None;
}

//In charge when first word is sensors
void SerialInterface::commandSensors() {
	Serial.println("> Sensoooooors huh!");
}

//Executes get setting action
void SerialInterface::getSensor(Sensors::Sensor sens) {
	switch (sens) {
		case Sensors::None:
			break;
		case Sensors::Temperature:
			break;
		case Sensors::Humidity:
			break;
		case Sensors::Light:
			break;
		case Sensors::Ec:
			break;
		case Sensors::Ph:
			break;
		case Sensors::Level:
			break;
	}
}

//In charge when first word is settings.
void SerialInterface::commandSettings() {
	Command comm;
	Settings::Setting sett;
	//Read second word and get command
	char *arg = _cmd.next();
	comm = interpretCommand(arg);
	arg = _cmd.next();
	sett = interpretSetting(arg);
	
	switch (comm) {
		case Invalid:
			list(nSettingsC,settingsCommands);
			break;
		case List:
			list(nSettings,settingsNames);
			break;
		case Get:
			if (sett == Settings::None) {
				Serial << "Available settings are:" << endl;
				list(nSettings,settingsNames);
			}
			getSetting(sett);
			break;
		case Set:
			if (sett == Settings::None) {
				Serial << "> Available settings are:" << endl;
				list(nSettings,settingsNames);
			}
			//setSetting(sett);
			break;
	}
}

//Displays info from input setting to screen
void SerialInterface::getSetting(Settings::Setting sett) {
		Serial << endl;
		switch (sett) {
			case Settings::None:
				break;
			case Settings::WaterTimed:
				Serial << "> " << settingsNames[0] << ": " << settings.getWaterTimed();
				break;
			case Settings::WaterHour:
				Serial << "> " << settingsNames[1] << ": " << settings.getWaterHour();
				break;
			case Settings::WaterMinute:
				Serial << "> " << settingsNames[2] << ": " << settings.getWaterMinute();
				break;
			case Settings::FloodMinute:
				Serial << "> " << settingsNames[3] << ": " << settings.getFloodMinute();
				break;
			case Settings::PHalarmUp:
				Serial << "> " << settingsNames[4] << ": " << settings.getPHalarmUp();
				break;
			case Settings::PHalarmDown:
				Serial << "> " << settingsNames[5] << ": " << settings.getPHalarmDown();
				break;
			case Settings::ECalarmUp:
				Serial << "> " << settingsNames[6] << ": " << settings.getECalarmUp();
				break;
			case Settings::ECalarmDown:
				Serial << "> " << settingsNames[7] << ": " << settings.getECalarmDown();
				break;
			case Settings::WaterAlarm:
				Serial << "> " << settingsNames[8] << ": " << settings.getWaterAlarm();
				break;
			case Settings::NightWatering:
				Serial << "> " << settingsNames[9] << ": " << settings.getNightWatering();
				break;
			case Settings::LightThreshold:
				Serial << "> " << settingsNames[10] << ": " << settings.getLightThreshold();
				break;
			case Settings::MaxWaterLvl:
				Serial << "> " << settingsNames[11] << ": " << settings.getMaxWaterLvl();
				break;
			case Settings::MinWaterLvl:
				Serial << "> " << settingsNames[12] << ": " << settings.getMinWaterLvl();
				break;
			case Settings::PumpProtectionLvl:
				Serial << "> " << settingsNames[13] << ": " << settings.getPumpProtectionLvl();
				break;
			case Settings::SensorSecond:
				Serial << "> " << settingsNames[14] << ": " << settings.getSensorSecond();
				break;
			case Settings::SDactive:
				Serial << "> " << settingsNames[15] << ": " << settings.getSDactive();
				break;
			case Settings::SDhour:
				Serial << "> " << settingsNames[16] << ": " << settings.getSDhour();
				break;
			case Settings::SDminute:
				Serial << "> " << settingsNames[17] << ": " << settings.getSDminute();
				break;
			case Settings::Sound:
				Serial << "> " << settingsNames[18] << ": " << settings.getSound();
				break;
			case Settings::SerialDebug:
				Serial << "> " << settingsNames[19] << ": " << settings.getSerialDebug();
				break;
			case Settings::ReservoirModule:
				Serial << "> " << settingsNames[20] << ": " << settings.getReservoirModule();
				break;
			case Settings::NextWhour:
				Serial << "> " << settingsNames[21] << ": " << settings.getNextWhour();
				break;
			case Settings::NextWminute:
				Serial << "> " << settingsNames[22] << ": " << settings.getNextWminute();
				break;
			case Settings::NightWateringStopped:
				Serial << "> " << settingsNames[23] << ": " << settings.getNightWateringStopped();
				break;
			case Settings::WateringPlants:
				Serial << "> " << settingsNames[24] << ": " << settings.getWateringPlants();
				break;
			case Settings::AlarmTriggered:
				Serial << "> " << settingsNames[25] << ": " << settings.getAlarmTriggered();
				break;
		}
		Serial << endl;
}

void SerialInterface::commandMemory() {
	Serial << endl << "> Available memory: " << freeMemory() << " bytes"<< endl << endl;
}
