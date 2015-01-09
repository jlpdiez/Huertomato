#include "SerialInterface.h"

SerialCommand SerialInterface::_cmd(SerialCommand::SERIAL0);

//Constructors
SerialInterface::SerialInterface() {}

SerialInterface::SerialInterface(const SerialInterface &other) {
	_cmd = other._cmd;
}

SerialInterface& SerialInterface::operator=(const SerialInterface &other) {
	_cmd = other._cmd;
	return *this;
}

SerialInterface::~SerialInterface() {}

//Adds commands keywords to corresponding processing functions
void SerialInterface::init() {
	if (settings.getSerialDebug()) {
		Serial.begin(115200);
		Serial << endl << F(".::[ Huertomato ]::.") << endl;
		Serial << F("www.TheGreenAutomation.com") << endl << endl;
		// Setup callbacks for SerialCommand commands
		_cmd.addCommand(commands[0],SerialInterface::help);
		_cmd.addCommand(commands[1],SerialInterface::commandSensors);
		_cmd.addCommand(commands[2],SerialInterface::commandSettings);
		_cmd.addCommand(commands[3],SerialInterface::commandMemory);
		_cmd.addCommand(commands[4],SerialInterface::commandStatus);
		// Handler for command that isn't matched
		_cmd.addDefaultHandler(help);
		timeStamp(PSTR("Serial input ready"));
	}
	  
}

//Ends serial communication
void SerialInterface::end() {
	timeStamp(PSTR("Deactivating serial communications"));
	Serial.end();
}

//Checks in buffers and calls to handle
void SerialInterface::processInput() {
	_cmd.readSerial();
	_cmd.clearBuffer();
}

//Writes "HH:MM:SS - <Text>" to serial console if serial debugging is on
void SerialInterface::timeStamp(const char* txt) const {
	if (settings.getSerialDebug()) {
		time_t t = now();
		uint8_t h = hour(t);
		uint8_t m = minute(t);
		uint8_t s = second(t);
		Serial << ((h<10)?F("0"):"") << h << F(":") << ((m<10)?F("0"):"") << m << F(":") << ((s<10)?F("0"):"") << s;
		Serial  << F(" - ") << txt << endl;
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
		Serial << F("> Huertomato version 1.4") << endl;
		Serial << F("> Type <help name> to find out more about the function <name>.") << endl << endl;
		for (int i = 0; i < nCommands; i++) {
			Serial << F("> ") << commands[i] << endl;
		}
		Serial << endl;
	//help sensors
	//if arg == commands[1]
	} else if (strcmp_P(arg,commands[1]) == 0) {
		printLn(commandsTxT);
		list(nSensorsC,sensorCommands);
	//help settings
	} else if (strcmp_P(arg,commands[2]) == 0) {
		printLn(commandsTxT);
		list(nSettingsC,settingsCommands);
	//help memory
	} else if (strcmp_P(arg,commands[3]) == 0)
		printLn(PSTR("Displays current system's free memory."));
	else if (strcmp_P(arg,commands[4]) == 0)
		printLn(PSTR("Displays all sensor info."));
	//not recognized
	else 
		Serial << endl << F("> No help found for command <") << arg << F(">") << endl << endl;
}

//Uses external FreeMemory library
void SerialInterface::commandMemory() {
	Serial << endl << F("> Available memory: ") << freeMemory() << F(" bytes") << endl << endl;
}

