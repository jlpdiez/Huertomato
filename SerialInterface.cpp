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
		_cmd.addCommand(commands[0],SerialInterface::printHelp);
		_cmd.addCommand(commands[1],SerialInterface::commandSettings);
		_cmd.addCommand(commands[2],SerialInterface::commandSensors);
		_cmd.addCommand(commands[3],SerialInterface::commandMemory);
		// Handler for command that isn't matched
		_cmd.addDefaultHandler(printHelp);
		timeStamp("Serial input ready");
	}
	  
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
void SerialInterface::printHelp() {
	//Serial.flush();
	//Read argument following "help"
	char *arg = _cmd.next();
	//Nothing more
	Serial << endl;
	if (arg == NULL) {
		Serial << "> Huertomato UI, version 1.0" << endl;
		Serial << "> These commands are defined internally.  Type <help> to see this list." << endl;
		Serial << "> Type <help name> to find out more about the function <name>." << endl << endl;
		for (int i = 0; i < nCommands; i++) {
			Serial << "> " << commands[i] << endl;
		}
	//help sensors
	//if arg == commands[1]
	} else if (strcmp(arg,commands[1]) == 0) {
		Serial << "> Used for managing sensors. Options are:" << endl << endl;
		for (int i = 0; i < nSensorsC; i++) {
			Serial << "> " << sensorCommands[i] << endl;
		}
	//help settings
	} else if (strcmp(arg,commands[2]) == 0) {
		Serial << "> Used for managing settings. Options are:" << endl << endl;
		for (int i = 0; i < nSettingsC; i++) {
			Serial << "> " << settingsCommands[i] << endl;
		}
	//help memory
	} else if (strcmp(arg,commands[3]) == 0)
		Serial << "> Displays current system's free memory." << endl;
	//not recognized
	else
		Serial << "> No help found for command <" << arg << ">" << endl;
		
	Serial << endl;
}

void SerialInterface::commandSettings() {
	int aNumber;
	char *arg;

	Serial.println("We're in process_command");
	arg = _cmd.next();
	if (arg != NULL)
	{
		aNumber=atoi(arg);    // Converts a char string to an integer
		Serial.print("First argument was: ");
		Serial.println(aNumber);
	}
	else {
		Serial.println("No arguments");
	}

	arg = _cmd.next();
	if (arg != NULL)
	{
		aNumber=atol(arg);
		Serial.print("Second argument was: ");
		Serial.println(aNumber);
	}
	else {
		Serial.println("No second argument");
	}

}
void SerialInterface::commandSensors() {
	Serial.println("Sensoooooors huh!");
}

void SerialInterface::commandMemory() {
	Serial << endl << "> Available memory: " << freeMemory() << " bytes"<< endl << endl;
}
