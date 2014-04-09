#include "Sensors.h"
//TODO: Update via MOD

//Constructors
Sensors::Sensors(Settings *settings) : _settings(settings){     
    pinMode(lightIn, INPUT);
    pinMode(humidIn, INPUT);
    pinMode(waterTrigger, OUTPUT);
    pinMode(waterEcho, INPUT);
    temperature.begin();
    temperature.setResolution(11);
    //Open communication with PH sensor
    Serial1.begin(38400);
    //Set it into reading on-demand mode
    Serial1.print("E\r");
    //EC sensor
    Serial2.begin(38400);
    Serial2.print("E\r");
    //Set to continuous mode (needs 20-25 readings of 1000ms to stabilize reading)
    Serial2.print("C\r");
    
    _iSample = 0;
    //Initiate data arrays
    for (int i = 0; i < numSamples; i++) {
      _temps[i] = 0;
      _lights[i] = 0;
      _humidities[i] = 0;
      _ecs[i] = 0;
      _phs[i] = 0;
      _waterLevels[i] = 0;
    }
}

Sensors::Sensors(const Sensors &other) {
	_settings = other._settings;
	_iSample = other._iSample;
	for (int i = 0; i < numSamples; i++) {
		_temps[i] = other._temps[i];
		_lights[i] = other._lights[i];
		_humidities[i] = other._humidities[i];
		_ecs[i] = other._ecs[i];
		_phs[i] = other._phs[i];
		_waterLevels[i] = other._waterLevels[i];
	}
	_temp = other._temp;
	_light = other._light;
	_humidity = other._humidity;
	_ec = other._ec;
	_ph = other._ph;
	_waterLevel = other._waterLevel;	
}

Sensors& Sensors::operator=(const Sensors &other) {
	_settings = other._settings;
	_iSample = other._iSample;
	for (int i = 0; i < numSamples; i++) {
		_temps[i] = other._temps[i];
		_lights[i] = other._lights[i];
		_humidities[i] = other._humidities[i];
		_ecs[i] = other._ecs[i];
		_phs[i] = other._phs[i];
		_waterLevels[i] = other._waterLevels[i];
	}
	_temp = other._temp;
	_light = other._light;
	_humidity = other._humidity;
	_ec = other._ec;
	_ph = other._ph;
	_waterLevel = other._waterLevel;
	
	return *this;	
}

Sensors::~Sensors() {}

//Getters
float Sensors::getTemp() const { return _temp; }

uint8_t Sensors::getLight() const { return _light; }

uint8_t Sensors::getHumidity() const { return _humidity; }

uint16_t Sensors::getEC() const { return _ec; }

float Sensors::getPH() const { return _ph; }

uint8_t Sensors::getWaterLevel() const { return _waterLevel; }
	
