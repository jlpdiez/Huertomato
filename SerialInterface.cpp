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
		/*
		//Wait for Serial and clear buffers
		while(!Serial);
		Serial.flush();
		while (Serial.available() > 0)
			Serial.read();
		*/
		//Welcome message
		Serial.println(pmChar(welcome0));
		Serial.println(pmChar(welcome1));
		Serial.println();
		// Setup callbacks for SerialCommand commands
		_cmd.addCommand(pmChar(commands[0]),SerialInterface::help);
		_cmd.addCommand(pmChar(commands[1]),SerialInterface::commandSensors);
		_cmd.addCommand(pmChar(commands[2]),SerialInterface::commandSettings);
		_cmd.addCommand(pmChar(commands[3]),SerialInterface::commandMemory);
		_cmd.addCommand(pmChar(commands[4]),SerialInterface::commandStatus);
		// Handler for command that isn't found
		_cmd.addDefaultHandler(notFound);
		timeStamp(serialRdyTxt);
	}
}

//Ends serial communication
void SerialInterface::end() {
	timeStamp(serialOffTxt);
	Serial.end();
}

//Checks in buffers and calls appropriate handle
void SerialInterface::processInput() {
	_cmd.readSerial();
	_cmd.clearBuffer();
	//Clears incoming Serial buffer
	while (Serial.available() > 0)
		Serial.read();
}

//Prints number preceeded by a '0' if needed
void SerialInterface::printDecNum(const uint8_t num) {
	(num < 10) ? Serial.print(F("0")) : 0;
	Serial.print(num);
}

//Writes "HH:MM:SS - <Text>" to serial console if serial debugging is on
//Assumes a PROGMEM char* as input
void SerialInterface::timeStamp(const char* txt) const {
	if (settings.getSerialDebug()) {
		time_t t = now();
		uint8_t h = hour(t);
		uint8_t m = minute(t);
		uint8_t s = second(t);
		printDecNum(h);
		Serial.print(pmChar(timeDots));
		printDecNum(m);
		Serial.print(pmChar(timeDots));
		printDecNum(s);
		Serial.print(pmChar(timeStampSeparator));
		Serial.println(pmChar(txt));
	}
}

//Converts a char array from PROGMEM to variable in SRAM
char* SerialInterface::pmChar(const char *pmArray) {
	strcpy_P(charBuffer, (char*)pmArray);
	return charBuffer;
}

//Prints a decorated line with optional leading and trailing blank lines
//Assumes a PROGMEM char* as input
void SerialInterface::printLn(const char* ln, boolean leadingBlankLine, boolean trailingBlankLine) {
	(leadingBlankLine) ? Serial.println() : 0;
	Serial.print(pmChar(lineDeco));
	Serial.println(pmChar(ln));
	(trailingBlankLine) ? Serial.println() : 0;
}

//Lists an array ending with an additional blank line
//Assumes a PROGMEM char* array as input
void SerialInterface::list(int length, const char* const names[]) {
	for (uint8_t i = 0; i < length; i++) {
		printLn((char*)pgm_read_word(&names[i]),false,false);
	}
}

//Prints "> Text: "
//Assumes a PROGMEM char* as input
void SerialInterface::printName(const char* ln) {
	Serial.print(pmChar(lineDeco));
	Serial.print(pmChar(ln));
	Serial.print(pmChar(textSeparator));
}

void SerialInterface::notFound() {
	Serial.println();
	Serial.print(pmChar(helpTxt0));
	Serial.println(versionNumber,2);
	printLn(helpTxt1,false,true);
	list(nCommands,commands);
}

