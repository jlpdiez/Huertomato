#include "WinMainScreen.h"

WinMainScreen::WinMainScreen(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinMainScreen::WinMainScreen(const WinMainScreen &other) : Window(other) {
	//_sensors = other._sensors;
	//_settings = other._settings;
}

WinMainScreen& WinMainScreen::operator=(const WinMainScreen &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinMainScreen::~WinMainScreen() {}

void WinMainScreen::draw() {
	_lcd->fillScr(VGA_WHITE);
	printMainHeader();
	printSensorInfo();
	printIconAndStatus();
}

//Prints mainscreen header text and clock
void WinMainScreen::printMainHeader() {
	//For small font y = 6. For big one y = 2;
	const int ySpacer = 2;
	
	//Get actual time
	time_t t = now();
	uint8_t hou = hour(t);
	uint8_t min = minute(t);
	uint8_t sec = second(t);

	printHeaderBackground();
	
	//Header title text
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->print("Huertomato 1.2",10,ySpacer);

	//Clock display HH:MM
	//X is calculated from the end of size
	_lcd->printNumI(hou,_xSize-(5*_bigFontSize)-2,ySpacer,2,'0');
	_lcd->print(":",_xSize-(3*_bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,_xSize-(2*_bigFontSize)-2,ySpacer,2,'0');
}

//Print sensor info on main screen. Data will turn red if theres an alarm triggered
void WinMainScreen::printSensorInfo() {
	const int xSpacer = _xSize - 25;
	const int ySpacer = 35;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//For x coord we take maxSize and extract letterSize*letters plus arbitrary spacing
	//Humidity
	int x = xSpacer-(_bigFontSize*(strlen(sensorText[0])+4));
	_lcd->print(sensorText[0],x,ySpacer);
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,ySpacer,3,' ');
	_lcd->print("%",xSpacer-_bigFontSize,ySpacer);
	//Temp
	x = xSpacer-(_bigFontSize*(strlen(sensorText[1])+7));
	int y = ySpacer+(_bigFontSize+8);
	_lcd->print(sensorText[1],x,y);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	_lcd->print("C",xSpacer-_bigFontSize,y);
	//Light
	x = xSpacer-(_bigFontSize*(strlen(sensorText[2])+4));
	y = ySpacer+(_bigFontSize+8)*2;
	_lcd->print(sensorText[2],x,y);
	_lcd->printNumI(_sensors->getLight(),xSpacer-_bigFontSize*4,y,3);
	_lcd->print("%",xSpacer-_bigFontSize,y);
	//pH
	x = xSpacer-(_bigFontSize*(strlen(sensorText[3])+5));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (ph > _settings->getPHalarmUp() || (ph < _settings->getPHalarmDown()))
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(sensorText[3],x,y);
	_lcd->printNumF(ph,2,xSpacer-_bigFontSize*4,y,'.',4);
	//EC
	x = xSpacer-(_bigFontSize*(strlen(sensorText[4])+7));
	y = ySpacer+(_bigFontSize+8)*4;
	uint16_t ec = _sensors->getEC();
	if (ec > _settings->getECalarmUp() || (ec < _settings->getECalarmDown()))
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(sensorText[4],x,y);
	_lcd->printNumI(ec,xSpacer-_bigFontSize*6,y,4);
	_lcd->print("uS",xSpacer-_bigFontSize*2,y);
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen(sensorText[5])+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (lvl < _settings->getWaterAlarm())
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(sensorText[5],x,y);
	_lcd->printNumI(lvl,xSpacer-_bigFontSize*4,y,3);
	_lcd->print("%",xSpacer-_bigFontSize,y);
}

//Updates main header's clock
void WinMainScreen::updateMainHeader() {
	const int ySpacer = 2;
	
	//Get actual time
	time_t t = now();
	uint8_t hou = hour(t);
	uint8_t min = minute(t);
	uint8_t sec = second(t);
	
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	//Clock display HH:MM
	//X is calculated from the end of size
	_lcd->printNumI(hou,_xSize-(5*_bigFontSize)-2,ySpacer,2,'0');
	_lcd->print(":",_xSize-(3*_bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,_xSize-(2*_bigFontSize)-2,ySpacer,2,'0');
}

//Redraws only values that change over time
//Used for refreshing from main sketch
void WinMainScreen::update() {
	updateMainHeader();
	updateSensorInfo();
	updateIconAndStatus();
}

//Print sensor info on main screen. Data will turn red if theres an alarm triggered
void WinMainScreen::print() {
	const int xSpacer = _xSize - 25;
	const int ySpacer = 35;
	
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);

	//For x coord we take maxSize and extract letterSize*letters plus arbitrary spacing
	//Humidity
	int x = xSpacer-(_bigFontSize*(strlen(sensorText[0])+4));
	_lcd->print(sensorText[0],x,ySpacer);
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,ySpacer,3,' ');
	_lcd->print("%",xSpacer-_bigFontSize,ySpacer);
	
	//Temp
	x = xSpacer-(_bigFontSize*(strlen(sensorText[1])+7));
	int y = ySpacer+(_bigFontSize+8);
	_lcd->print(sensorText[1],x,y);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	_lcd->print("C",xSpacer-_bigFontSize,y);
	
	//Light
	x = xSpacer-(_bigFontSize*(strlen(sensorText[2])+4));
	y = ySpacer+(_bigFontSize+8)*2;
	_lcd->print(sensorText[2],x,y);
	_lcd->printNumI(_sensors->getLight(),xSpacer-_bigFontSize*4,y,3);
	_lcd->print("%",xSpacer-_bigFontSize,y);
	
	//pH
	x = xSpacer-(_bigFontSize*(strlen(sensorText[3])+5));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (ph > _settings->getPHalarmUp() || (ph < _settings->getPHalarmDown()))
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);

	_lcd->print(sensorText[3],x,y);
	_lcd->printNumF(ph,2,xSpacer-_bigFontSize*4,y,'.',4);
	
	//EC
	x = xSpacer-(_bigFontSize*(strlen(sensorText[4])+7));
	y = ySpacer+(_bigFontSize+8)*4;
	uint16_t ec = _sensors->getEC();
	if (ec > _settings->getECalarmUp() || (ec < _settings->getECalarmDown()))
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	
	_lcd->print(sensorText[4],x,y);
	_lcd->printNumI(ec,xSpacer-_bigFontSize*6,y,4);
	_lcd->print("uS",xSpacer-_bigFontSize*2,y);
	
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen(sensorText[5])+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (lvl < _settings->getWaterAlarm())
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	
	_lcd->print(sensorText[5],x,y);
	_lcd->printNumI(lvl,xSpacer-_bigFontSize*4,y,3);
	_lcd->print("%",xSpacer-_bigFontSize,y);
}

