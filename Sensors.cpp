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
    for (uint8_t i = 0; i < _numSamples; i++) {
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
	for (uint8_t i = 0; i < _numSamples; i++) {
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
	for (uint8_t i = 0; i < _numSamples; i++) {
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

boolean Sensors::phOffRange() {
	if ((_ph < _settings->getPHalarmDown()) || (_ph > _settings->getPHalarmUp())) 
		return true;
	return false;
}

boolean Sensors::ecOffRange() {
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
	for (uint8_t i = 0; i < _numSamples; i++) { resF += _temps[i]; }
	_temp = (float)(resF / _numSamples);
	//Light
	uint16_t res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _lights[i]; }
	_light = (uint16_t)(res / _numSamples);
	//Humidity
	res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _humidities[i]; }
	_humidity = (uint8_t)(res / _numSamples);
	//Electroconductivity
	uint32_t res32 = 0;
	for (uint8_t i = 0; i < _numSamples; i++) {  res32 += _ecs[i]; }
	_ec = (uint16_t)(res32 / _numSamples);
	//PH
	resF = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { resF += _phs[i]; }
	_ph = (float)(resF / _numSamples);
	//Water level
	res = 0;
	for (uint8_t i = 0; i < _numSamples; i++) { res += _waterLevels[i]; }
	_waterLevel = (uint8_t)(res / _numSamples);  
}

//Returns light level in luxes
uint16_t Sensors::light() {
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

//Returns a PH reading. Reading takes 378ms
float Sensors::ph() {   
	if (!_calibratingPh) {
		Serial2.print("R\r");
		if (Serial2.available() > 0) {
			float res = Serial2.parseFloat();
			//Discard carriage return '/r'
			Serial2.read();
			return res;
		}
		//There's some kind of problem communicating with circuit
		return 0;
	} else
		return _ph;
	
}

//Returns EC in uSiemens. Reading takes 1000ms
uint16_t Sensors::ec() {
	//If sensors being calibrated we return previous values
	if (!_calibratingEc) {
		if (Serial1.available() > 0) {
 			uint16_t res = Serial1.parseInt();
 			//Clear buffer of remaining messages
			while (Serial1.available() > 0)
				Serial1.read();
			return res;
		}
		//Buffer has been emptied before and circuit still hasn't put data into it again
		return 0;
	} else
		return _ec;
}

//This should be set while calibrating to prevent messing up circuits if update() or adjustTemp() called
void Sensors::calibratingPH(boolean c) {
	_calibratingPh = c;
}

void Sensors::calibratingEC(boolean c) {
	_calibratingEc = c;
}

void Sensors::clearPHbuffer() {
 	while (Serial2.available() > 0)
 		Serial2.read();
}

void Sensors::clearECbuffer() {
	while (Serial1.available() > 0)
		Serial1.read();
}

//pH circuit commands
void Sensors::resetPH() {
	clearPHbuffer();
	Serial2.print("X\r");
	//Leave some time for circuit reset and for data to be received
	delay(100);
	phToSerial();
}

void Sensors::getPHinfo() {
	clearPHbuffer();
	Serial2.print("I\r");
	//Leave some time for data to be received
	delay(10);
	phToSerial();
}

void Sensors::setPHled(boolean state) {
	if (state)
		Serial2.print("L1\r");
	else
		Serial2.print("L0\r");
}

void Sensors::setPHcontinuous() {
	Serial2.print("C\r");
}

void Sensors::setPHstandby() {
	Serial2.print("E\r");
}

void Sensors::setPHfour() {
	Serial2.print("F\r");
	if (_settings->getSerialDebug())
		Serial.println(4.00);
}

void Sensors::setPHseven() {
	Serial2.print("S\r");
	if (_settings->getSerialDebug())
		Serial.println(7.00);
}

void Sensors::setPHten() {
	Serial2.print("T\r");
	if (_settings->getSerialDebug())
		Serial.println(10.00);
}

//Sends command to pH sensor to adjust readings to temperature only if sensor not being calibrated
void Sensors::adjustPHtemp() {
	if ((_temp != 0) && (!_calibratingPh)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(_temp,4,2,tempArray);
		String command = (String)tempArray + "\r";
		Serial2.print(command);
	}
}

//EC circuit commands
void Sensors::resetEC() {
	clearECbuffer();
	Serial1.print("X\r");
	delay(3000);
	ecToSerial();
}

void Sensors::getECinfo() {
	clearECbuffer();
	Serial1.print("I\r");
	delay(1450);
	ecToSerial();
}

void Sensors::setECled(boolean state) {
	if (state)
		Serial1.print("L1\r");
	else
		Serial1.print("L0\r");
}

void Sensors::setECcontinuous() {
	Serial1.print("C\r");
}

void Sensors::setECstandby() {
	Serial1.print("E\r");
}

void Sensors::setECprobeType() {
	Serial1.print("P,2\r");
	if (_settings->getSerialDebug())
		Serial.println("k1.0");
}

void Sensors::setECdry() {
	Serial1.print("Z0\r");
	if (_settings->getSerialDebug())
		Serial.println("dry cal");
}

void Sensors::setECfortyThousand() {
	Serial1.print("Z40\r");
	if (_settings->getSerialDebug())
		Serial.println("40,000 uS cal");
}

void Sensors::setECtenThousand() {
	Serial1.print("Z10\r");
	if (_settings->getSerialDebug())
		Serial.println("10,500 uS cal");
}

//Sends command to EC sensor to adjust readings to temperature if not calibrating sensor
void Sensors::adjustECtemp() {
	if ((_temp != 0) && (!_calibratingEc)) {
		//Convert temp from float to char*
		char tempArray[4];
		dtostrf(_temp,4,2,tempArray);
		String command = (String)tempArray + ",C\r";
		Serial1.print(command);
	}
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

//Output pH circuit's response to serial
void Sensors::phToSerial() {
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
