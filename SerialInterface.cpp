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
		_cmd.addCommand(commands[1],SerialInterface::commandSensors);
		_cmd.addCommand(commands[2],SerialInterface::commandSettings);
		_cmd.addCommand(commands[3],SerialInterface::commandMemory);
		_cmd.addCommand(commands[4],SerialInterface::commandStatus);
		// Handler for command that isn't matched
		_cmd.addDefaultHandler(help);
		timeStamp("Serial input ready");
	}
	  
}

//Ends serial communication
void SerialInterface::end() {
	timeStamp("Deactivating serial communications");
	Serial.end();
}

//Checks in buffers and calls to handle
void SerialInterface::processInput() {
	_cmd.readSerial();
	_cmd.clearBuffer();
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
	Serial.flush();
}

//This gets set as the default handler, and gets called when no other command matches.
void SerialInterface::help() {
	Serial.flush();
	//Read argument following "help"
	char *arg = _cmd.next();
	//Not more words
	if (arg == NULL) {
		Serial << endl;
		Serial << "> Huertomato UI, version 1.0" << endl;
		//Serial << "> These commands are defined internally.  Type <help> to see this list." << endl;
		Serial << "> Type <help name> to find out more about the function <name>." << endl << endl;
		for (int i = 0; i < nCommands; i++) {
			Serial << "> " << commands[i] << endl;
		}
		Serial << endl;
	//help sensors
	//if arg == commands[1]
	} else if (strcmp(arg,commands[1]) == 0) {
		printLn(commandsTxT);
		list(nSensorsC,sensorCommands);
	//help settings
	} else if (strcmp(arg,commands[2]) == 0) {
		printLn(commandsTxT);
		list(nSettingsC,settingsCommands);
	//help memory
	} else if (strcmp(arg,commands[3]) == 0)
		printLn("Displays current system's free memory.");
	else if (strcmp(arg,commands[4]) == 0)
		printLn("Displays all sensor info.");
	//not recognized
	else 
		Serial << endl << "> No help found for command <" << arg << ">" << endl << endl;
}

//Uses external FreeMemory library
void SerialInterface::commandMemory() {
	Serial << endl << "> Available memory: " << freeMemory() << " bytes"<< endl << endl;
}

//Sends sensor data through serial
void SerialInterface::commandStatus() {
	if (settings.getSerialDebug()) {
		Serial << endl;
		Serial << "> Available memory: " << freeMemory() << " bytes"<< endl;
		Serial << "> Temp: " << sensors.getTemp() << "C" << endl;
		Serial << "> Humidity: " << sensors.getHumidity() << "%" << endl;
		Serial << "> Light level: " << sensors.getLight() << "%" << endl;
		if (settings.getReservoirModule()) {
			Serial << "> EC: " << sensors.getEC() << endl;
			Serial << "> pH: " << sensors.getPH() << endl;
			Serial << "> Water level: " << sensors.getWaterLevel() << "%" << endl;
		}
		Serial << endl;
	}
	Serial.flush();
}

//Prints a decorated line
void SerialInterface::printLn(char* ln) {
	Serial << endl << "> " << ln << endl << endl;
}


//Lists an array with blank lines before and after and a ">" symbol first in each line
void SerialInterface::list(int length, char* names[]) {
	Serial << endl;
	for (int i = 0; i < length; i++) {
		Serial << "> " << names[i] << endl;
	}
	Serial <<endl;
}

