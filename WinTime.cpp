/*
 * MainMenu.cpp
 *
 * Created: 07/11/2014 1:20:03
 *  Author: HAL
 */ 
#include "WinTime.h"

WinTime::WinTime(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
: Window(lcd,touch,sensors,settings) { }

WinTime::WinTime(const WinTime &other) : Window(other) { }
	
WinTime& WinTime::operator=(const WinTime& other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	_buttons = other._buttons;
	return *this;
}

WinTime::~WinTime() {}
	
Window::Screen WinTime::getType() const {
	return Window::TimeDate;
}

void WinTime::print() {
	const int yTime = 60;
	const int yDate = 135;
	const int xSpacer = 25;
	
	const int houU[] = {150, yTime-22};       //hour up
	const int minU[] = {220, yTime-22};       //min up
	const int secU[] = {290, yTime-22};       //sec up
	const int houD[] = {150, yTime+22};       //hour down
	const int minD[] = {220, yTime+22};       //min down
	const int secD[] = {290, yTime+22};       //sec down
	const int dayU[] = {150, yDate-22};       //day up
	const int monU[] = {220, yDate-22};       //month up
	const int yeaU[] = {290, yDate-22};       //year up
	const int dayD[] = {150, yDate+22};       //day down
	const int monD[] = {220, yDate+22};       //month down
	const int yeaD[] = {290, yDate+22};       //year down

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
	timeButtons[3] = _buttons.addButton(houU[0],houU[1],timeButtonText[0],BUTTON_SYMBOL);
	timeButtons[4] = _buttons.addButton(minU[0],minU[1],timeButtonText[1],BUTTON_SYMBOL);
	timeButtons[5] = _buttons.addButton(secU[0],secU[1],timeButtonText[2],BUTTON_SYMBOL);
	timeButtons[6] = _buttons.addButton(houD[0],houD[1],timeButtonText[3],BUTTON_SYMBOL);
	timeButtons[7] = _buttons.addButton(minD[0],minD[1],timeButtonText[4],BUTTON_SYMBOL);
	timeButtons[8] = _buttons.addButton(secD[0],secD[1],timeButtonText[5],BUTTON_SYMBOL);
	
	timeButtons[9] = _buttons.addButton(dayU[0],dayU[1],timeButtonText[6],BUTTON_SYMBOL);
	timeButtons[10] = _buttons.addButton(monU[0],monU[1],timeButtonText[7],BUTTON_SYMBOL);
	timeButtons[11] = _buttons.addButton(yeaU[0],yeaU[1],timeButtonText[8],BUTTON_SYMBOL);
	timeButtons[12] = _buttons.addButton(dayD[0],dayD[1],timeButtonText[9],BUTTON_SYMBOL);
	timeButtons[13] = _buttons.addButton(monD[0],monD[1],timeButtonText[10],BUTTON_SYMBOL);
	timeButtons[14] = _buttons.addButton(yeaD[0],yeaD[1],timeButtonText[11],BUTTON_SYMBOL);
	
	//TIME
	char* timeS = "Time";
	_lcd->setFont(hallfetica_normal);
	_lcd->print(timeS, xSpacer, yTime);
	_lcd->setFont(Sinclair_S);
	char* hhmmssS = "(HH:MM:SS)";
	//(strlen(timeS)*bigFontSize)/2 is middle point of "Time". (strlen(hhmmss)*smallFontSize)/2 is middle point of "(HH:MM:SS)"
	//So the x coord is xTime + middle "Time" - middle of "(HH:MM:SS)"
	_lcd->print(hhmmssS, xSpacer+(strlen(timeS)*_bigFontSize)/2-(strlen(hhmmssS)*_smallFontSize)/2, yTime+_bigFontSize+2);
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_sysHour,houU[0]+_smallFontSize/2-_bigFontSize+2,yTime,2,'0');
	_lcd->print(":",houU[0]+39,yTime);
	_lcd->printNumI(_sysMin,minU[0]+_smallFontSize/2-_bigFontSize+2,yTime,2,'0');
	_lcd->print(":",minU[0]+39,yTime);
	_lcd->printNumI(_sysSec,secU[0]+_smallFontSize/2-_bigFontSize+2,yTime,2,'0');

	//DATE
	char* dateS = "Date";
	_lcd->print(dateS, xSpacer, yDate);
	_lcd->setFont(Sinclair_S);
	char* ddmmyyyyS = "(DD/MM/YYYY)";
	_lcd->print(ddmmyyyyS, xSpacer+(strlen(dateS)*_bigFontSize)/2-(strlen(ddmmyyyyS)*_smallFontSize)/2, yDate+_bigFontSize+2);
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_sysDay, dayU[0]+_smallFontSize/2-_bigFontSize+2, yDate,2,'0');
	_lcd->print("/", dayU[0]+39, yDate);
	_lcd->printNumI(_sysMonth, monU[0]+_smallFontSize/2-_bigFontSize+2, yDate,2,'0');
	_lcd->print("/", monU[0]+39, yDate);
	_lcd->printNumI(_sysYear, yeaU[0]+_smallFontSize/2-_bigFontSize+2, yDate,4);
	
}

