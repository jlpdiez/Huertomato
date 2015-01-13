#include "WinMainScreen.h"

WinMainScreen::WinMainScreen(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) {}

WinMainScreen::WinMainScreen(const WinMainScreen &other) : Window(other) {}
WinMainScreen& WinMainScreen::operator=(const WinMainScreen &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinMainScreen::~WinMainScreen() {}
	
Window::Screen WinMainScreen::getType() const {
	return Window::MainScreen;
}

void WinMainScreen::draw() {
	_lcd->fillScr(VGA_WHITE);
	printMainHeader();
	(_settings->getReservoirModule()) ? printInfoReservoir() : printInfoNoModule();
	printIconAndStatus();
}

//Refreshes minimun or redraws if status of system has changed
void WinMainScreen::update() {
	if (!_settings->moduleChanged()) {
		updateMainHeader();
		(_settings->getReservoirModule()) ? updateInfoReservoir() : updateInfoNoModule();
		updateIconAndStatus();
	} else 
		draw();
}
 
//Prints mainscreen header text and clock
void WinMainScreen::printMainHeader() {
	const uint8_t ySpacer = 2;
	printHeaderBackground();
	//Header title text
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->print(pmChar(versionName),10,ySpacer);
	updateMainHeader();
}

//Updates main header's clock
void WinMainScreen::updateMainHeader() {
	const uint8_t ySpacer = 2;
	
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
	_lcd->print(pmChar(timeSeparator),_xSize-(3*_bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,_xSize-(2*_bigFontSize)-2,ySpacer,2,'0');
}

//Print sensor info on main screen. Data will turn red if theres an alarm triggered
void WinMainScreen::printInfoReservoir() {
	const int xSpacer = _xSize - 25;
	const int ySpacer = 35;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//For x coord we take maxSize and extract letterSize*letters plus arbitrary spacing
	//Humidity
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorText[0])+4));
	_lcd->print(pmChar(sensorText[0]),x,ySpacer);
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,ySpacer,3,' ');
	_lcd->print(pmChar(percentSign),xSpacer-_bigFontSize,ySpacer);
	//Temp
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[1])+7));
	int y = ySpacer+(_bigFontSize+8);
	_lcd->print(pmChar(sensorText[1]),x,y);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	_lcd->print(pmChar(tempUnit),xSpacer-_bigFontSize,y);
	//Light
	x = xSpacer-(_bigFontSize*(11 + strlen_P(lightUnit)));
	y = ySpacer+(_bigFontSize+8)*2;
	_lcd->print(pmChar(sensorText[2]),x,y);
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	_lcd->print(pmChar(lightUnit),xSpacer-(_bigFontSize * strlen_P(lightUnit)),y);
	//pH
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[3])+6));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (_sensors->phOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
		_lcd->print(pmChar(sensorText[3]),x,y);
		_lcd->printNumF(ph,2,xSpacer-_bigFontSize*5,y,'.',5);
	//EC
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[4])+7));
	y = ySpacer+(_bigFontSize+8)*4;
	uint16_t ec = _sensors->getEC();
	if (_sensors->ecOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(pmChar(sensorText[4]),x,y);
	_lcd->printNumI(ec,xSpacer-_bigFontSize*7,y,5);
	_lcd->print(pmChar(ecUnit),xSpacer-_bigFontSize*2,y);
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[5])+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (_sensors->lvlOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
		_lcd->print(pmChar(sensorText[5]),x,y);
		_lcd->printNumI(lvl,xSpacer-_bigFontSize*4,y,3);
		_lcd->print(pmChar(percentSign),xSpacer-_bigFontSize,y);
}

//Redraws only sensor numbers and changes colour if needed (alarm triggered)
void WinMainScreen::updateInfoReservoir() {
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
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	//pH
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorText[3])+6));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (_sensors->phOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
	
	_lcd->print(pmChar(sensorText[3]),x,y);
	_lcd->printNumF(ph,2,xSpacer-_bigFontSize*5,y,'.',5);	
	//EC
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[4])+7));
	y = ySpacer+(_bigFontSize+8)*4;
	uint16_t ec = _sensors->getEC();
	if (_sensors->ecOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(pmChar(sensorText[4]),x,y);
	_lcd->printNumI(ec,xSpacer-_bigFontSize*7,y,5);
	_lcd->print(pmChar(ecUnit),xSpacer-_bigFontSize*2,y);	
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[5])+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (_sensors->lvlOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);	
	_lcd->print(pmChar(sensorText[5]),x,y);
	_lcd->printNumI(lvl,xSpacer-_bigFontSize*4,y,3);
	_lcd->print(pmChar(percentSign),xSpacer-_bigFontSize,y);
}