//Returns command contained in input keyword or Invalid
SerialInterface::Command SerialInterface::interpretCommand(char* keyword) {
	if (keyword == NULL)
		return Invalid;
	//keyword equals settingsCommands[]
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

//TODO: Add set Time!
//In charge when first word is sensors
void SerialInterface::commandSensors() {
	Command comm;
	Sensors::Sensor sens;
	//Read second word and get command
	char *arg = _cmd.next();
	comm = interpretCommand(arg);
	arg = _cmd.next();
	sens = interpretSensor(arg);
	
	switch (comm) {
		case Invalid:
			printLn(commandsTxT);
			list(nSensorsC,sensorCommands);
			break;
		case List:
			printLn(sensorsTxT);
			list(nSensors,sensorsNames);
			break;
		case Get:
			if (sens == Sensors::None) {
				printLn(sensorsTxT);
				list(nSensors,sensorsNames);
			} else
				getSensor(sens);
			break;
		default:
			printLn(commandsTxT);
			list(nSensorsC,sensorCommands);
			break;
	}
}

//Displays info from input setting to screen
void SerialInterface::getSensor(Sensors::Sensor sens) {
	Serial.flush();
	Serial << endl;
	switch (sens) {
		case Sensors::None:
			break;
		case Sensors::Temperature:
			Serial << "> " << sensorsNames[0] << ": " << sensors.getTemp() << endl;
			break;
		case Sensors::Humidity:
			Serial << "> " << sensorsNames[1] << ": " << sensors.getHumidity() << endl;
			break;
		case Sensors::Light:
			Serial << "> " << sensorsNames[2] << ": " << sensors.getLight() << endl;
			break;
		case Sensors::Ec:
			Serial << "> " << sensorsNames[3] << ": " << sensors.getEC() << endl;
			break;
		case Sensors::Ph:
			Serial << "> " << sensorsNames[4] << ": " << sensors.getPH() << endl;
			break;
		case Sensors::Level:
			Serial << "> " << sensorsNames[5] << ": " << sensors.getWaterLevel() << endl;
			break;
	}
	Serial << endl;
}

//Links keywords to program logic and executes commands
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
			printLn(commandsTxT);
			list(nSettingsC,settingsCommands);
			break;
		case List:
			printLn(settingsTxt);
			list(nSettings,settingsNames);
			break;
		case Get:
			if (sett == Settings::None) {
				printLn(settingsTxt);
				list(nSettings,settingsNames);
			} else
				getSetting(sett);
			break;
		case Set:
			if (sett == Settings::None) {
				printLn(settingsTxt);
				list(nSettings,settingsNames);
			} else
				setSetting(sett);
			break;
		default:
			printLn(commandsTxT);
			list(nSensorsC,sensorCommands);
			break;
	}
}

//Displays info from input setting to screen
void SerialInterface::getSetting(Settings::Setting sett) {
		Serial.flush();
		Serial << endl;
		switch (sett) {
			case Settings::None:
				break;
			case Settings::WaterTimed:
				Serial << "> " << settingsNames[0] << ": " << settings.getWaterTimed() << endl;
				break;
			case Settings::WaterHour:
				Serial << "> " << settingsNames[1] << ": " << settings.getWaterHour() << endl;
				break;
			case Settings::WaterMinute:
				Serial << "> " << settingsNames[2] << ": " << settings.getWaterMinute() << endl;
				break;
			case Settings::FloodMinute:
				Serial << "> " << settingsNames[3] << ": " << settings.getFloodMinute() << endl;
				break;
			case Settings::PHalarmUp:
				Serial << "> " << settingsNames[4] << ": " << settings.getPHalarmUp() << endl;
				break;
			case Settings::PHalarmDown:
				Serial << "> " << settingsNames[5] << ": " << settings.getPHalarmDown() << endl;
				break;
			case Settings::ECalarmUp:
				Serial << "> " << settingsNames[6] << ": " << settings.getECalarmUp() << endl;
				break;
			case Settings::ECalarmDown:
				Serial << "> " << settingsNames[7] << ": " << settings.getECalarmDown() << endl;
				break;
			case Settings::WaterAlarm:
				Serial << "> " << settingsNames[8] << ": " << settings.getWaterAlarm() << endl;
				break;
			case Settings::NightWatering:
				Serial << "> " << settingsNames[9] << ": " << settings.getNightWatering() << endl;
				break;
			case Settings::LightThreshold:
				Serial << "> " << settingsNames[10] << ": " << settings.getLightThreshold() << endl;
				break;
			case Settings::MaxWaterLvl:
				Serial << "> " << settingsNames[11] << ": " << settings.getMaxWaterLvl() << endl;
				break;
			case Settings::MinWaterLvl:
				Serial << "> " << settingsNames[12] << ": " << settings.getMinWaterLvl() << endl;
				break;
			case Settings::PumpProtectionLvl:
				Serial << "> " << settingsNames[13] << ": " << settings.getPumpProtectionLvl() << endl;
				break;
			case Settings::SensorSecond:
				Serial << "> " << settingsNames[14] << ": " << settings.getSensorSecond() << endl;
				break;
			case Settings::SDactive:
				Serial << "> " << settingsNames[15] << ": " << settings.getSDactive() << endl;
				break;
			case Settings::SDhour:
				Serial << "> " << settingsNames[16] << ": " << settings.getSDhour() << endl;
				break;
			case Settings::SDminute:
				Serial << "> " << settingsNames[17] << ": " << settings.getSDminute() << endl;
				break;
			case Settings::Sound:
				Serial << "> " << settingsNames[18] << ": " << settings.getSound() << endl;
				break;
			case Settings::SerialDebug:
				Serial << "> " << settingsNames[19] << ": " << settings.getSerialDebug() << endl;
				break;
			case Settings::ReservoirModule:
				Serial << "> " << settingsNames[20] << ": " << settings.getReservoirModule() << endl;
				break;
			case Settings::NextWhour:
				Serial << "> " << settingsNames[21] << ": " << settings.getNextWhour() << endl;
				break;
			case Settings::NextWminute:
				Serial << "> " << settingsNames[22] << ": " << settings.getNextWminute() << endl;
				break;
			case Settings::NightWateringStopped:
				Serial << "> " << settingsNames[23] << ": " << settings.getNightWateringStopped() << endl;
				break;
			case Settings::WateringPlants:
				Serial << "> " << settingsNames[24] << ": " << settings.getWateringPlants() << endl;
				break;
			case Settings::AlarmTriggered:
				Serial << "> " << settingsNames[25] << ": " << settings.getAlarmTriggered() << endl;
				break;
		}
		Serial << endl;
}


