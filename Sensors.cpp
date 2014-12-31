#include "Sensors.h"

//Constructors
Sensors::Sensors(Settings *settings) : _settings(settings){     
    pinMode(lightIn, INPUT);
    pinMode(humidIn, INPUT);
    pinMode(waterTrigger, OUTPUT);
    pinMode(waterEcho, INPUT);
    temperature.begin();
    temperature.setResolution(11);
    //Open communication with PH sensor
    Serial2.begin(38400);
    //Set it into reading on-demand mode
    Serial2.print("E\r");
	
    //EC sensor
    Serial1.begin(38400);	
    Serial1.print("E\r");
    //Set to continuous mode (needs 20-25 readings of 1000ms to stabilize reading)
    Serial1.print("C\r");
    
    _iSample = 0;
    //Initiate data arrays
    for (int i = 0; i < _numSamples; i++) {
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
	for (int i = 0; i < _numSamples; i++) {
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
	for (int i = 0; i < _numSamples; i++) {
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

uint16_t Sensors::getLight() const { return _light; }

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

boolean Sensors::ecOffRange() {
	if ((_ph < _settings->getPHalarmDown()) || (_ph > _settings->getPHalarmUp())) 
		return true;
	return false;
}

boolean Sensors::phOffRange() {
	if ((_ec < _settings->getECalarmDown()) || (_ec > _settings->getECalarmUp()))
		return true;
	return false;
}

boolean Sensors::lvlOffRange() {
	if (_waterLevel < _settings->getWaterAlarm())
		return true;
	return false;
}

//Updates sample arrays with readings from sensors and performs smoothing
void Sensors::update() {
	_temps[_iSample] = temp();
	_lights[_iSample] = light();
	_humidities[_iSample] = humidity();
	
	if (_settings->getReservoirModule()) {
		_ecs[_iSample] = ec();
		_phs[_iSample] = ph();
		_waterLevels[_iSample] = waterLevel();
	}

	_iSample++;
	if (_iSample >= _numSamples)
		_iSample = 0;
	
	smoothSensorReadings();
}

//For each sensor data array calculates the average and stores it
void Sensors::smoothSensorReadings() {
	//Temp
	float resF = 0;
	for (int i = 0; i < _numSamples; i++) { resF += _temps[i]; }
	_temp = (float)(resF / _numSamples);
	//Light
	uint16_t res = 0;
	for (int i = 0; i < _numSamples; i++) { res += _lights[i]; }
	_light = (uint16_t)(res / _numSamples);
	//Humidity
	res = 0;
	for (int i = 0; i < _numSamples; i++) { res += _humidities[i]; }
	_humidity = (uint8_t)(res / _numSamples);
	//Electroconductivity
	uint32_t res32 = 0;
	for (int i = 0; i < _numSamples; i++) {  res32 += _ecs[i]; }
	_ec = (uint16_t)(res32 / _numSamples);
	//PH
	resF = 0;
	for (int i = 0; i < _numSamples; i++) { resF += _phs[i]; }
	_ph = (float)(resF / _numSamples);
	//Water level
	res = 0;
	for (int i = 0; i < _numSamples; i++) { res += _waterLevels[i]; }
	_waterLevel = (uint8_t)(res / _numSamples);  
}

//Returns light level 0~100%
uint16_t Sensors::light() {
	//int adc = analogRead(lightIn); 
	//return map(adc, 0, 1023, 0, 100); 
	
	//int rawADC = analogRead(lightIn);
	//const int voltageLvl = 4.3;
	//Code from Billie:
	//https://github.com/BillieBricks/Billie-s-Hydroponic-Controller/blob/master/HydroponicControllerV1.1.0
	//int lightADCReading = analogRead(lightIn);
	// Calculating the voltage of the ADC for light
	//double lightInputVoltage = voltageLvl * ((double)lightADCReading / 1024.0);
	// Calculating the resistance of the photoresistor in the voltage divider
	//double lightResistance = (10.0 * voltageLvl) / lightInputVoltage - 10.0;
	// Calculating the intensity of light in lux
	//uint16_t currentLightInLux = (uint16_t)(255.84 * pow(lightResistance, -10/9));
	//return currentLightInLux;
	
	// Equation to calculate Resistance of LDR, [R-LDR =(R1 (Vin - Vout))/ Vout]
	// Vout = Output voltage from potential Divider. [Vout = ADC * (Vin / 1024)]

	//http://forum.arduino.cc/index.php?topic=141815.0
	const float vcc = 4.3;
	const int resInKohm = 10.0;
	int adc = analogRead(lightIn);
	//Vout = Output voltage from potential Divider. [Vout = adc * (Vin / 1024)]
	float vo = adc * (vcc / 1024);
	//Equation to calculate Resistance of LDR, [R-LDR =(R1 (Vin - Vout))/ Vout]
	float rldr = (resInKohm * (vcc - vo)) / vo;
	uint16_t luxes = (500 / rldr);
	return luxes;
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
	//Calc duration. Timeout of half second
	duration = pulseIn(waterEcho, HIGH,500000);
	distance = (duration / 29) / 2;
	//Serial << "Distance: " << distance << endl;	
	distance = constrain(distance, _settings->getMaxWaterLvl(), _settings->getMinWaterLvl());
	return map(distance, _settings->getMaxWaterLvl(), _settings->getMinWaterLvl(), 100, 0); 
}

//Returns PH level 
//Temp-adjusted reading
//TODO: Redo once PCB works
float Sensors::ph() {   
	if (Serial2.available() > 0) {
		//Convert temperature from float to char*
		//char tempArray[4];
		//dtostrf(_temp,4,2,tempArray);
		//String command = (String)tempArray + "\r";
		//Serial2.print(command);
		//Normal reading
		Serial2.print("R\r");
		//Wait for transmission to end
		Serial2.flush();
		float res = Serial2.parseFloat();
		//Discard carriage return '/r'
		Serial2.read();
		//Make sure data is valid
		if (res < 14)
			return res;
		else 
			return _ph;
	}
	return 0;
}

//Returns EC in uSiemens
//TODO: Redo once PCB works
uint16_t Sensors::ec() {
	if (Serial1.available() > 0) {
 		uint16_t res = Serial1.parseInt();
 		//Serial << "EC: " << res << endl;
 		//Clear buffer of remaining message
 		while (Serial1.peek() != '\r') {
 			Serial1.read();
		}
 		Serial1.read();
		//Sometimes readings spike. We should prevent that
		//Not sure if its from Serial buffer overflowing or noise in cables
		if (res < 20000)
			return res;
		else
			return _ec;	 
	}
	return 0;
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
		Serial1.print(command); 
	}
}

//pH Calibration
void Sensors::setPHcontinuous() {
	Serial2.print("C/r");
	Serial.println("Continuous pH");
}
void Sensors::setPHstandby() {
	Serial2.print("E/r");
	Serial.println("Standby pH");
}
void Sensors::setPHfour() {
	Serial2.print("F/r");
}
void Sensors::setPHseven() {
	Serial2.print("S/r");
}
void Sensors::setPHten() {
	Serial2.print("T/r");
}
//EC Calibration
void Sensors::setECcontinuous() {
	Serial1.print("C/r");
	ecToSerial();
	Serial.println("Continuous EC");
}
void Sensors::setECstandby() {
	Serial1.print("E/r");
	Serial.println("Standby EC");
}
void Sensors::setECprobeType() {
	Serial1.print("P,2/r");
	ecToSerial();
}
void Sensors::setECdry() {
	Serial1.print("Z0/r");
	ecToSerial();
}
void Sensors::setECfortyThousand() {
	Serial1.print("Z40/r");
	ecToSerial();
}
void Sensors::setECtenThousand() {
	Serial1.print("Z10/r");
	ecToSerial();
}

//Output EC circuit's response to serial
void Sensors::ecToSerial() {
	if (_settings->getSerialDebug()) {
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
			Serial.println(sensorString);
		}
	}
}

//Updates RTC internal time
void Sensors::setRTCtime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, int y) {
	tmElements_t t;
	t.Hour = h;
	t.Minute = m;
	t.Second = s;
	t.Day = d;
	t.Month = mo;
	//year argument is offset from 1970
	t.Year = y - 1970;
	time_t time = makeTime(t);
	setTime(time);
	RTC.set(time);
}
