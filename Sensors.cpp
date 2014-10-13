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
	if (_iSample >= numSamples)
		_iSample =0;
	
	smoothSensorReadings();
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
	uint32_t res32 = 0;
	for (int i = 0; i < numSamples; i++) {  res32 += _ecs[i]; }
	_ec = (uint16_t)(res32 / numSamples);
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
//Temp-adjusted reading
float Sensors::ph() {   
	//Convert temperature from float to char*
	//char tempArray[4];
	//dtostrf(_temp,4,2,tempArray);
	//String command = (String)tempArray + "\r";
	//Serial1.print(command);
	//Normal reading
	Serial1.print("R\r");
	//Wait for transmission to end
	Serial1.flush();
	float res = Serial1.parseFloat();
	//Discard carriage return '/r'
	Serial1.read();
	//Make sure data is valid
	if (res < 14)
		return res;
	else 
		return _ph;
}

//Returns EC in uSiemens
uint16_t Sensors::ec() {
 	uint16_t res = Serial2.parseInt();
 	//Serial << "EC: " << res << endl;
 	//Clear buffer of remaining message
 	while (Serial2.peek() != '\r') {
 		Serial2.read();
	 }
 	Serial2.read();
	//Sometimes readings spike. We should prevent that
	//Not sure if its from Serial buffer overflowing or noise in cables
	if (res < 20000)
		return res;
	else
		return _ec;	 
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
