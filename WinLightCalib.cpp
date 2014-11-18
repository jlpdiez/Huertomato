#include "WinLightCalib.h"

WinLightCalib::WinLightCalib(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinLightCalib::WinLightCalib(const WinLightCalib &other) : Window(other) { }
	
WinLightCalib& WinLightCalib::operator=(const WinLightCalib& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinLightCalib::~WinLightCalib() {}
	
Window::Screen WinLightCalib::getType() const {
	return Window::LightCalib;
}

void WinLightCalib::print() {
	_rawLightLvl = _sensors->getRawLight();
	_lightThreshold = _settings->getLightThreshold();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
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
	lightCalibrationButtons[_nFlowButtons] = _buttons.addButton(x,_yTwoLnsSecond,lightCalibrationButtonsText[0]);
}

void WinLightCalib::update() {
	_rawLightLvl = _sensors->getRawLight();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = _xConfig + 16*_bigFontSize;
	_lcd->printNumI(_rawLightLvl,x,_yTwoLnsFirst,4,' ');
	
	//Second Line
	x = _xConfig + 10*_bigFontSize;
	_lcd->printNumI(_lightThreshold,x,_yTwoLnsSecond,4,' ');
}

//Draws entire screen Light Calibration
void WinLightCalib::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinLightCalib);
	addFlowButtons(true,true,true,lightCalibrationButtons);
	print();
	_buttons.drawButtons();
} 

Window::Screen WinLightCalib::processTouch(const int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == lightCalibrationButtons[0]) { return SensorCalib; }
	//Save
	else if (buttonIndex == lightCalibrationButtons[1]) {
		_settings->setLightThreshold(_lightThreshold);
		printSavedButton();
	//Exit
	} else if (buttonIndex == lightCalibrationButtons[2]) { return MainScreen; }
		
	else if (buttonIndex == lightCalibrationButtons[3]) {
		_lightThreshold = _rawLightLvl;
		update();
	}
	return None;
}
