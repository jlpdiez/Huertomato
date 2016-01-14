#include "WinMainScreen.h"

WinMainScreen::WinMainScreen(LiquidCrystal *lcd, Sensors *sensors, Settings *settings) 
: Window(lcd,sensors,settings) {
	_alarm = false;
	_temp = 0.0;
	_waterLvl = 0;
	_ph = 0.0;
	_ec = 0.0;
}

WinMainScreen::WinMainScreen(const WinMainScreen &other) : Window(other) {}
WinMainScreen& WinMainScreen::operator=(const WinMainScreen &other) {
	_lcd = other._lcd;
	_sensors = other._sensors;
	_settings = other._settings;
	return *this;
}

WinMainScreen::~WinMainScreen() {}
	
Window::Screen WinMainScreen::getType() const {
	return Window::MainScreen;
}

void WinMainScreen::draw() {
	_lcd->clear();
	//No alarms
	if (!_settings->getAlarmTriggered()) {
		//First line
		_lcd->setCursor(1,0);
		_lcd->print(_sensors->getTemp(),1);
		_lcd->print(pmChar(celsStr));
		_lcd->print(" LVL:");
		_lcd->print(_sensors->getWaterLevel());
		_lcd->print(pmChar(percentSign));
		//Second line
		_lcd->setCursor(0,1);
		//Prints one or two decimals depending on value
		float ph = _sensors->getPH();
		(ph < 10) ? _lcd->print(ph,2) : _lcd->print(ph,1);
		_lcd->print("pH EC:");
		//Prints one or two decimals depending on value
		float ec = _sensors->getEC();
		(ec < 10) ? _lcd->print(ec,2) : _lcd->print(ec,1);
		_lcd->print(pmChar(miliSiemens));
		
	//EC alarm triggered
	} else if (_sensors->ecOffRange()) {
		//EC upper alarm triggered
		if (_sensors->getEC() > _settings->getECalarmUp()) {
			_lcd->setCursor(4,0);
			_lcd->print(pmChar(ecAlarmUp));
			_lcd->setCursor(2,1);
			_lcd->print(pmChar(ecAlarmUp1));
		//EC lower alarm triggered	
		} else if (_sensors->getEC() < _settings->getECalarmDown()) {
			_lcd->setCursor(4,0);
			_lcd->print(pmChar(ecAlarmDown));
			_lcd->setCursor(1,1);
			_lcd->print(pmChar(ecAlarmDown1));
		}
		
	//pH alarm triggered
	} else if (_sensors->phOffRange()) {
		//Upper alarm triggered
		if (_sensors->getPH() > _settings->getPHalarmUp()) {
			_lcd->setCursor(4,0);
			_lcd->print(pmChar(phAlarmUp));
			_lcd->setCursor(1,1);
			_lcd->print(pmChar(phAlarmUp1));
		//Lower alarm triggered
		} else if (_sensors->getPH() < _settings->getPHalarmDown()) {
			_lcd->setCursor(4,0);
			_lcd->print(pmChar(phAlarmDown));
			_lcd->setCursor(1,1);
			_lcd->print(pmChar(phAlarmDown1));
		}
	
	//Water level alarm triggered
	} else if (_sensors->lvlOffRange()) {
		//Pump is critically low too - No watering
		if (_settings->getPumpProtected()) {
			_lcd->setCursor(4,0);
			_lcd->print(pmChar(lvlAlarm));
			_lcd->setCursor(1,1);
			_lcd->print(pmChar(lvlAlarm1));
		} else {
			_lcd->setCursor(4,0);
			_lcd->print(pmChar(pumpAlarm));
			_lcd->setCursor(1,1);
			_lcd->print(pmChar(pumpAlarm1));
		}
	}
}

//TODO: RE-enable
//TODO: Pensar una forma de hacer clear() when screen changes
//draw() when number of digits change or alarm gets active/deactive
//Refreshes minimum data
void WinMainScreen::update() {
	if ((settings.getAlarmTriggered() != _alarm)) { //Or temp or whatever changed digits
		_alarm = _settings->getAlarmTriggered();
		draw();	
	}
	
	//No alarm
	if (!_settings->getAlarmTriggered()){
		_lcd->setCursor(1,0);
		//First line
		_lcd->print(_sensors->getTemp(),1);
		_lcd->setCursor(11,0);
		_lcd->print(_sensors->getWaterLevel());
		_lcd->print(pmChar(percentSign));
		_lcd->print(pmChar(spaceChar));
		_lcd->print(pmChar(spaceChar));
		//Second line
		_lcd->setCursor(0,1);
		_lcd->print(_sensors->getPH(),1);
		_lcd->setCursor(10,1);
		float ec = _sensors->getEC();
		(ec < 10) ? _lcd->print(_sensors->getEC(),2) : _lcd->print(_sensors->getEC(),1);
	
	}
}

Window::Screen WinMainScreen::processTouch(int but) { 
	//Select button
	if (but == 5)
		return WateringCycle;
	else
		return None;
}