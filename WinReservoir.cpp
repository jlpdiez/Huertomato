#include "WinReservoir.h"

WinReservoir::WinReservoir(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: Window(lcd,touch,sensors,settings) { }
	
WinReservoir::WinReservoir(const WinReservoir &other) : Window(other) {
	for (uint8_t i = 0; i < _nReservoirButtons; i++) {
		_reservoirButtons[i] = other._reservoirButtons[i];
	}
}
	
WinReservoir& WinReservoir::operator=(const WinReservoir &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nReservoirButtons; i++) {
		_reservoirButtons[i] = other._reservoirButtons[i];
	}
	return *this;
}

WinReservoir::~WinReservoir() { }
	
Window::Screen WinReservoir::getType() const {
	return Window::Reservoir;
}

void WinReservoir::print() {
	_reservoirActive = _settings->getReservoirModule();
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Print first bulletpoint
	_lcd->setFont(various_symbols);
	_lcd->print(pmChar(bulletStr),_xMenu,_yFourLines);
	//First textline
	_lcd->setFont(hallfetica_normal);
	_reservoirButtons[_nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yFourLines,reservoirButtonsText[0]);
	//First toggle
	if (_reservoirActive)
		_lcd->print(pmChar(onStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(reservoirButtonsText[0]),_yFourLines);
	else
		_lcd->print(pmChar(offStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(reservoirButtonsText[0]),_yFourLines);
	
	//Rest of lines
	//Change color depending on active/inactive
	if (_reservoirActive)
		_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	else
		_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(various_symbols);
	//Bullets - Start in 1 because nReservoirButtons[0] already added
	for (uint8_t i = 1; i < _nReservoirButtons - _nFlowButtons; i++) {
		_lcd->print(pmChar(bulletStr),_xMenu,_yFourLines+_bigFontSize*_yFactor4lines*i);
		_reservoirButtons[i + _nFlowButtons] = _buttons.addButton(_xMenu+_bigFontSize*2,_yFourLines+_bigFontSize*_yFactor4lines*i,(char*)pgm_read_word(&reservoirButtonsText[i]));
	}
	//Disable buttons depending on active/inactive
	if (!_reservoirActive) {
		for (uint8_t i = 4; i < _nReservoirButtons; i++)
			_buttons.disableButton(_reservoirButtons[i],true);
	} else {
		for (uint8_t i = 4; i < _nReservoirButtons; i++)
			_buttons.enableButton(_reservoirButtons[i],true);
	}
}

void WinReservoir::update() {
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(hallfetica_normal);
	//First toggle
	if (_reservoirActive)
		_lcd->print(pmChar(onStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(reservoirButtonsText[0]),_yFourLines);
	else
		_lcd->print(pmChar(offStr),_xMenu+_bigFontSize*2+_bigFontSize*strlen_P(reservoirButtonsText[0]),_yFourLines);
	
	//Rest of lines
	//Change color depending on active/inactive
	if (_reservoirActive)
		_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	else
		_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(various_symbols);
	//Bullets - Start in 1 because nReservoirButtons[0] already added
	for (uint8_t i = 1; i < _nReservoirButtons - _nFlowButtons; i++) {
		_lcd->print(pmChar(bulletStr),_xMenu,_yFourLines+_bigFontSize*_yFactor4lines*i);
	}
	//Disable buttons depending on active/inactive
	if (!_reservoirActive) {
		for (uint8_t i = 4; i < _nReservoirButtons; i++)
			_buttons.disableButton(_reservoirButtons[i],true);
	} else {
		for (uint8_t i = 4; i < _nReservoirButtons; i++)
			_buttons.enableButton(_reservoirButtons[i],true);
	}
}

void WinReservoir::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinReservoir);
	addFlowButtons(true,false,true,_reservoirButtons);
	print();
	_buttons.drawButtons();
}

Window::Screen WinReservoir::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _reservoirButtons[0])
		return SystemSettings;
	//Exit
	else if (buttonIndex == _reservoirButtons[2])
		return MainScreen;
		
	//On/Off toggle
	else if (buttonIndex == _reservoirButtons[3]) {
		_reservoirActive = !_reservoirActive;
		_settings->setReservoirModule(_reservoirActive);
		update();
	} else if (buttonIndex == _reservoirButtons[4])
		return Alarms;
	else if (buttonIndex == _reservoirButtons[5])
		return Pump;
	else if (buttonIndex == _reservoirButtons[6])
		return SensorCalib;
	return None;
}