//Checks if inputs are valid and convert methods
boolean SerialInterface::isBoolean(char* str) {
	return ((strcmp(str,"true") == 0) || (strcmp(str,"1") == 0) 
			|| (strcmp(str,"false") == 0) || (strcmp(str,"0") == 0));
}

boolean SerialInterface::getBoolean(char* str) {
	if (str == NULL)
		return false;
	else if ((strcmp(str,"true") == 0) || (strcmp(str,"1") == 0))
		return true;
	else if ((strcmp(str,"false") == 0) || (strcmp(str,"0") == 0))
		return false;
}

boolean SerialInterface::isUint8_t(char* str) {
	if (str == NULL)
		return false;
	//We use an order of magnitude bigger number to cast and check correctnes
	long num = atol(str);
	return ((num >= 0) && (num < 256));
}

//Doesn't check if input is right... use method above
uint8_t SerialInterface::getUint8_t(char* str) {
	return (uint8_t)atoi(str);
}

boolean SerialInterface::isUint16_t(char* str) {
	if (str == NULL)
		return false;
	long num = atol(str);
	return ((num >= 0) && (num < 65536));
}

//Doesn't check if input is right... use method above
uint16_t SerialInterface::getUint16_t(char* str) {
	return (uint16_t)atol(str);
}
//Strangest way if checking for data.
boolean SerialInterface::isFloat(char* str) {
	if (str == NULL)
		return false;
	float num = atof(str);
	return ((num >= 0) && (num < 99.99));	
}

//Doesn't check if input is right... use method above
float SerialInterface::getFloat(char* str) {
	return atof(str);
}

