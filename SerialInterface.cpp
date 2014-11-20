#include "SerialInterface.h"

SerialCommand SerialInterface::_cmd(SerialCommand::SERIAL0);

//Constructors
SerialInterface::SerialInterface(Sensors *sensors, Settings *settings)
: _sensors(sensors), _settings(settings) {}

SerialInterface::SerialInterface(const SerialInterface &other) {
	_sensors = other._sensors;
	_settings = other._settings;
	_cmd = other._cmd;
}

SerialInterface& SerialInterface::operator=(const SerialInterface &other) {
	_sensors = other._sensors;
	_settings = other._settings;
	_cmd = other._cmd;
	return *this;
}

SerialInterface::~SerialInterface() {}

//Adds commands keywords to corresponding processing functions
void SerialInterface::init() {
	if (_settings->getSerialDebug()) {
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
	if (_settings->getSerialDebug()) {
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
	if (_settings->getSerialDebug()) {
		Serial << "Available memory: " << freeMemory() << " bytes"<< endl;
		Serial << "Temp: " << _sensors->getTemp() << "C" << endl;
		Serial << "Humidity: " << _sensors->getHumidity() << "%" << endl;
		Serial << "Light level: " << _sensors->getLight() << "%" << endl;
		if (_settings->getReservoirModule()) {
			Serial << "EC: " << _sensors->getEC() << endl;
			Serial << "pH: " << _sensors->getPH() << endl;
			Serial << "Water level: " << _sensors->getWaterLevel() << "%" << endl;
		}
	}
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
		helpSensors();
	//help settings
	else if (strcmp(arg,commands[2]) == 0)
		helpSettings();
	//help memory
	else if (strcmp(arg,commands[3]) == 0)
		Serial << endl << "> Displays current system's free memory." << endl << endl;
	//not recognized
	else 
		Serial << endl << "> No help found for command <" << arg << ">" << endl << endl;
}

//Displays list of available sensor commands
void SerialInterface::helpSensors() {
	Serial << endl;
	Serial << "> Used for managing sensors. Options are:" << endl << endl;
	for (int i = 0; i < nSensorsC; i++) {
		Serial << "> " << sensorCommands[i] << endl;
	}
	Serial << endl;
}

//Displays list of available settings commands
void SerialInterface::helpSettings() {
	Serial << endl;
	Serial << "> Used for managing settings. Options are:" << endl << endl;
	for (int i = 0; i < nSettingsC; i++) {
		Serial << "> " << settingsCommands[i] << endl;
	}
	Serial << endl;
}

//In charge when first word is sensors
void SerialInterface::commandSensors() {
	Serial.println("Sensoooooors huh!");
}

//In charge when first word is settings.
void SerialInterface::commandSettings() {
	Commands comm;
	//Read second word
	char *arg = _cmd.next();
	//int aNumber;
	//aNumber=atoi(arg);    // Converts a char string to an integer
	//aNumber=atol(arg);
	if (arg == NULL)
		helpSettings();
	//List
	else if (strcmp(arg,settingsCommands[List]) == 0) {
		Serial << endl;
		for (int i = 0; i < nSettings; i++) {
			Serial << "> " << settingsNames[i] << endl;
		}
		Serial << endl;
	//Get
	} else if (strcmp(arg,settingsCommands[Get]) == 0)
		comm = Get;
	//Set
	else if (strcmp(arg,settingsCommands[Set]) == 0) {
		comm = Set;
	//Second word unrecognised	
	} else
		helpSettings();
}

//In charge when "settings get".
void SerialInterface::commandGetSetting() {
	//Reads third word
	char *arg = _cmd.next();
	if (arg == NULL)
		helpSensors();
	else if (strcmp(arg,settingsNames[0]) == 0)
		Serial << endl << "> " << _settings->getWaterTimed() << endl << endl;
	else
		helpSettings();
}

//In charge when "settings set". Interprets third word
void SerialInterface::commandSetSetting() {
	char *arg = _cmd.next();
	if (arg == NULL)
		helpSettings();
	else
		helpSettings();
}

void SerialInterface::commandMemory() {
	Serial << endl << "> Available memory: " << freeMemory() << " bytes"<< endl << endl;
}
