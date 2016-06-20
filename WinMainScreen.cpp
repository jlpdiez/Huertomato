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
 
//Prints mainscreen header name and version
void WinMainScreen::printMainHeader() {
	printHeaderBackground();
	//Header title text
	_lcd->setFont(hallfetica_normal);
	//_lcd->setFont(Sinclair_S);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	int x = 10;
	_lcd->print(pmChar(htmtTxt),x,_headerTextY);
	x += (strlen_P(htmtTxt)+1)*_bigFontSize;
	_lcd->printNumF(versionNumber,1,x,_headerTextY);
	updateMainHeader();
}

//Updates main header's clock
void WinMainScreen::updateMainHeader() {	
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
	_lcd->printNumI(hou,_xSize-(5*_bigFontSize)-2,_headerTextY,2,'0');
	_lcd->print(pmChar(timeSeparator),_xSize-(3*_bigFontSize)-2,_headerTextY);
	_lcd->printNumI(min,_xSize-(2*_bigFontSize)-2,_headerTextY,2,'0');
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
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr0)+4));
	_lcd->print(pmChar(sensorTextStr0),x,ySpacer);
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,ySpacer,3,' ');
	_lcd->print(pmChar(percentSign),xSpacer-_bigFontSize,ySpacer);
	//Temp
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr1)+7));
	int y = ySpacer+(_bigFontSize+8);
	_lcd->print(pmChar(sensorTextStr1),x,y);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	if (_settings->getCelsius())
		_lcd->print(pmChar(tempUnitC),xSpacer-_bigFontSize,y);
	else
		_lcd->print(pmChar(tempUnitF),xSpacer-_bigFontSize,y);
	//Light
	x = xSpacer-(_bigFontSize*(11 + strlen_P(lightUnit)));
	y = ySpacer+(_bigFontSize+8)*2;
	_lcd->print(pmChar(sensorTextStr2),x,y);
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	_lcd->print(pmChar(lightUnit),xSpacer-(_bigFontSize * strlen_P(lightUnit)),y);
	//pH
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr3)+6));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (_sensors->phOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
		_lcd->print(pmChar(sensorTextStr3),x,y);
		_lcd->printNumF(ph,2,xSpacer-_bigFontSize*5,y,'.',5);
	//EC
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr4)+7));
	y = ySpacer+(_bigFontSize+8)*4;
	float ec = _sensors->getEC();
	if (_sensors->ecOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(pmChar(sensorTextStr4),x,y);
	_lcd->printNumF(ec,2,xSpacer-_bigFontSize*7,y,'.',5);
	_lcd->print(pmChar(ecUnit),xSpacer-_bigFontSize*2,y);
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr5)+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (_sensors->lvlOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
		_lcd->print(pmChar(sensorTextStr5),x,y);
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
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr3)+6));
	y = ySpacer+(_bigFontSize+8)*3;
	float ph = _sensors->getPH();
	if (_sensors->phOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
	
	_lcd->print(pmChar(sensorTextStr3),x,y);
	_lcd->printNumF(ph,2,xSpacer-_bigFontSize*5,y,'.',5);	
	//EC
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr4)+7));
	y = ySpacer+(_bigFontSize+8)*4;
	float ec = _sensors->getEC();
	if (_sensors->ecOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->print(pmChar(sensorTextStr4),x,y);
	_lcd->printNumF(ec,2,xSpacer-_bigFontSize*7,y,'.',5);
	_lcd->print(pmChar(ecUnit),xSpacer-_bigFontSize*2,y);	
	//Deposit level
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr5)+4));
	y = ySpacer+(_bigFontSize+8)*5;
	uint8_t lvl = _sensors->getWaterLevel();
	if (_sensors->lvlOffRange())
		_lcd->setColor(red[0],red[1],red[2]);
	else
		_lcd->setColor(grey[0], grey[1], grey[2]);	
	_lcd->print(pmChar(sensorTextStr5),x,y);
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
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr1)+7));
	_lcd->print(pmChar(sensorTextStr1),x,y);
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	if (_settings->getCelsius())
		_lcd->print(pmChar(tempUnitC),xSpacer-_bigFontSize,y);
	else
		_lcd->print(pmChar(tempUnitF),xSpacer-_bigFontSize,y);
	//Light
	x = xSpacer-(_bigFontSize*(11 + strlen_P(lightUnit)));
	y +=  _bigFontSize + 45;
	_lcd->print(pmChar(sensorTextStr2),x,y);
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	_lcd->print(pmChar(lightUnit),xSpacer-(_bigFontSize * strlen_P(lightUnit)),y);
	//Humidity
	y = ySpacer+(_bigFontSize+8)*5;
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr0)+4));
	_lcd->print(pmChar(sensorTextStr0),x,y);
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
	int x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr1)+7));
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-_bigFontSize*6,y,'.',5);
	//Light
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr2)+4));
	y +=  _bigFontSize + 45;
	_lcd->printNumI(_sensors->getLight(),xSpacer-(_bigFontSize * (4 + strlen_P(lightUnit))),y,4);
	//Humidity
	y = ySpacer+(_bigFontSize+8)*5;
	x = xSpacer-(_bigFontSize*(strlen_P(sensorTextStr0)+4));
	_lcd->printNumI(_sensors->getHumidity(),xSpacer-_bigFontSize*4,y,3,' ');
}

