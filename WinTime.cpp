#include "WinTime.h"

WinTime::WinTime(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinTime::WinTime(const WinTime &other) : Window(other) {
	for (uint8_t i = 0; i < _nTimeButtons; i++) {
		_timeButtons[i] = other._timeButtons[i];
	}
}
	
WinTime& WinTime::operator=(const WinTime& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	for (uint8_t i = 0; i < _nTimeButtons; i++) {
		_timeButtons[i] = other._timeButtons[i];
	}
	return *this;
}

WinTime::~WinTime() {}
	
Window::Screen WinTime::getType() const {
	return Window::TimeDate;
}

void WinTime::print() {
	const int houU[] = {150, _yTwoLnsFirst-22};       //hour up
	const int minU[] = {220, _yTwoLnsFirst-22};       //min up
	const int secU[] = {290, _yTwoLnsFirst-22};       //sec up
	const int houD[] = {150, _yTwoLnsFirst+22};       //hour down
	const int minD[] = {220, _yTwoLnsFirst+22};       //min down
	const int secD[] = {290, _yTwoLnsFirst+22};       //sec down
	const int dayU[] = {150, _yTwoLnsSecond-22};       //day up
	const int monU[] = {220, _yTwoLnsSecond-22};       //month up
	const int yeaU[] = {290, _yTwoLnsSecond-22};       //year up
	const int dayD[] = {150, _yTwoLnsSecond+22};       //day down
	const int monD[] = {220, _yTwoLnsSecond+22};       //month down
	const int yeaD[] = {290, _yTwoLnsSecond+22};       //year down

	//Get actual time
	time_t t = now();
	_sysHour = hour(t);
	_sysMin = minute(t);
	_sysSec = second(t);
	_sysDay = day(t);
	_sysMonth = month(t);
	_sysYear = year(t);

	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	
	//Make +/- buttons
	_timeButtons[_nFlowButtons] = _buttons.addButton(houU[0],houU[1],plusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+1] = _buttons.addButton(minU[0],minU[1],plusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+2] = _buttons.addButton(secU[0],secU[1],plusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+3] = _buttons.addButton(houD[0],houD[1],minusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+4] = _buttons.addButton(minD[0],minD[1],minusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+5] = _buttons.addButton(secD[0],secD[1],minusStr,BUTTON_SYMBOL);
	
	_timeButtons[_nFlowButtons+6] = _buttons.addButton(dayU[0],dayU[1],plusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+7] = _buttons.addButton(monU[0],monU[1],plusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+8] = _buttons.addButton(yeaU[0],yeaU[1],plusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+9] = _buttons.addButton(dayD[0],dayD[1],minusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+10] = _buttons.addButton(monD[0],monD[1],minusStr,BUTTON_SYMBOL);
	_timeButtons[_nFlowButtons+11] = _buttons.addButton(yeaD[0],yeaD[1],minusStr,BUTTON_SYMBOL);
	
	//TIME
	_lcd->setFont(hallfetica_normal);
	_lcd->print(pmChar(timeS), _xConfig, _yTwoLnsFirst);
	_lcd->setFont(Sinclair_S);
	//(strlen_P(timeS)*bigFontSize)/2 is middle point of "Time". (strlen_P(hhmmss)*smallFontSize)/2 is middle point of "(HH:MM:SS)"
	//So the x coord is xTime + middle "Time" - middle of "(HH:MM:SS)"
	_lcd->print(pmChar(timeFormatS), _xConfig+(strlen_P(timeS)*_bigFontSize)/2-(strlen_P(timeFormatS)*_smallFontSize)/2, _yTwoLnsFirst+_bigFontSize+2);
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_sysHour,houU[0]+_smallFontSize/2-_bigFontSize+2,_yTwoLnsFirst,2,'0');
	_lcd->print(pmChar(timeSeparator),houU[0]+39,_yTwoLnsFirst);
	_lcd->printNumI(_sysMin,minU[0]+_smallFontSize/2-_bigFontSize+2,_yTwoLnsFirst,2,'0');
	_lcd->print(pmChar(timeSeparator),minU[0]+39,_yTwoLnsFirst);
	_lcd->printNumI(_sysSec,secU[0]+_smallFontSize/2-_bigFontSize+2,_yTwoLnsFirst,2,'0');

	//DATE
	_lcd->print(pmChar(dateS), _xConfig, _yTwoLnsSecond);
	_lcd->setFont(Sinclair_S);
	_lcd->print(pmChar(dateFormatS), _xConfig+(strlen_P(dateS)*_bigFontSize)/2-(strlen_P(dateFormatS)*_smallFontSize)/2, _yTwoLnsSecond+_bigFontSize+2);
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_sysDay, dayU[0]+_smallFontSize/2-_bigFontSize+2, _yTwoLnsSecond,2,'0');
	_lcd->print(pmChar(dateSeparator), dayU[0]+39, _yTwoLnsSecond);
	_lcd->printNumI(_sysMonth, monU[0]+_smallFontSize/2-_bigFontSize+2, _yTwoLnsSecond,2,'0');
	_lcd->print(pmChar(dateSeparator), monU[0]+39, _yTwoLnsSecond);
	_lcd->printNumI(_sysYear, yeaU[0]+_smallFontSize/2-_bigFontSize+2, _yTwoLnsSecond,4);
	
}