//Draws entire screen Time Settings
void WinTime::draw() {
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Time & Date -");
	addFlowButtons(true,true,true,timeButtons);
	print();
	_buttons.drawButtons();
}

//Redraws only time & date numbers from inner temp vars
//Used when +- signs are pressed
void WinTime::update() {
	const int yTime = 60;
	const int yDate = 135;
	
	const int houU[] = {150, yTime-22};       //hour up
	const int minU[] = {220, yTime-22};       //min up
	const int secU[] = {290, yTime-22};       //sec up
	const int dayU[] = {150, yDate-22};       //day up
	const int monU[] = {220, yDate-22};       //month up
	const int yeaU[] = {290, yDate-22};       //year up
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(_sysHour,houU[0]+_smallFontSize/2-_bigFontSize+2,yTime,2,'0');
	_lcd->printNumI(_sysMin,minU[0]+_smallFontSize/2-_bigFontSize+2,yTime,2,'0');
	_lcd->printNumI(_sysSec,secU[0]+_smallFontSize/2-_bigFontSize+2,yTime,2,'0');
	
	_lcd->printNumI(_sysDay, dayU[0]+_smallFontSize/2-_bigFontSize+2, yDate,2,'0');
	_lcd->printNumI(_sysMonth, monU[0]+_smallFontSize/2-_bigFontSize+2, yDate,2,'0');
	_lcd->printNumI(_sysYear, yeaU[0]+_smallFontSize/2-_bigFontSize+2, yDate,4);
}

Window::Screen WinTime::processTouch(const int x, const int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == timeButtons[0]) { return ControllerSettings; }
	//Save
	else if (buttonIndex == timeButtons[1]) {
		_sensors->setRTCtime(_sysHour, _sysMin, _sysSec, _sysDay, _sysMonth, _sysYear);
		printSavedButton();
	//Exit
	} else if (buttonIndex == timeButtons[2]) { return MainScreen; }
		
	//Hour up
	else if (buttonIndex == timeButtons[3]) {
		(_sysHour >= 23) ? _sysHour=0 : _sysHour++;
		update();
	//Min up
	} else if (buttonIndex == timeButtons[4]) {
		(_sysMin >= 59) ? _sysMin=0 : _sysMin++;
		update();
	//Sec up
	} else if (buttonIndex == timeButtons[5]) {
		(_sysSec >= 59) ? _sysSec=0 : _sysSec++;;
		update();
	//Hour down
	} else if (buttonIndex == timeButtons[6]) {
		(_sysHour <= 0) ? _sysHour=23 : _sysHour--;
		update();
	//Min down
	} else if (buttonIndex == timeButtons[7]) {
		(_sysMin <= 0) ? _sysMin=59 : _sysMin--;
		update();
	//Sec down
	} else if (buttonIndex == timeButtons[8]) {
		(_sysSec <= 0) ? _sysSec=59 : _sysSec--;
		update();
	//Day up
	} else if (buttonIndex == timeButtons[9]) {
		(_sysDay >= 31) ? _sysDay=1 : _sysDay++;
		update();
	//Month up
	} else if (buttonIndex == timeButtons[10]) {
		(_sysMonth >= 12) ? _sysMonth=1 : _sysMonth++;
		update();
	//Year up
	} else if (buttonIndex == timeButtons[11]) {
		//TimeAlarms can only handle 1971-2037
		(_sysYear >= 2037) ? _sysYear=1971 : _sysYear++;
		update();
	//Day down
	} else if (buttonIndex == timeButtons[12]) {
		(_sysDay <= 1) ? _sysDay=31 : _sysDay--;
		update();
	//Month down
	} else if (buttonIndex == timeButtons[13]) {
		(_sysMonth <= 1) ? _sysMonth=12 : _sysMonth--;
		update();
	//Year down
	} else if (buttonIndex == timeButtons[14]) {
		(_sysYear <= 1971) ? _sysYear=2037 : _sysYear--;
		update();
	}
	return None;
}