//Polls sonic range sensor and returns raw reading
uint16_t Sensors::getRawWaterLevel(){
	digitalWrite(waterTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(waterTrigger, HIGH);
	delayMicroseconds(5);
	digitalWrite(waterTrigger, LOW);

	return (pulseIn(waterEcho, HIGH) / 29) / 2;
}

//TODO: Convert to LUX
//#include <Streaming.h>
//Returns light raw data
uint16_t Sensors::getRawLight() {
	int rawADC = analogRead(lightIn);
	//float volts = (rawADC * 5.0) / 1023;
	//Serial << rawADC << ", " << volts << endl;
	return analogRead(lightIn);
}

//Updates sample arrays with readings from sensors and performs smoothing
//TODO: Another form of keeping track needed?
void Sensors::updateMain() {
	_temps[_iSample] = temp();
	_lights[_iSample] = light();
	_humidities[_iSample] = humidity();
	/*_ecs[_iSample] = ec();
	_phs[_iSample] = ph();
	_waterLevels[_iSample] = waterLevel();*/
	
	_iSample++;
	if (_iSample >= numSamples)
		_iSample =0;
	
	smoothSensorReadings();
}

void Sensors::updateReservoir() {
	_ecs[_iSample] = ec();
	_phs[_iSample] = ph();
	_waterLevels[_iSample] = waterLevel();
}

//For each sensor data array calculates the average and stores it
void Sensors::smoothSensorReadings() {
	//Temp
	float resF = 0;
	for (int i = 0; i < numSamples; i++) { resF += _temps[i]; }
	_temp = (float)(resF / numSamples);
	//Light
	uint16_t res = 0;
	for (int i = 0; i < numSamples; i++) { res += _lights[i]; }
	_light = (uint8_t)(res / numSamples);
	//Humidity
	res = 0;
	for (int i = 0; i < numSamples; i++) { res += _humidities[i]; }
	_humidity = (uint8_t)(res / numSamples);
	//Electroconductivity
	res = 0;
	for (int i = 0; i < numSamples; i++) {  res += _ecs[i]; }
	_ec = (uint16_t)(res / numSamples);
	//PH
	resF = 0;
	for (int i = 0; i < numSamples; i++) { resF += _phs[i]; }
	_ph = (float)(resF / numSamples);
	//Water level
	res = 0;
	for (int i = 0; i < numSamples; i++) { res += _waterLevels[i]; }
	_waterLevel = (uint8_t)(res / numSamples);  
}

//Returns light level 0~100%
uint8_t Sensors::light() {
	int adc = analogRead(lightIn); 
	return map(adc, 0, 1023, 0, 100); 
	/* Code from Billie
	lightADCReading = analogRead(lightSensor);
	// Calculating the voltage of the ADC for light
	lightInputVoltage = 5.0 * ((double)lightADCReading / 1024.0);
	// Calculating the resistance of the photoresistor in the voltage divider
	lightResistance = (10.0 * 5.0) / lightInputVoltage - 10.0;
	// Calculating the intensity of light in lux       
	currentLightInLux = 255.84 * pow(lightResistance, -10/9);*/
}

//Returns temp in Celsius
float Sensors::temp() {
	temperature.requestTemperatures();
	return temperature.getTempCByIndex(0);
}

//Returns humidity percentage (-1 for error)
uint8_t Sensors::humidity() {
	if (DHT11.read(humidIn) == DHTLIB_OK)
		return DHT11.humidity;
	else
		return 0;
}

//Returns water reservoir % level
uint8_t Sensors::waterLevel() {
	int duration, distance;
	digitalWrite(waterTrigger, LOW);
	delayMicroseconds(2);
	digitalWrite(waterTrigger, HIGH);
	delayMicroseconds(5);
	digitalWrite(waterTrigger, LOW);
	duration = pulseIn(waterEcho, HIGH);
	distance = (duration / 29) / 2;
	//Serial << "Distance: " << distance << endl;	
	distance = constrain(distance, _settings->getMaxWaterLvl(), _settings->getMinWaterLvl());
	return map(distance, _settings->getMaxWaterLvl(), _settings->getMinWaterLvl(), 100, 0); 
}

//Returns PH level 
float Sensors::ph() {   
	//Temp-adjusted reading
	//Convert temperature from float to char*
	char tempArray[4];
	dtostrf(_temp,4,2,tempArray);
	String command = (String)tempArray + "\r";
	//Serial << "Ph command: " << command << endl;
	Serial1.print(command);
	//Normal reading
	//Serial1.print("R\r");
	if (Serial1.available() > 0) {
		String sensorString = ""; 
		sensorString.reserve(30);
		//Read data from sensor
		while (Serial1.peek() != '\r') {
			char inchar = (char)Serial1.read();                            
			sensorString += inchar;           
		}
		//Discard <CR>
		Serial1.read(); 
		//Convert string to float and return
		char sensorArray[sensorString.length() + 1];
		sensorString.toCharArray(sensorArray, sizeof(sensorArray));
		//Serial << "PH read: " << sensorArray << endl;
		return atof(sensorArray);
	} 
	else { return 0.0; }
}

//TODO: Error checking should be better implemented
//Returns EC in uSiemens
uint16_t Sensors::ec() {
  //As EC readings are continuous we can get two types of errors when reading from arduino
  //We can have a string with more than 2 commas and we can have a number too large to be valid data 
  if (Serial2.available() > 0) {
    //Format is: uSiemens,PPM,Salinity<CR>
    String sensorString = ""; 
    sensorString.reserve(30);
    //Read data from sensor
    char inchar;
    while (Serial2.peek() != '\r') {
      inchar = (char)Serial2.read();    
      sensorString += inchar;           
    }
    //Serial << "String given by EC: " << sensorString << endl;
    //Discard <CR>
    Serial2.read();
    //Count number of ","
    int comma = 0;
    int index = 0;
    while (sensorString.indexOf(',',index+1) != -1) {
      index = sensorString.indexOf(',',index+1);
      comma++;  
    }
    //If comma != 2 we have read buffer while sensor was sending data
    if (comma == 2) {
      //uSiemens
      int firstComma = sensorString.indexOf(',');
      String uSiemens = sensorString.substring(0,firstComma);
      //PPM / TDS
      //int secondComma = sensorString.indexOf(',',firstComma+1);
      //String ppm = sensorString.substring(firstComma+1,secondComma);
      //Salinity
      //String salinity = sensorString.substring(secondComma+1,sensorString.length());
      //Convert string to int and return
      char uSiemensArray[uSiemens.length() + 1];
      uSiemens.toCharArray(uSiemensArray, sizeof(uSiemensArray));      
      //Serial << "EC in uS: " << atol(uSiemensArray) << endl;
      uint16_t uS = atol(uSiemensArray);
      //we dont accept readings that are clearly off charts
      if (uS > 10000) {
        //Serial << "Number too big: " << sensors.ec << endl;
        return _ec;
      }
      //We received valid data
      else
        return atol(uSiemensArray);
    } 
    else { 
      //Serial << "Too many commas: " << sensors.ec << endl;
      return _ec; 
    }
  } 
  //We have read buffer before sensor sent any data
  else { 
    //Serial << "Buffer empty: " << sensors.ec << endl;  
    return _ec; 
  }
}

//Sends command to EC sensor to adjust readings to temperature
void Sensors::adjustECtemp() {
	//Serial << "Adjusting EC to temp: ";
	if (_temp != 0) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(_temp,4,2,tempArray);
		String command = (String)tempArray + ",C\r";
		//Serial << command << endl;
		Serial2.print(command); 
	}
}