//Gets info from input, checks validity and modifies data
void SerialInterface::setSetting(Settings::Setting sett) {
	Serial.flush();
	Serial << endl;
	char* arg = _cmd.next();
	switch (sett) {
		case Settings::None:
			break;
			
		case Settings::WaterTimed:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setWaterTimed(true) : settings.setWaterTimed(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::WaterHour: 
			if (isUint8_t(arg)) {
				if (settings.setWaterHour(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(hourTxt);
			} else
				printLn(hourTxt);
			break;
			
		case Settings::WaterMinute:
			if (isUint8_t(arg)) {
				if (settings.setWaterMinute(getUint8_t(arg)))
				printLn(doneTxt);
				else
				printLn(minSecTxt);
			} else
			printLn(minSecTxt);
			break;
			
		case Settings::FloodMinute:
			if (isUint8_t(arg)) {
				if (settings.setFloodMinute(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(minSecTxt);
			} else
				printLn(minSecTxt);
			break;
			
		case Settings::PHalarmUp:
			if (isFloat(arg)) {
				if (settings.setPHalarmUp(getFloat(arg)))
					printLn(doneTxt);
				else
					printLn(phTxt);
			} else
				printLn(phTxt);
			break;
			
		case Settings::PHalarmDown:
			if (isFloat(arg)) {
				if (settings.setPHalarmDown(getFloat(arg)))
					printLn(doneTxt);
				else
					printLn(phTxt);
			} else
				printLn(phTxt);
			break;
			
		case Settings::ECalarmUp:
			if (isUint16_t(arg)) {
				if (settings.setECalarmUp(getUint16_t(arg)))
					printLn(doneTxt);
				else
					printLn(ecTxt);
			} else
				printLn(ecTxt);
			break;
			
		case Settings::ECalarmDown:
			if (isUint16_t(arg)) {
				if (settings.setECalarmDown(getUint16_t(arg)))
					printLn(doneTxt);
				else
					printLn(ecTxt);
			} else
				printLn(ecTxt);
			break;
			
		case Settings::WaterAlarm:
			if (isUint8_t(arg)) {
				if (settings.setWaterAlarm(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(percentTxT);
			} else
				printLn(percentTxT);
			break;
			
		case Settings::NightWatering:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setNightWatering(true) : settings.setNightWatering(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::LightThreshold:
			if (isUint8_t(arg)) {
				if (settings.setLightThreshold(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(percentTxT);
			} else
				printLn(percentTxT);
			break;
			
		case Settings::MaxWaterLvl:
			if (isUint16_t(arg)) {
				if (settings.setMaxWaterLvl(getUint16_t(arg)))
					printLn(doneTxt);
				else
					printLn(lvlTxt);
			} else
				printLn(lvlTxt);
			break;
			
		case Settings::MinWaterLvl:
			if (isUint16_t(arg)) {
				if (settings.setMinWaterLvl(getUint16_t(arg)))
					printLn(doneTxt);
				else
					printLn(lvlTxt);
			} else
				printLn(lvlTxt);
			break;
			
		case Settings::PumpProtectionLvl:
			if (isUint8_t(arg)) {
				if (settings.setPumpProtectionLvl(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(percentTxT);
			} else
				printLn(percentTxT);
			break;
			
		case Settings::SensorSecond:
			if (isUint8_t(arg)) {
				if (settings.setSensorSecond(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(minSecTxt);
			} else
				printLn(minSecTxt);
			break;
			
		case Settings::SDactive:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setSDactive(true) : settings.setSDactive(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::SDhour:
			if (isUint8_t(arg)) {
				if (settings.setSDhour(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(hourTxt);
			} else
				printLn(hourTxt);
			break;
			
		case Settings::SDminute:
			if (isUint8_t(arg)) {
				if (settings.setSDminute(getUint8_t(arg)))
					printLn(doneTxt);
				else
					printLn(minSecTxt);
			} else
				printLn(minSecTxt);
			break;
			
		case Settings::Sound:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setSound(true) : settings.setSound(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::SerialDebug:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setSerialDebug(true) : settings.setSerialDebug(false);
				printLn("Done");
			} else
				printLn(boolTxt);
			break;
			
		case Settings::ReservoirModule:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setReservoirModule(true) : settings.setReservoirModule(false);
				printLn("Done");
			} else
				printLn(boolTxt);
			break;
		//These are left blank on purpose. Only main .ino logic should touch these 
		//as they are internal states not user-selectable settings per se
		case Settings::NextWhour:
		case Settings::NextWminute:
		case Settings::NightWateringStopped:
		case Settings::WateringPlants:
		case Settings::AlarmTriggered:
		default:
			break;
	}
	Serial << endl;
}
