#include "WinWaterNight.h"

WinWaterNight::WinWaterNight(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) { }

WinWaterNight::WinWaterNight(const WinWaterNight &other) : Window(other) {
	for (int i = 0; i < _nWaterNightButtons; i++) {
		_waterNightButtons[i] = other._waterNightButtons[i];
	}	
}

WinWaterNight& WinWaterNight::operator=(const WinWaterNight& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (int i = 0; i < _nWaterNightButtons; i++) {
		_waterNightButtons[i] = other._waterNightButtons[i];
	}
	return *this;
}

WinWaterNight::~WinWaterNight() {}

Window::Screen WinWaterNight::getType() const {
	return Window::NightWater;
}

void WinWaterNight::print() {
	_rawLightLvl = _sensors->getLight();
	_lightThreshold = _settings->getLightThreshold();
	_nightWater = _settings->getNightWatering();
	
	//First line
	//Triangle symbol
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print(bulletStr,_xConfig,_yThreeLnsFirst);
	//First line button
	_lcd->setFont(hallfetica_normal);
	_waterNightButtons[_nFlowButtons] = _buttons.addButton(_xConfig+2*_bigFontSize,_yThreeLnsFirst,pmChar(waterNightButtonsText[0]));
	//Watering at night ON/OFF
	if (_nightWater)
		_lcd->print(onStr,_xConfig+_bigFontSize*2+_bigFontSize*strlen_P(waterNightButtonsText[0]),_yThreeLnsFirst);
	else
		_lcd->print(offStr,_xConfig+_bigFontSize*2+_bigFontSize*strlen_P(waterNightButtonsText[0]),_yThreeLnsFirst);
	
	
	//Second Line
	_lcd->setColor(grey[0],grey[1],grey[2]);
	int x = _xConfig;
	_lcd->print(rawLight,x,_yThreeLnsSecond);
	x += _bigFontSize * (strlen_P(rawLight) + 1);
	_lcd->printNumI(_rawLightLvl,x,_yThreeLnsSecond,4);
	x +=5*_bigFontSize;
	_lcd->print(newLightUnit,x,_yThreeLnsSecond);
	
	//Third Line
	x = _xConfig;
	_lcd->print(lightThreshold,x,_yThreeLnsThird);
	x += _bigFontSize * (strlen_P(lightThreshold) + 1);
	_lcd->printNumI(_lightThreshold,x,_yThreeLnsThird,4);
	x += 5*_bigFontSize;
	_waterNightButtons[_nFlowButtons+1] = _buttons.addButton(x,_yThreeLnsThird,pmChar(waterNightButtonsText[1]));
}

void WinWaterNight::update() {
	_rawLightLvl = _sensors->getLight();
	
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Watering at night ON/OFF
	if (_nightWater)
		_lcd->print(onStr,_xConfig+_bigFontSize*2+_bigFontSize*strlen_P(waterNightButtonsText[0]),_yThreeLnsFirst);
	else
		_lcd->print(offStr,_xConfig+_bigFontSize*2+_bigFontSize*strlen_P(waterNightButtonsText[0]),_yThreeLnsFirst);
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Second Line
	int x = _xConfig + _bigFontSize * (strlen_P(rawLight) + 1);
	_lcd->printNumI(_rawLightLvl,x,_yThreeLnsSecond,4);
	//Third Line
	x = _xConfig + _bigFontSize * (strlen_P(lightThreshold) + 1);
	_lcd->printNumI(_lightThreshold,x,_yThreeLnsThird,4);
}

//Draws entire screen Light Calibration
void WinWaterNight::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinWaterNight);
	addFlowButtons(true,true,true,_waterNightButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinWaterNight::processTouch(const int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _waterNightButtons[0]) { return SystemSettings;}
	//Save
	else if (buttonIndex == _waterNightButtons[1]) {
			_settings->setNightWatering(_nightWater);
			_settings->setLightThreshold(_lightThreshold);
			printSavedButton();
	//Exit
	} else if (buttonIndex == _waterNightButtons[2])
		return MainScreen;
		
	//On/Off toggle	
	else if (buttonIndex == _waterNightButtons[3]) {
		_nightWater = !_nightWater;
		update();
	//Threshold set button
	} else if (buttonIndex == _waterNightButtons[4]) {
		_lightThreshold = _rawLightLvl;
		update();
	}
	return None;
}