//Redraws only sensor numbers and changes colour if needed (alarm triggered)
void WinMainScreen::updateSensorInfo() {
	const int xSpacer = _xSize - 25;
	const int ySpacer = 35;
	
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);

	//Humidity
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,ySpacer,3,' ');
	//Temp
	int y = ySpacer+(_bigFontSize+8);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	//Light
	y = ySpacer+(_bigFontSize+8)*2;
	_lcd->printNumI(_sensors->getLight(),xSpacer-_bigFontSize*4,y,3);
	//pH
	int x = xSpacer-(_bigFontSize*(strlen(sensorText[3])+5));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (ph > _settings->getPHalarmUp() || (ph < _settings->getPHalarmDown()))
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);

	_lcd->print(sensorText[3],x,y);
	_lcd->printNumF(ph,2,xSpacer-_bigFontSize*4,y,'.',4);
	
	//EC
	x = xSpacer-(_bigFontSize*(strlen(sensorText[4])+7));
	y = ySpacer+(_bigFontSize+8)*4;
	uint16_t ec = _sensors->getEC();
	if (ec > _settings->getECalarmUp() || (ec < _settings->getECalarmDown()))
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	
	_lcd->print(sensorText[4],x,y);
	_lcd->printNumI(ec,xSpacer-_bigFontSize*6,y,4);
	_lcd->print("uS",xSpacer-_bigFontSize*2,y);
	
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen(sensorText[5])+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (lvl < _settings->getWaterAlarm())
	_lcd->setColor(red[0],red[1],red[2]);
	else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	
	_lcd->print(sensorText[5],x,y);
	_lcd->printNumI(lvl,xSpacer-_bigFontSize*4,y,3);
	_lcd->print("%",xSpacer-_bigFontSize,y);
}