//Sends sensor data through serial
void SerialInterface::commandStatus() {
	if (settings.getSerialDebug()) {
		time_t t = now();
		uint8_t h = hour(t);
		uint8_t m = minute(t);
		uint8_t s = second(t);
		uint8_t d = day(t);
		uint8_t mo = month(t);
		uint16_t y = year(t);
		
		Serial << endl;
		Serial << F("> Date: ") << ((d<10)?F("0"):"") << d << F("/") << ((mo<10)?F("0"):"") << mo << F("/") << y << endl;
		Serial << F("> Time: ") << ((h<10)?F("0"):"") << h << F(":") << ((m<10)?F("0"):"") << m << F(":") << ((s<10)?F("0"):"") << s << endl;
		Serial << F("> Available memory: ") << freeMemory() << F(" bytes") << endl;
		Serial << F("> Temp: ") << sensors.getTemp() << F("C") << endl;
		Serial << F("> Humidity: ") << sensors.getHumidity() << F("%") << endl;
		Serial << F("> Light level: ") << sensors.getLight() << F("lux") << endl;
		if (settings.getReservoirModule()) {
			Serial << F("> EC: ") << sensors.getEC() << endl;
			Serial << F("> pH: ") << sensors.getPH() << endl;
			Serial << F("> Water level: ") << sensors.getWaterLevel() << F("%") << endl;
		}
		Serial << endl;
	}
	Serial.flush();
}

//Prints a decorated line
void SerialInterface::printLn(const char* ln) {
	Serial << endl << F("> ") << pmChar(ln) << endl << endl;
}

//Lists an array with blank lines before and after and a ">" symbol first in each line
void SerialInterface::list(int length, const char* const names[]) {
	Serial << endl;
	for (int i = 0; i < length; i++) {
		Serial << F("> ") << pmChar(names[i]) << endl;
	}
	Serial <<endl;
}

//Returns command contained in input keyword or Invalid
SerialInterface::Command SerialInterface::interpretCommand(char* keyword) {
	if (keyword == NULL)
		return Invalid;
	//keyword equals settingsCommands[]
	else if (strcmp_P(keyword,settingsCommands[0]) == 0)
		return List;
	else if (strcmp_P(keyword,settingsCommands[1]) == 0)
		return Get;
	else if (strcmp_P(keyword,settingsCommands[2]) == 0)
		return Set;
	else
		return Invalid;
}

//Returns sensor contained in input keyword or None
Sensors::Sensor SerialInterface::interpretSensor(char* keyword) {
	if (keyword == NULL)
		return Sensors::None;
	else if (strcmp_P(keyword,sensorsNames[0]) == 0)
		return Sensors::Temperature;
	else if (strcmp_P(keyword,sensorsNames[1]) == 0)
		return Sensors::Humidity;
	else if (strcmp_P(keyword,sensorsNames[2]) == 0)
		return Sensors::Light;
	else if (strcmp_P(keyword,sensorsNames[3]) == 0)
		return Sensors::Ec;
	else if (strcmp_P(keyword,sensorsNames[4]) == 0)
		return Sensors::Ph;
	else if (strcmp_P(keyword,sensorsNames[5]) == 0)
		return Sensors::Level;
	//Word unrecognised
	else
		return Sensors::None;
}