//pH Calibration
void Sensors::setPHcontinuous() {
	String command = "C/r";
	Serial1.print(command);
	phToSerial();
}

void Sensors::setPHstandby() {
	Serial1.print("E/r");
	phToSerial();
}

void Sensors::setPHfour() {
	Serial1.print("F/r");
	phToSerial();
}

void Sensors::setPHseven() {
	Serial1.print("S/r");
	phToSerial();
}

void Sensors::setPHten() {
	Serial1.print("T/r");
	phToSerial();
}

//EC Calibration
void Sensors::setProbeType() {
	Serial2.print("P,2/r");
	ecToSerial();
}

void Sensors::setECdry() {
	Serial2.print("Z0/r");
	ecToSerial();
}

void Sensors::setECtenThousand() {
	Serial2.print("Z10/r");
	ecToSerial();
}

void Sensors::setECfortyThousand() {
	Serial2.print("Z40/r");
	ecToSerial();
}

//Prints to Serial(if active) pH circuit's response to commands
void Sensors::phToSerial() {
	if (_settings->getSerialDebug()) {
		while (Serial1.available() <= 0) {}
		if (Serial1.available() > 0) {
			String sensorString = "";
			sensorString.reserve(30);
			//Read data from sensor
			char inchar;
			while (Serial1.peek() != '\r') {
				inchar = (char)Serial1.read();
				sensorString += inchar;
			}
			//Discard <CR>
			Serial1.read();
			Serial << sensorString << endl;
		}
	}
}

//Same as above but for EC circuit
void Sensors::ecToSerial() {
	if (_settings->getSerialDebug()) {
		if (Serial2.available() > 0) {
			String sensorString = "";
			sensorString.reserve(30);
			//Read data from sensor
			char inchar;
			while (Serial2.peek() != '\r') {
				inchar = (char)Serial2.read();
				sensorString += inchar;
			}
			//Discard <CR>
			Serial2.read();
			Serial << sensorString << endl;
		}
	}
}