void WinMainScreen::printInfoNoModule() {
	const int xSpacer = _xSize - 25;
	const int ySpacer = 35;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Temp
	int y = 35;
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorText[1])+7));
	_lcd->print(pmChar(sensorText[1]),x,y);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	_lcd->print(pmChar(tempUnit),xSpacer-_bigFontSize,y);
	//Light
	x = xSpacer-(_bigFontSize*(11 + strlen_P(lightUnit)));
	y +=  _bigFontSize + 45;
	_lcd->print(pmChar(sensorText[2]),x,y);
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	_lcd->print(pmChar(lightUnit),xSpacer-(_bigFontSize * strlen_P(lightUnit)),y);
	//Humidity
	y = ySpacer+(_bigFontSize+8)*5;
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[0])+4));
	_lcd->print(pmChar(sensorText[0]),x,y);
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,y,3,' ');
	_lcd->print(pmChar(percentSign),xSpacer-_bigFontSize,y);
}

void WinMainScreen::updateInfoNoModule() {
	const int xSpacer = _xSize - 25;
	const int ySpacer = 35;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Temp
	int y = 35;
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorText[1])+7));
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	//Light
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[2])+4));
	y +=  _bigFontSize + 45;
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	//Humidity
	y = ySpacer+(_bigFontSize+8)*5;
	x = xSpacer-(_bigFontSize*(strlen_P(sensorText[0])+4));
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,y,3,' ');
}



//Shows system status in main screen
//Loads img files from /PICTURE folder of the SD card
//This one is shared by two windows, it places the icon in different places using _settings->getReservoirModule()
void WinMainScreen::printIconAndStatus() {
	int xSpacer = 10;
	int ySpacer = 200;
	File img;
	char* path;
	
	//Watering Stopped
	if (_settings->getNightWateringStopped()) {
		_lcd->setColor(grey[0],grey[1],grey[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print(pmChar(bulletStr),xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		_lcd->print(pmChar(noNight),xSpacer+_bigFontSize*2,ySpacer);
		path = pmChar(nightPath);
	//Timed mode and watering plants
	} else if (_settings->getWaterTimed() && _settings->getWateringPlants()) {
		_lcd->setColor(blue[0],blue[1],blue[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print(pmChar(bulletStr),xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal); 
		int x = xSpacer + _bigFontSize*2;
		_lcd->print(pmChar(htmtWatering),x,ySpacer);
		x += _bigFontSize*strlen_P(htmtWatering);
		path = pmChar(logoPath);
	//Normal or alarm modes
	} else {
		boolean alarm = _settings->getAlarmTriggered();
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		(alarm) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print(pmChar(bulletStr),xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + _bigFontSize*2;
		
		//Timed mode
		if (_settings->getWaterTimed()) {
			_lcd->print(pmChar(nextWater),x,ySpacer);
			x += _bigFontSize*(strlen_P(nextWater));
			_lcd->printNumI(wHour,x,ySpacer,2,'0');
			x += _bigFontSize*2;
			_lcd->print(pmChar(timeSeparator),x,ySpacer);
			x += _bigFontSize;
			_lcd->printNumI(wMin,x,ySpacer,2,'0');
			x += 2*_bigFontSize;
			_lcd->print(pmChar(spaceChar),x,ySpacer);		
		//Continuous
		} else
			(alarm) ? _lcd->print(pmChar(alarmCont),x,ySpacer) : _lcd->print(pmChar(normalCont),x,ySpacer);
		//Path to image
		(alarm) ? path = pmChar(alarmPath) : path = pmChar(plantPath);
	}
	
	//Read from SD line by line and display icon.
	//Requires new function at UTFT library
	//Slow but effective
	xSpacer = 15;
	ySpacer = 25 + _bigFontSize;
	if (SD.exists(path)) {
		img = SD.open(path,FILE_READ);
		for (int y = 0; y < _bigIconSize && img.available(); y++) {
			uint16_t buf[_bigIconSize];
			for (int x = _bigIconSize - 1; x >= 0; x--) {
				byte l = img.read();
				byte h = img.read();
				buf[x] = ((uint16_t)h << 8) | l;
			}
			_lcd->drawPixelLine(xSpacer,ySpacer+y,_bigIconSize,buf);
		}
		img.close();
	} else
		_lcd->drawBitmap(xSpacer,ySpacer,_bigIconSize,_bigIconSize,logo126);
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
		
		_lcd->print(pmChar(bulletStr),xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + _bigFontSize*2;
		_lcd->print(pmChar(nextWater),x,ySpacer);
		x += _bigFontSize*(strlen_P(nextWater));
		_lcd->printNumI(wHour,x,ySpacer,2,'0');
		x += _bigFontSize*2;
		_lcd->print(pmChar(timeSeparator),x,ySpacer);
		x += _bigFontSize;
		_lcd->printNumI(wMin,x,ySpacer,2,'0');
		x += 2*_bigFontSize;
		_lcd->print(pmChar(spaceChar),x,ySpacer);
	}
}

Window::Screen WinMainScreen::processTouch(const int x, const int y) { 
	return MainMenu; 
}