//Returns setting contained in input keyword or None
Settings::Setting SerialInterface::interpretSetting(char* keyword) {
	if (keyword == NULL)
		return Settings::None;
	else if (strcmp_P(keyword,settingsNames[0]) == 0)
		return Settings::WaterTimed;
	else if (strcmp_P(keyword,settingsNames[1]) == 0)
		return Settings::WaterHour;
	else if (strcmp_P(keyword,settingsNames[2]) == 0)
		return Settings::WaterMinute;
	else if (strcmp_P(keyword,settingsNames[3]) == 0)
		return Settings::FloodMinute;
	else if (strcmp_P(keyword,settingsNames[4]) == 0)
		return Settings::PHalarmUp;
	else if (strcmp_P(keyword,settingsNames[5]) == 0)
		return Settings::PHalarmDown;
	else if (strcmp_P(keyword,settingsNames[6]) == 0)
		return Settings::ECalarmUp;
	else if (strcmp_P(keyword,settingsNames[7]) == 0)
		return Settings::ECalarmDown;
	else if (strcmp_P(keyword,settingsNames[8]) == 0)
			return Settings::WaterAlarm;
	else if (strcmp_P(keyword,settingsNames[9]) == 0)
		return Settings::NightWatering;
	else if (strcmp_P(keyword,settingsNames[10]) == 0)
		return Settings::LightThreshold;
	else if (strcmp_P(keyword,settingsNames[11]) == 0)
		return Settings::MaxWaterLvl;
	else if (strcmp_P(keyword,settingsNames[12]) == 0)
		return Settings::MinWaterLvl;
	else if (strcmp_P(keyword,settingsNames[13]) == 0)
		return Settings::PumpProtectionLvl;
	else if (strcmp_P(keyword,settingsNames[14]) == 0)
		return Settings::SensorSecond;
	else if (strcmp_P(keyword,settingsNames[15]) == 0)
		return Settings::SDactive;
	else if (strcmp_P(keyword,settingsNames[16]) == 0)
		return Settings::SDhour;
	else if (strcmp_P(keyword,settingsNames[17]) == 0)
		return Settings::SDminute;
	else if (strcmp_P(keyword,settingsNames[18]) == 0)
		return Settings::Sound;
	else if (strcmp_P(keyword,settingsNames[19]) == 0)
		return Settings::SerialDebug;
	else if (strcmp_P(keyword,settingsNames[20]) == 0)
		return Settings::ReservoirModule;
	else if (strcmp_P(keyword,settingsNames[21]) == 0)
		return Settings::NextWhour;
	else if (strcmp_P(keyword,settingsNames[22]) == 0)
		return Settings::NextWminute;
	else if (strcmp_P(keyword,settingsNames[23]) == 0)
		return Settings::NightWateringStopped;
	else if (strcmp_P(keyword,settingsNames[24]) == 0)
		return Settings::WateringPlants;
	else if (strcmp_P(keyword,settingsNames[25]) == 0)
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
			Serial << F("> ") << pmChar(sensorsNames[0]) << F(": ") << sensors.getTemp() << endl;
			break;
		case Sensors::Humidity:
			Serial << F("> ") << pmChar(sensorsNames[1]) << F(": ") << sensors.getHumidity() << endl;
			break;
		case Sensors::Light:
			Serial << F("> ") << pmChar(sensorsNames[2]) << F(": ") << sensors.getLight() << endl;
			break;
		case Sensors::Ec:
			Serial << F("> ") << pmChar(sensorsNames[3]) << F(": ") << sensors.getEC() << endl;
			break;
		case Sensors::Ph:
			Serial << F("> ") << pmChar(sensorsNames[4]) << F(": ") << sensors.getPH() << endl;
			break;
		case Sensors::Level:
			Serial << F("> ") << pmChar(sensorsNames[5]) << F(": ") << sensors.getWaterLevel() << endl;
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
				Serial << F("> ") << pmChar(settingsNames[0]) << F(": ") << settings.getWaterTimed() << endl;
				break;
			case Settings::WaterHour:
				Serial << F("> ") << pmChar(settingsNames[1]) << F(": ") << settings.getWaterHour() << endl;
				break;
			case Settings::WaterMinute:
				Serial << F("> ") << pmChar(settingsNames[2]) << F(": ") << settings.getWaterMinute() << endl;
				break;
			case Settings::FloodMinute:
				Serial << F("> ") << pmChar(settingsNames[3]) << F(": ") << settings.getFloodMinute() << endl;
				break;
			case Settings::PHalarmUp:
				Serial << F("> ") << pmChar(settingsNames[4]) << F(": ") << settings.getPHalarmUp() << endl;
				break;
			case Settings::PHalarmDown:
				Serial << F("> ") << pmChar(settingsNames[5]) << F(": ") << settings.getPHalarmDown() << endl;
				break;
			case Settings::ECalarmUp:
				Serial << F("> ") << pmChar(settingsNames[6]) << F(": ") << settings.getECalarmUp() << endl;
				break;
			case Settings::ECalarmDown:
				Serial << F("> ") << pmChar(settingsNames[7]) << F(": ") << settings.getECalarmDown() << endl;
				break;
			case Settings::WaterAlarm:
				Serial << F("> ") << pmChar(settingsNames[8]) << F(": ") << settings.getWaterAlarm() << endl;
				break;
			case Settings::NightWatering:
				Serial << F("> ") << pmChar(settingsNames[9]) << F(": ") << settings.getNightWatering() << endl;
				break;
			case Settings::LightThreshold:
				Serial << F("> ") << pmChar(settingsNames[10]) << F(": ") << settings.getLightThreshold() << endl;
				break;
			case Settings::MaxWaterLvl:
				Serial << F("> ") << pmChar(settingsNames[11]) << F(": ") << settings.getMaxWaterLvl() << endl;
				break;
			case Settings::MinWaterLvl:
				Serial << F("> ") << pmChar(settingsNames[12]) << F(": ") << settings.getMinWaterLvl() << endl;
				break;
			case Settings::PumpProtectionLvl:
				Serial << F("> ") << pmChar(settingsNames[13]) << F(": ") << settings.getPumpProtectionLvl() << endl;
				break;
			case Settings::SensorSecond:
				Serial << F("> ") << pmChar(settingsNames[14]) << F(": ") << settings.getSensorSecond() << endl;
				break;
			case Settings::SDactive:
				Serial << F("> ") << pmChar(settingsNames[15]) << F(": ") << settings.getSDactive() << endl;
				break;
			case Settings::SDhour:
				Serial << F("> ") << pmChar(settingsNames[16]) << F(": ") << settings.getSDhour() << endl;
				break;
			case Settings::SDminute:
				Serial << F("> ") << pmChar(settingsNames[17]) << F(": ") << settings.getSDminute() << endl;
				break;
			case Settings::Sound:
				Serial << F("> ") << pmChar(settingsNames[18]) << F(": ") << settings.getSound() << endl;
				break;
			case Settings::SerialDebug:
				Serial << F("> ") << pmChar(settingsNames[19]) << F(": ") << settings.getSerialDebug() << endl;
				break;
			case Settings::ReservoirModule:
				Serial << F("> ") << pmChar(settingsNames[20]) << F(": ") << settings.getReservoirModule() << endl;
				break;
			case Settings::NextWhour:
				Serial << F("> ") << pmChar(settingsNames[21]) << F(": ") << settings.getNextWhour() << endl;
				break;
			case Settings::NextWminute:
				Serial << F("> ") << pmChar(settingsNames[22]) << F(": ") << settings.getNextWminute() << endl;
				break;
			case Settings::NightWateringStopped:
				Serial << F("> ") << pmChar(settingsNames[23]) << F(": ") << settings.getNightWateringStopped() << endl;
				break;
			case Settings::WateringPlants:
				Serial << F("> ") << pmChar(settingsNames[24]) << F(": ") << settings.getWateringPlants() << endl;
				break;
			case Settings::AlarmTriggered:
				Serial << F("> ") << pmChar(settingsNames[25]) << F(": ") << settings.getAlarmTriggered() << endl;
				break;
		}
		Serial << endl;
}