//Draws entire screen Time Settings
void WinTime::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader(nameWinTime);
	addFlowButtons(true,true,true,_timeButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only time & date numbers from inner temp vars
//Used when +- signs are pressed
void WinTime::update() {
	const int houU[] = {150, _yTwoLnsFirst-22};       //hour up
	const int minU[] = {220, _yTwoLnsFirst-22};       //min up
	const int secU[] = {290, _yTwoLnsFirst-22};       //sec up
	const int dayU[] = {150, _yTwoLnsSecond-22};       //day up
	const int monU[] = {220, _yTwoLnsSecond-22};       //month up
	const int yeaU[] = {290, _yTwoLnsSecond-22};       //year up
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_sysHour,houU[0]+_smallFontSize/2-_bigFontSize+2,_yTwoLnsFirst,2,'0');
	_lcd->printNumI(_sysMin,minU[0]+_smallFontSize/2-_bigFontSize+2,_yTwoLnsFirst,2,'0');
	_lcd->printNumI(_sysSec,secU[0]+_smallFontSize/2-_bigFontSize+2,_yTwoLnsFirst,2,'0');
	
	_lcd->printNumI(_sysDay, dayU[0]+_smallFontSize/2-_bigFontSize+2, _yTwoLnsSecond,2,'0');
	_lcd->printNumI(_sysMonth, monU[0]+_smallFontSize/2-_bigFontSize+2, _yTwoLnsSecond,2,'0');
	_lcd->printNumI(_sysYear, yeaU[0]+_smallFontSize/2-_bigFontSize+2, _yTwoLnsSecond,4);
}

Window::Screen WinTime::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == _timeButtons[0]) 
		return ControllerSettings;
	//Save
	else if (buttonIndex == _timeButtons[1]) {
		_settings->setRTCtime(_sysHour, _sysMin, _sysSec, _sysDay, _sysMonth, _sysYear);
		printSavedButton();
	//Exit
	} else if (buttonIndex == _timeButtons[2]) 
		return MainScreen;
		
	//Hour up
	else if (buttonIndex == _timeButtons[3]) {
		(_sysHour >= 23) ? _sysHour=0 : _sysHour++;
		update();
	//Min up
	} else if (buttonIndex == _timeButtons[4]) {
		(_sysMin >= 59) ? _sysMin=0 : _sysMin++;
		update();
	//Sec up
	} else if (buttonIndex == _timeButtons[5]) {
		(_sysSec >= 59) ? _sysSec=0 : _sysSec++;;
		update();
	//Hour down
	} else if (buttonIndex == _timeButtons[6]) {
		(_sysHour <= 0) ? _sysHour=23 : _sysHour--;
		update();
	//Min down
	} else if (buttonIndex == _timeButtons[7]) {
		(_sysMin <= 0) ? _sysMin=59 : _sysMin--;
		update();
	//Sec down
	} else if (buttonIndex == _timeButtons[8]) {
		(_sysSec <= 0) ? _sysSec=59 : _sysSec--;
		update();
	//Day up
	} else if (buttonIndex == _timeButtons[9]) {
		(_sysDay >= 31) ? _sysDay=1 : _sysDay++;
		update();
	//Month up
	} else if (buttonIndex == _timeButtons[10]) {
		(_sysMonth >= 12) ? _sysMonth=1 : _sysMonth++;
		update();
	//Year up
	} else if (buttonIndex == _timeButtons[11]) {
		//TimeAlarms can only handle 1971-2037
		(_sysYear >= 2037) ? _sysYear=1971 : _sysYear++;
		update();
	//Day down
	} else if (buttonIndex == _timeButtons[12]) {
		(_sysDay <= 1) ? _sysDay=31 : _sysDay--;
		update();
	//Month down
	} else if (buttonIndex == _timeButtons[13]) {
		(_sysMonth <= 1) ? _sysMonth=12 : _sysMonth--;
		update();
	//Year down
	} else if (buttonIndex == _timeButtons[14]) {
		(_sysYear <= 1971) ? _sysYear=2037 : _sysYear--;
		update();
	}
	return None;
}
