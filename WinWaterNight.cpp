#include "WinWaterNight.h"

WinWaterNight::WinWaterNight(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) { }

WinWaterNight::WinWaterNight(const WinWaterNight &other) : Window(other) { }

WinWaterNight& WinWaterNight::operator=(const WinWaterNight& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinWaterNight::~WinWaterNight() {}

Window::Screen WinWaterNight::getType() const {
	return Window::NightWater;
}

void WinWaterNight::print() {
	_rawLightLvl = _sensors->getRawLight();
	_lightThreshold = _settings->getLightThreshold();
	_nightWater = _settings->getNightWatering();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//TODO:	
	//Watering at night ON/OFF
	/*if (_nightWater)
		_lcd->print(onStr,_xSpacer+_bigFontSize*2+_bigFontSize*strlen(systemButtonText[4]),_ySpacer+_bigFontSize*2*4);
	else
		_lcd->print(offStr,_xSpacer+_bigFontSize*2+_bigFontSize*strlen(systemButtonText[4]),_ySpacer+_bigFontSize*2*4);
		*/
		
	//First Line
	int x = _xConfig;
	_lcd->print(rawLight,x,_yTwoLnsFirst);
	x += 16*_bigFontSize;
	_lcd->printNumI(_rawLightLvl,x,_yTwoLnsFirst,4,' ');
	//x +=4*bigFontSize;
	//_lcd->print("lux",x,yFirstLine);
	
	//Second Line
	x = _xConfig;
	_lcd->print(lightThreshold,x,_yTwoLnsSecond);
	x += 10*_bigFontSize;
	_lcd->printNumI(_lightThreshold,x,_yTwoLnsSecond,4,' ');
	//x += 4*bigFontSize;
	//_lcd->print("lux",x,ySecondLine);
	x += 5*_bigFontSize;
	waterNightButtons[_nFlowButtons] = _buttons.addButton(x,_yTwoLnsSecond,waterNightButtonsText[0]);
}

void WinWaterNight::update() {
	_rawLightLvl = _sensors->getRawLight();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	/*
	//Watering at night ON/OFF
	if (_nightWater)
	_lcd->print(onStr,_xSpacer+_bigFontSize*2+_bigFontSize*strlen(systemButtonText[4]),_ySpacer+_bigFontSize*2*4);
	else
	_lcd->print(offStr,_xSpacer+_bigFontSize*2+_bigFontSize*strlen(systemButtonText[4]),_ySpacer+_bigFontSize*2*4);

*/
	//First Line
	int x = _xConfig + 16*_bigFontSize;
	_lcd->printNumI(_rawLightLvl,x,_yTwoLnsFirst,4,' ');
	
	//Second Line
	x = _xConfig + 10*_bigFontSize;
	_lcd->printNumI(_lightThreshold,x,_yTwoLnsSecond,4,' ');
}

//Draws entire screen Light Calibration
void WinWaterNight::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinWaterNight);
	addFlowButtons(true,true,true,waterNightButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinWaterNight::processTouch(const int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == waterNightButtons[0]) { return SensorCalib; }
	//Save
	else if (buttonIndex == waterNightButtons[1]) {
		_settings->setLightThreshold(_lightThreshold);
		printSavedButton();
		//Exit
		} else if (buttonIndex == waterNightButtons[2]) { return MainScreen; }
		
		else if (buttonIndex == waterNightButtons[3]) {
			_lightThreshold = _rawLightLvl;
			update();
		}
		return None;
	}