//Checks if inputs are valid and convert methods
boolean SerialInterface::isBoolean(char* str) {
	return ((strcmp_P(str, PSTR("true")) == 0) || (strcmp_P(str, PSTR("1")) == 0) 
			|| (strcmp_P(str, PSTR("false")) == 0) || (strcmp_P(str, PSTR("0")) == 0));
}

boolean SerialInterface::getBoolean(char* str) {
	if (str == NULL)
		return false;
	else if ((strcmp_P(str, PSTR("true")) == 0) || (strcmp_P(str, PSTR("1")) == 0))
		return true;
	else if ((strcmp_P(str, PSTR("false")) == 0) || (strcmp_P(str, PSTR("0")) == 0))
		return false;
}

boolean SerialInterface::isUint8_t(char* str) {
	if (str == NULL)
		return false;
	//We use an order of magnitude bigger number to cast and check correctness
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
				printLn(PSTR("Done"));
			} else
				printLn(boolTxt);
			break;
			
		case Settings::ReservoirModule:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setReservoirModule(true) : settings.setReservoirModule(false);
				printLn(PSTR("Done");
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

//Converts a char array from PROGMEM to variable in SRAM
char* SerialInterface::pmChar(const char *pmArray) {
	strcpy_P(_stringBuffer, (char*)pmArray);
	return _stringBuffer;
}