//Prints status line in main screen
//Input var expects a pointer to PROGMEM
void WinMainScreen::printStatus(const char* msg) {
	int xSpacer = 10;
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print(pmChar(bulletStr),xSpacer,_statusTextY);
	_lcd->setFont(hallfetica_normal);
	int x = xSpacer + _bigFontSize*2;
	_lcd->print(pmChar(msg),x,_statusTextY);
	uint8_t length = strlen_P(msg);
	x += (length)*_bigFontSize;
	for (uint8_t i = length; i < _statusLength; i++) {
		_lcd->print(pmChar(spaceChar),x,_statusTextY);
		x += _bigFontSize;
	}
	
}

//Prints the next watering hour in status line
void WinMainScreen::printNextWaterHour() {
	int x = _statusTextX + _bigFontSize*(strlen_P(nextWater)+3);
	int y = _statusTextY - 1;
	int wHour = _settings->getNextWhour();
	int wMin = _settings->getNextWminute();
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(hallfetica_normal);
	(_settings->getAlarmTriggered()) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
	//Next watering time
	_lcd->printNumI(wHour,x,y,2,'0');
	x += _bigFontSize*2;
	_lcd->print(pmChar(timeSeparator),x,y);
	x += _bigFontSize;
	_lcd->printNumI(wMin,x,y,2,'0');
}

//Shows system status in main screen
//Loads img files from /PICTURE folder of the SD card
void WinMainScreen::printIconAndStatus() {
	File img;
	char* path;
	boolean alarm = _settings->getAlarmTriggered();
	boolean pumpOff = _settings->getPumpProtected();
	boolean nightStopped =_settings->getNightWateringStopped();
	boolean waterTimed = _settings->getWaterTimed();
	boolean watering = _settings->getWateringPlants();
	
	//In order of precedence. If first one active all else are hidden and so forth
	//Watering OFF, nutrient level critical, pump off
	if (pumpOff) {
		_lcd->setColor(red[0],red[1],red[2]);
		printStatus(pumpCont);
		path = pmChar(alarmPath);
	//Night watering stopped & no pump protection triggered
	} else if (nightStopped) {
		(alarm) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(grey[0],grey[1],grey[2]);
		printStatus(noNight);
		path = pmChar(nightPath);
	//Timed mode and watering plants
	} else if (waterTimed && watering) {
		_lcd->setColor(blue[0],blue[1],blue[2]);
		printStatus(htmtWatering);
		path = pmChar(logoPath);
	//Normal or alarm modes
	} else {
		(alarm) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);	
		//Timed mode
		if (waterTimed) {
			printStatus(nextWater);
			printNextWaterHour();
		//Continuous
		} else
			(alarm) ? printStatus(alarmCont) : printStatus(normalCont);
		//Path to image
		(alarm) ? path = pmChar(alarmPath) : path = pmChar(plantPath);
	}
	
	//Read from SD line by line and display icon.
	//Requires new function at UTFT library
	//Slow but effective
	int xSpacer = 15;
	int ySpacer = 25 + _bigFontSize;
	if (SD.exists(path) && _settings->getSDactive()) {
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
		&& _settings->getWaterTimed() && !_settings->getPumpProtected()) {
		printNextWaterHour();
	}
}

Window::Screen WinMainScreen::processTouch(const int x, const int y) { 
	return MainMenu; 
}