//This gets set as the default handler, and gets called when no other command matches.
void SerialInterface::help() {
	//Read argument following "help"
	char *arg = _cmd.next();
	//Not more words
	if (arg == NULL) {
		Serial.println();
		Serial.print(pmChar(helpTxt0));
		Serial.println(versionNumber,2);
		printLn(helpTxt1,false,true);
		list(nCommands,commands);
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
		printLn(memHelpTxt);
	else if (strcmp_P(arg,commands[4]) == 0)
		printLn(statusHelpTxt);
	//not recognized
	else {
		Serial.println();
		Serial.print(pmChar(noHelp));
		Serial.print(arg);
		Serial.println(pmChar(lineDeco));
	}
		
}

//Uses external FreeMemory library
void SerialInterface::commandMemory() {
	Serial.println();
	Serial.print(pmChar(memoryTxt));
	Serial.print(freeMemory());
	Serial.println(pmChar(memoryTxt1));
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
		int mem = freeMemory();
		
		Serial.println();
		//Date
		Serial.print(pmChar(dateTxt));
		printDecNum(d);
		Serial.print(pmChar(dateSlash));
		printDecNum(mo);
		Serial.print(pmChar(dateSlash));
		Serial.println(y);
		//Time
		Serial.print(pmChar(timeTxt));
		printDecNum(h);
		Serial.print(pmChar(timeDots));
		printDecNum(m);
		Serial.print(pmChar(timeDots));
		printDecNum(s);
		Serial.println();
		//Memory
		Serial.print(pmChar(memoryTxt));
		Serial.print(mem);
		Serial.println(pmChar(memoryTxt1));
		//Temp
		Serial.print(pmChar(tempTxt));
		Serial.print(sensors.getTemp());
		(settings.getCelsius()) ? Serial.println(pmChar(celsTxt)) : Serial.println(pmChar(fahrTxt));
		//Humidity
		Serial.print(pmChar(humidTxt));
		Serial.print(sensors.getHumidity());
		Serial.println(pmChar(percentTxt));
		//Light
		Serial.print(pmChar(lightTxt));
		Serial.print(sensors.getLight());
		Serial.println(pmChar(luxTxt));
		//Reservoir module
		if (settings.getReservoirModule()) {
			//EC
			Serial.print(pmChar(elecTxt));
			Serial.print(sensors.getEC());
			Serial.println(pmChar(ecUnitsTxt));
			//pH
			Serial.print(pmChar(pihTxt));
			Serial.println(sensors.getPH());
			//Level
			Serial.print(pmChar(levelTxt));
			Serial.print(sensors.getWaterLevel());
			Serial.println(pmChar(percentTxt));
		}
	}
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
	//Word unrecognized
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
	else if (strcmp_P(keyword,settingsNames[26]) == 0)
		return Settings::Led;
	else if (strcmp_P(keyword,settingsNames[27]) == 0)
		return Settings::Celsius;
	//Word unrecognised
	else
		return Settings::None;
}

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
	Serial.println();
	switch (sens) {
		case Sensors::None:
			break;
		case Sensors::Temperature:
			printName(sensorsNames[0]);
			Serial.println(sensors.getTemp());
			break;
		case Sensors::Humidity:
			printName(sensorsNames[1]);
			Serial.println(sensors.getHumidity());
			break;
		case Sensors::Light:
			printName(sensorsNames[2]);
			Serial.println(sensors.getLight());
			break;
		case Sensors::Ec:
			if (settings.getReservoirModule()) {
				printName(sensorsNames[3]);
				Serial.println(sensors.getEC());				
			} else {
				Serial.println(pmChar(noReservoir));
			}
			break;
		case Sensors::Ph:
			if (settings.getReservoirModule()) {
				printName(sensorsNames[4]);
				Serial.println(sensors.getPH());				
			} else {
				Serial.println(pmChar(noReservoir));
			}
			break;
		case Sensors::Level:
			if (settings.getReservoirModule()) {
				printName(sensorsNames[5]);
				Serial.println(sensors.getWaterLevel());				
			} else {
				Serial.println(pmChar(noReservoir));
			}
			break;
	}
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
		Serial.println();
		switch (sett) {
			case Settings::None:
				break;
			case Settings::WaterTimed:
				printName(settingsNames[0]);
				Serial.println(settings.getWaterTimed());				
				break;
			case Settings::WaterHour:
				printName(settingsNames[1]);
				Serial.println(settings.getWaterHour());				
				break;
			case Settings::WaterMinute:
				printName(settingsNames[2]);
				Serial.println(settings.getWaterMinute());				
				break;
			case Settings::FloodMinute:
				printName(settingsNames[3]);
				Serial.println(settings.getFloodMinute());				
				break;
			case Settings::PHalarmUp:
				printName(settingsNames[4]);
				Serial.println(settings.getPHalarmUp());				
				break;
			case Settings::PHalarmDown:
				printName(settingsNames[5]);
				Serial.println(settings.getPHalarmDown());				
				break;
			case Settings::ECalarmUp:
				printName(settingsNames[6]);
				Serial.println(settings.getECalarmUp());				
				break;
			case Settings::ECalarmDown:
				printName(settingsNames[7]);
				Serial.println(settings.getECalarmDown());				
				break;
			case Settings::WaterAlarm:
				printName(settingsNames[8]);
				Serial.println(settings.getWaterAlarm());				
				break;
			case Settings::NightWatering:
				printName(settingsNames[9]);
				Serial.println(settings.getNightWatering());				
				break;
			case Settings::LightThreshold:
				printName(settingsNames[10]);
				Serial.println(settings.getLightThreshold());				
				break;
			case Settings::MaxWaterLvl:
				printName(settingsNames[11]);
				Serial.println(settings.getMaxWaterLvl());				
				break;
			case Settings::MinWaterLvl:
				printName(settingsNames[12]);
				Serial.println(settings.getMinWaterLvl());				
				break;
			case Settings::PumpProtectionLvl:
				printName(settingsNames[13]);
				Serial.println(settings.getPumpProtectionLvl());				
				break;
			case Settings::SensorSecond:
				printName(settingsNames[14]);
				Serial.println(settings.getSensorSecond());				
				break;
			case Settings::SDactive:
				printName(settingsNames[15]);
				Serial.println(settings.getSDactive());				
				break;
			case Settings::SDhour:
				printName(settingsNames[16]);
				Serial.println(settings.getSDhour());				
				break;
			case Settings::SDminute:
				printName(settingsNames[17]);
				Serial.println(settings.getSDminute());				
				break;
			case Settings::Sound:
				printName(settingsNames[18]);
				Serial.println(settings.getSound());				
				break;
			case Settings::SerialDebug:
				printName(settingsNames[19]);
				Serial.println(settings.getSerialDebug());				
				break;
			case Settings::ReservoirModule:
				printName(settingsNames[20]);
				Serial.println(settings.getReservoirModule());				
				break;
			case Settings::NextWhour:
				printName(settingsNames[21]);
				Serial.println(settings.getNextWhour());				
				break;
			case Settings::NextWminute:
				printName(settingsNames[22]);
				Serial.println(settings.getNextWminute());				
				break;
			case Settings::NightWateringStopped:
				printName(settingsNames[23]);
				Serial.println(settings.getNightWateringStopped());				
				break;
			case Settings::WateringPlants:
				printName(settingsNames[24]);
				Serial.println(settings.getWateringPlants());				
				break;
			case Settings::AlarmTriggered:
				printName(settingsNames[25]);
				Serial.println(settings.getAlarmTriggered());				
				break;
			case Settings::Led:
				printName(settingsNames[26]);
				Serial.println(settings.getLed());				
				break;
			case Settings::Celsius:
				printName(settingsNames[27]);
				Serial.println(settings.getCelsius());				
				break;
		}
}