//Shows system status in main screen
//Loads img files from /PICTURE folder of the SD card
//TODO: make common part of alarm and normal state a function!
void WinMainScreen::printIconAndStatus() {
	const int xSpacer = 10;
	const int ySpacer = 200;
	File img;
	char* path;
	
	//Watering Stopped
	if (_settings->getNightWateringStopped()) {
		_lcd->setColor(grey[0],grey[1],grey[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		_lcd->print("No Watering @ Night   ",xSpacer+_bigFontSize*2,ySpacer);
		path = "/PICTURE/moon126.RAW";
		
		//Timed mode and watering plants
		} else if (_settings->getWaterTimed() && _settings->getWateringPlants()) {
		_lcd->setColor(blue[0],blue[1],blue[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		char* watering = "Huertomato Watering";
		int x = xSpacer + _bigFontSize*2;
		_lcd->print(watering,x,ySpacer);
		//3 blank chars afterwards to clear line
		x += _bigFontSize*strlen(watering);
		_lcd->print("   ",x,ySpacer);
		path = "/PICTURE/logo126.RAW";
		
		//Alarm triggered
		} else if (_settings->getAlarmTriggered()) {
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		_lcd->setColor(red[0],red[1],red[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + _bigFontSize*2;
		//Timed mode
		if (_settings->getWaterTimed()) {
			//Space char added after @ and last number to white out line
			char* nWater = "Next Watering @ ";
			_lcd->print(nWater,x,ySpacer);
			x += _bigFontSize*(strlen(nWater));
			_lcd->printNumI(wHour,x,ySpacer,2,'0');
			x += _bigFontSize*2;
			_lcd->print(":",x,ySpacer);
			x += _bigFontSize;
			_lcd->printNumI(wMin,x,ySpacer,2,'0');
			x += 2*_bigFontSize;
			_lcd->print(" ",x,ySpacer);
			//Continuous mode
			} else {
			char* nWater = "Alarm - Check Solution";
			_lcd->print(nWater,x,ySpacer);
		}
		path = "/PICTURE/alarm126.RAW";
		
		//Normal operation
		} else {
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + _bigFontSize*2;
		
		//Timed mode
		if (_settings->getWaterTimed()) {
			//Space char added after @ and last number to white out line
			char* nWater = "Next Watering @ ";
			_lcd->print(nWater,x,ySpacer);
			x += _bigFontSize*(strlen(nWater));
			_lcd->printNumI(wHour,x,ySpacer,2,'0');
			x += _bigFontSize*2;
			_lcd->print(":",x,ySpacer);
			x += _bigFontSize;
			_lcd->printNumI(wMin,x,ySpacer,2,'0');
			x += 2*_bigFontSize;
			_lcd->print(" ",x,ySpacer);
			
			//Continuous
			} else {
			char* nWater = "System working fine   ";
			_lcd->print(nWater,x,ySpacer);
		}
		path = "/PICTURE/plant126.RAW";
	}
	
	//Read from SD line by line and display.
	//Requires new function at UTFT library
	//Slow but effective
	if (SD.exists(path)) {
		img = SD.open(path,FILE_READ);
		for (int y = 0; y < _iconSize && img.available(); y++) {
			uint16_t buf[_iconSize];
			for (int x = _iconSize - 1; x >= 0; x--) {
				byte l = img.read();
				byte h = img.read();
				buf[x] = ((uint16_t)h << 8) | l;
			}
			_lcd->drawPixelLine(15,y+25+_bigFontSize,_iconSize,buf);
		}
		img.close();
	} else
	_lcd->drawBitmap(15,25+_bigFontSize,_iconSize,_iconSize,logo126);
}

//Same as above except it only changes icon if system state changed from previous
//If not should only update next watering time
void WinMainScreen::updateIconAndStatus() {
	if (_settings->systemStateChanged())
	printIconAndStatus();
	
	//Updates next watering time if needed
	else if (!_settings->getNightWateringStopped() && !_settings->getWateringPlants()
	&& _settings->getWaterTimed()) {
		
		const int xSpacer = 10;
		const int ySpacer = 200;
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		
		//Alarm triggered
		if (_settings->getAlarmTriggered())
		_lcd->setColor(red[0],red[1],red[2]);
		//Normal operation
		else
		_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + _bigFontSize*2;
		//Space char added after @ and last number to white out line
		char* nWater = "Next Watering @ ";
		_lcd->print(nWater,x,ySpacer);
		x += _bigFontSize*(strlen(nWater));
		_lcd->printNumI(wHour,x,ySpacer,2,'0');
		x += _bigFontSize*2;
		_lcd->print(":",x,ySpacer);
		x += _bigFontSize;
		_lcd->printNumI(wMin,x,ySpacer,2,'0');
		x += 2*_bigFontSize;
		_lcd->print(" ",x,ySpacer);
	}
}

Window::Screen WinMainScreen::processTouch(const int x, const int y) { return MainMenu; }