//Checks if inputs are valid and convert methods
boolean SerialInterface::isBoolean(char* str) {
	return ((strcmp(str, "true") == 0) || (strcmp(str, "True") == 0) || (strcmp(str, "1") == 0) 
			|| (strcmp(str, "on") == 0) || (strcmp(str, "On") == 0) || (strcmp(str, "false") == 0) 
			|| (strcmp(str, "False") == 0) || (strcmp(str, "0") == 0) || (strcmp(str, "off") == 0)
			|| (strcmp(str, "Off") == 0));
}

boolean SerialInterface::getBoolean(char* str) {
	if (str == NULL)
		return false;
	else if ((strcmp(str, "true") == 0) || (strcmp(str, "1") == 0) || (strcmp(str, "on") == 0) 
		|| (strcmp(str, "True") == 0) || (strcmp(str, "On") == 0))
		return true;
	else if ((strcmp(str, "false") == 0) || (strcmp(str, "0") == 0) || (strcmp(str, "off") == 0)
		|| (strcmp(str, "False") == 0) || (strcmp(str, "Off") == 0))
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
	Serial.println();
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
				uint16_t l = getUint16_t(arg);
				if (settings.setMaxWaterLvl(l)) {
					sensors.setMaxLvl(l);
					printLn(doneTxt);
				} else
					printLn(lvlTxt);
			} else
				printLn(lvlTxt);
			break;
			
		case Settings::MinWaterLvl:
			if (isUint16_t(arg)) {
				uint16_t l = getUint16_t(arg);
				if (settings.setMinWaterLvl(l)) {
					sensors.setMinLvl(l);
					printLn(doneTxt);
				} else
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
			
		case Settings::Led:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setLed(true) : settings.setLed(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::Celsius:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setCelsius(true) : settings.setCelsius(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::SerialDebug:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setSerialDebug(true) : settings.setSerialDebug(false);
				printLn(doneTxt);
			} else
				printLn(boolTxt);
			break;
			
		case Settings::ReservoirModule:
			if (isBoolean(arg)) {
				(getBoolean(arg)) ? settings.setReservoirModule(true) : settings.setReservoirModule(false);
				printLn(doneTxt);
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
}