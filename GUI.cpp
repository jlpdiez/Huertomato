#include "GUI.h"
/*#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include "Buttons.h"*/

GUI::GUI(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings) 
	: _lcd(lcd), _touch(touch), _buttons(lcd,touch), _sensors(sensors), _settings(settings) {
  _actScreen = 0;

  _buttons.setTextFont(hallfetica_normal);
  _buttons.setSymbolFont(various_symbols);
  _buttons.setButtonColors(lightGreen, darkGreen, white, grey, white);
}

GUI::GUI(const GUI &other) : _buttons(other._buttons) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	
	_actScreen = 0;

	_buttons.setTextFont(hallfetica_normal);
	_buttons.setSymbolFont(various_symbols);
	_buttons.setButtonColors(lightGreen, darkGreen, white, grey, white);
}

GUI& GUI::operator=(const GUI &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_buttons = other._buttons;
	_sensors = other._sensors;
	_settings = other._settings;
	
	return *this;
}

GUI::~GUI() {}
	
//GUI::GUI(int lcdRS,int lcdWR,int lcdCS,int lcdRST,int lcdTCLK,int lcdTCS,int lcdTDIN,int lcdTDOUT,int lcdIRQ) :
//  _lcd(ITDB32WD, lcdRS,lcdWR,lcdCS,lcdRST), _touch(lcdTCLK,lcdTCS,lcdTDIN,lcdTDOUT,lcdIRQ), _buttons(&_lcd, &_touch){
//    
//  _actScreen = 0;
//  _lcd->InitLCD();
//  _lcd->clrScr();
//  _lcd->fillScr(VGA_WHITE);
//  _touch.InitTouch();
//  _touch.setPrecision(PREC_MEDIUM);
//  _buttons.setTextFont(hallfetica_normal);
//  _buttons.setButtonColors(lightGreen, darkGreen, white, grey, white);
//}

void GUI::processTouch() {
  if (_touch->dataAvailable()) {
    _touch->read();
    int x = _touch->getX();
    int y = _touch->getY();
    
    switch (_actScreen) {
      //Main Screen
      case 0: 
        _actScreen = 1;
        _lcd->fillScr(VGA_WHITE);
        drawMainMenu();
        break;
      //MainMenu
      case 1:
        processTouchMainMenu(x,y);
        break;
      //System Settings
      case 2:
        processTouchSystem(x,y);
        break;
      //Controller Settings
      case 3:
        processTouchController(x,y);
        break;
      //Time & Date
      case 4:
        processTouchTime(x,y);
        break;
      //Sensor Polling
      case 5:
        processTouchSensorPolling(x,y);
        break;
      //SD Card
      case 6:
        processTouchSDcard(x,y);
        break;
      //Watering Cycle
      case 7:
        processTouchWaterCycle(x,y);
        break;
      //Sensor Alarms
      case 8: 
        processTouchSensorAlarms(x,y);
        break;
      //pH Alarms
      case 9: 
        processTouchPHalarms(x,y);
        break;
      //EC Alarms
      case 10:
        processTouchECalarms(x,y);
        break;
      //Nutrient Level Alarms
      case 11:
        processTouchWaterAlarms(x,y);
        break;
      //Auto Config Alarms
      case 12:
        processTouchAutoConfig(x,y);
        break;
      //Sensor Calibration
      case 13:
        processTouchSensorCalibration(x,y);
        break;
      //Water Level Calibration
      case 14:
        processTouchWaterCalibration(x,y);
        break;
      //pH Calibration
      case 15:
        processTouchPHcalibration(x,y);
        break;
      //EC Calibration
      case 16:
        processTouchECcalibration(x,y);
        break;
      //Light Calibration
      case 17:
        processTouchLightCalibration(x,y);
        break;
    }
  
  }
}

//private
void GUI::printHeader() {
  const int headerHeight = 20; 
  //Header background
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->fillRect(0,0,xSize,headerHeight);
  //Separtator line
  _lcd->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
  _lcd->drawLine(0, headerHeight, xSize, headerHeight);  
}

void GUI::printMainHeader() {
  printHeader(); 
  //Left
  //_lcd->print("20:55:33",2,2);
  //Center
  //_lcd->print("20:55:33",xSize/2-_lcd->getFontXsize()*4,2);
  //RIGHT
  //max X - 16px per letter X 8 letters - 2px spacing
  //_lcd->print("20:57:23",xSize-(_lcd->getFontXsize()*8)-2, 2);
  
  //Spacing is 4px between imgs (so its orig x + 16 + 4)
  //if (alarm)
//  _lcd->drawBitmap(2, 2, 16, 16, alarm16);
//  //if (manual control)
//  _lcd->drawBitmap(22, 2, 16, 16, water16);
//  //if (watering plants)
//  _lcd->drawBitmap(42, 2, 16, 16, mano16); 
  //Small TextS
  _lcd->setFont(Sinclair_S);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->print("Huertomato 1.2",10,6); 
  //Change below too
  //max X - 8px per letter X 5 letters - 6px spacing
  _lcd->print("16:57:05",xSize-(8*_lcd->getFontXsize())-2, 6); 
}

void GUI::updateMainHeader() {
  _lcd->setFont(Sinclair_S);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  //max X - 8px per letter X 5 letters - 6px spacing
  //Change above too
  _lcd->print("16:57:05",xSize-(8*_lcd->getFontXsize())-2, 6); 
}

void GUI::printMenuHeader(char* c) {
  printHeader();
  _lcd->setFont(hallfetica_normal);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]); 
  _lcd->print(c,xSize/2-(_lcd->getFontXsize()*(strlen(c)/2)),2);
}

//Print sensor info on main screen. Data will tyurn red if theres an alarm triggered
void GUI::printSensorInfo() {
  _lcd->setFont(hallfetica_normal);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(VGA_WHITE);

  //For x coord we take maxSize and extract letterSize*letters plus 25 arbitrary spacing
  char* humid = "Humidity: 34%";
  _lcd->print(humid,xSize-25-(_lcd->getFontXsize()*strlen(humid)),35);
  //_lcd->printNumF(sensors.getTemp(),2,114,64,'.',4);
  //_lcd->print("23.42C",xSize-25-_lcd->getFontXsize()*6,35);
  //_lcd->print("C",154,64);
  
  
  char* temp = "Temp: 23.42C"; 
  _lcd->print(temp,xSize-25-(_lcd->getFontXsize()*strlen(temp)),35+(_lcd->getFontXsize()+8));
  //_lcd->printNumI(sensors.getHumidity(),90,82,2);
  //_lcd->print("34%",106,82);
  
  char* light = "Light: 51%";
  _lcd->print(light,xSize-25-(_lcd->getFontXsize()*strlen(light)),35+(_lcd->getFontXsize()+8)*2);
  //_lcd->printNumI(sensors.getLight(),66,100,2);
  //_lcd->print("51%",82,100);
  
  //((sensors.ph > phAlarmU) || (sensors.ph < phAlarmD)) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(grey[0], grey[1], grey[2]);
  //_lcd->setColor(red[0],red[1],red[2]);
  char* ph = "pH: 6.3";
  _lcd->print(ph,xSize-25-(_lcd->getFontXsize()*strlen(ph)),35+(_lcd->getFontXsize()+8)*3);
  //_lcd->printNumF(sensors.getPH(),2,42,118,'.',4);
 
  //((sensors.ec > ecAlarmU || (sensors.ec < ecAlarmD))) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(grey[0], grey[1], grey[2]);
  char* ec = "EC: 1315uS";
  _lcd->print(ec,xSize-25-(_lcd->getFontXsize()*strlen(ec)),35+(_lcd->getFontXsize()+8)*4);
  //_lcd->printNumI(sensors.getEC(),42,136,4);
  //_lcd->print("uS",74,136);
  

  //(sensors.waterLevel < waterAlarm) ? _lcd->setColor(red[0],red[1],red[2]) : _lcd->setColor(grey[0], grey[1], grey[2]);
  char* lvl = "Deposit: 100%";
  _lcd->print(lvl,xSize-25-(_lcd->getFontXsize()*strlen(lvl)),35+(_lcd->getFontXsize()+8)*5);
  //_lcd->printNumI(sensors.getWaterLevel(),130,154,3);
  //_lcd->print("%",154,154);
}

void GUI::updateSensorInfo() {
  //Same as above but should only change numbers
  //Unless theres an alarm triggered in which case turns entire text red
}

//Shows system status in main screen
void GUI::printIconAndStatus() {
  //Change Y coord.
  
  //If theres an alarm
  if (_settings->getAlarmTriggered()) {
    //_lcd->drawBitmap (15, 25+_lcd->getFontXsize(), 126, 126, alarm126);    
    _lcd->setColor(red[0],red[1],red[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("Alarm - Check Solution",10+_lcd->getFontXsize()*2,200);
  
  //watering and not in continuous mode
  } else if (_settings->getWaterTimed() && _settings->getWateringPlants()) {  
    //_lcd->drawBitmap (15, 25+_lcd->getFontXsize(), 126, 126, logo126);     
    _lcd->setColor(blue[0],blue[1],blue[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("Huertomato Watering",10+_lcd->getFontXsize()*2,200); 
  
  //water stopped because its night  
  } else if (_settings->getNightWateringStopped()) {
    //_lcd->drawBitmap (15, 25+_lcd->getFontXsize(), 126, 126, moon126);
    _lcd->setColor(grey[0],grey[1],grey[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("No Watering @ Night",10+_lcd->getFontXsize()*2,200);
    
  //water pump in manual mode    
  } else if (_settings->getManualPump()) {
	//_lcd->drawBitmap (15, 25+_lcd->getFontXsize(), 126, 126, hand126); 
	_lcd->setColor(red[0],red[1],red[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print("T",10,200);
	_lcd->setFont(hallfetica_normal);
	_lcd->print("Pump Manually Enabled",10+_lcd->getFontXsize()*2,200);  

  //Normal operation    
  } else {
    _lcd->drawBitmap (15, 25+_lcd->getFontXsize(), 126, 126, plant126);  
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("Next Watering @ 17:56",10+_lcd->getFontXsize()*2,200);
  }
}

void GUI::updateIconAndStatus() {
  //Same as above. Should change icon only if system state changed.
  //If not should only update next watering time
}

void GUI::drawMainScreen() {
  printMainHeader();
  printSensorInfo();
  printIconAndStatus();
}

void GUI::updateMainScreen() {
  updateMainHeader();
  //Should be changed to updateSensorInfo
  printSensorInfo(); 
  //Change to updateIconAndStatus();
  printIconAndStatus();
}

//These function should be the first to get its buttons into the array buttons
//It gets input button array and adds appropriate back/save/cancel to positions 0, 1 & 2 
void GUI::printFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]) {
//  _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
//  _lcd->drawLine(0, 195, xSize, 195);

  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(hallfetica_normal);
  
  if (backButton) {
    const int backX = 15;
    const int backY = 215;
    char* backText = " Back ";
    _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);   
    //_lcd->print(backS,backX,backY);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(backX-1,backY-5,backX+_lcd->getFontXsize()*strlen(backText),backY-5);
    buttonArray[0] = _buttons.addButton(backX, backY, backText);
    
    //Coords should be x1, y1, x2, y2
    //x1,y1 are line's x1,y1. x2 is line's x2. y2 is backY+_lcd->getFontXsize()+something (or maxY)
    //const int backB[]= {backX-1, backY-5, backX+_lcd->getFontXsize()*strlen(backS), maxY};
    
  } else {
    buttonArray[0] = -1;
  }
  
  if (saveButton) {
    char* saveText = " Save ";
    const int saveX = xSize/2 - _lcd->getFontXsize()*strlen(saveText)/2;
    const int saveY = 215;
    _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
    //_lcd->print(saveS,saveX,saveY);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(saveX-1,saveY-5,saveX+_lcd->getFontXsize()*strlen(saveText),saveY-5);
    buttonArray[1] = _buttons.addButton(saveX, saveY, saveText);
    
  } else {
    buttonArray[1] = -1; 
  }
  
  if (exitButton) { 
    char* cancelText = " Exit ";
    const int cancelX = xSize - 15 - _lcd->getFontXsize()*strlen(cancelText);
    const int cancelY = 215;  
    _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
    //_lcd->print(cancelS,cancelX,cancelY);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(cancelX-1,cancelY-5,cancelX+_lcd->getFontXsize()*strlen(cancelText),cancelY-5);
    buttonArray[2] = _buttons.addButton(cancelX, cancelY, cancelText);
    
  } else {
    buttonArray[2] = -1; 
  }
}

//Makes window decoration and buttons
void GUI::printMainMenu() {
  
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  const int controllerY = 135;
  const int xSpacer = 15;
  const int systemY = 60;
  //Rehacer alineacion X para iconos
  _lcd->setFont(hallfetica_normal);
  
  //TODO: MAke into check the dark version
  _lcd->drawBitmap (xSpacer, systemY-18, 64, 64, plant64);
  //Change to light version
  _lcd->drawBitmap (xSpacer, controllerY-18, 64, 64, settings64);
  
  //Make menu buttons. System and Controller settings
  mainMenuButtons[3] = _buttons.addButton(xSpacer+70,systemY,mainMenuButtonText[0]);
  mainMenuButtons[4] = _buttons.addButton(xSpacer+70,controllerY,mainMenuButtonText[1]);
}

//Draws main menu into LCD
void GUI::drawMainMenu() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Main Menu -"); 
  printFlowButtons(false,false,true,mainMenuButtons);
  printMainMenu(); 
  _buttons.drawButtons();
}

// Processes touch for main menu screen
void GUI::processTouchMainMenu(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Exit
  if (buttonIndex == mainMenuButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  //System settings button
  } else if (buttonIndex == mainMenuButtons[3]) {
    //Go to system settings
    _actScreen = 2;
    _lcd->fillScr(VGA_WHITE);
    drawSystemSettings();
  //Controller Settings button    
  } else if (buttonIndex == mainMenuButtons[4]) {
    //Go to controller settings  
    _actScreen = 3;
    _lcd->fillScr(VGA_WHITE);
    drawControllerSettings();
  }
}

//Prepares window for drawing
void GUI::printSystemSettings() {
  const int xSpacer = 15;
  const int ySpacer = 35;

  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  //Print triangles
  _lcd->setFont(various_symbols);
  //Before 3 there are the flow buttons
  for (int i = 0; i < nSystemButtons -3; i++) {
    _lcd->print("T",xSpacer,ySpacer+_lcd->getFontXsize()*2*i);
  }  
  
  //Make menu buttons
  //-3 because back/save/exit are already added at this stage
  for (int i = 0; i < nSystemButtons - 3; i++) {
    systemButtons[i + 3] = _buttons.addButton(xSpacer+_lcd->getFontXsize()*2,ySpacer+_lcd->getFontXsize()*2*i,systemButtonText[i]);
  } 
   _lcd->setFont(hallfetica_normal);
  
  //Watering at night ON/OFF 
  if (_settings->getNightWatering())
	_lcd->print("ON",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(systemButtonText[3]),ySpacer+_lcd->getFontXsize()*2*3);
  else
	_lcd->print("OFF",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(systemButtonText[3]),ySpacer+_lcd->getFontXsize()*2*3);
  
  //Manual Pump ON/OFF	
  if (_settings->getManualPump())
    _lcd->print("ON",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(systemButtonText[4]),ySpacer+_lcd->getFontXsize()*2*4);
  else
	_lcd->print("OFF",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(systemButtonText[4]),ySpacer+_lcd->getFontXsize()*2*4);

  //7 buttons
  // _lcd->print("T",xSpacer,ySpacer+_lcd->getFontXsize()*1.5*i);
  //systemButtons[i + 3] = _buttons.addButton(xSpacer+_lcd->getFontXsize()*2,ySpacer+_lcd->getFontXsize()*1.5*i,systemButtonText[i]);
  //if (waterAtNight)
  //_lcd->print("ON",xSpacer+_lcd->getFontXsize()*3+_lcd->getFontXsize()*strlen(systemButtonText[5]),ySpacer+_lcd->getFontXsize()*1.5*5);
  //if !(manualWaterPump)
  //_lcd->print("OFF",xSpacer+_lcd->getFontXsize()*3+_lcd->getFontXsize()*strlen(systemButtonText[6]),ySpacer+_lcd->getFontXsize()*1.5*6);

}

void GUI::drawSystemSettings() {
  _buttons.deleteAllButtons();
  printMenuHeader("- System Settings -");
  printFlowButtons(true,false,true,systemButtons);
  printSystemSettings(); 
  _buttons.drawButtons();
}

void GUI::processTouchSystem(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == systemButtons[0]) {
    //Go to main menu
    _actScreen = 1;
    _lcd->fillScr(VGA_WHITE);
    drawMainMenu();
  //Exit
  } else if (buttonIndex == systemButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  
  //Watering Cycle  
  } else if (buttonIndex == systemButtons[3]) {
    _actScreen = 7;
    _lcd->fillScr(VGA_WHITE);
    drawWaterCycle();    
  //Sensor Alarms  
  } else if (buttonIndex == systemButtons[4]) {
    _actScreen = 8;
    _lcd->fillScr(VGA_WHITE);
    drawSensorAlarms(); 
  //Sensor Calibration  
  } else if (buttonIndex == systemButtons[5]) {
    _actScreen = 13;
    _lcd->fillScr(VGA_WHITE);
    drawSensorCalibration();
    //Water at night Toggle 
  } else if (buttonIndex == systemButtons[5]) {
    processTouchWaterAtNight(x,y);
    //Manual pump toggle
  } else if (buttonIndex == systemButtons[5]) {
    processTouchManualPump(x,y);
  }

}

void GUI::printControllerSettings() {
  const int xSpacer = 15;
  const int ySpacer = 40;
  
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  //Print triangles
  _lcd->setFont(various_symbols);
  for (int i = 0; i < nControllerButtons - 3; i++) {
    _lcd->print("T",xSpacer,ySpacer+_lcd->getFontXsize()*2*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nControllerButtons - 3; i++) {
    controllerButtons[i + 3] = _buttons.addButton(xSpacer+_lcd->getFontXsize()*2,ySpacer+_lcd->getFontXsize()*2*i,controllerButtonText[i]);
  }
  
  _lcd->setFont(hallfetica_normal); 
  
  //Sound ON/OFF
  if (_settings->getSound())
    _lcd->print("ON",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(controllerButtonText[3]),ySpacer+_lcd->getFontXsize()*2*3);
  else
	_lcd->print("OFF",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(controllerButtonText[3]),ySpacer+_lcd->getFontXsize()*2*3);
  //Serial Debug ON/OFF
  if (_settings->getSerialDebug())
    _lcd->print("ON",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(controllerButtonText[4]),ySpacer+_lcd->getFontXsize()*2*4);
  else
	_lcd->print("OFF",xSpacer+_lcd->getFontXsize()*2+_lcd->getFontXsize()*strlen(controllerButtonText[4]),ySpacer+_lcd->getFontXsize()*2*4);
  
  //This goes to waitForControllerSettings or Buttons.h
//  _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
//  _lcd->setBackColor(VGA_WHITE);
//  _lcd->setFont(various_symbols);
//  _lcd->print("e",xSpacer,40+_lcd->getFontXsize()*2*3);
//  _lcd->setFont(hallfetica_normal);
//  _lcd->print(buttonText[3],xSpacer+_lcd->getFontXsize()*2,40+_lcd->getFontXsize()*2*3);
}

void GUI::drawControllerSettings() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Controller Settings -"); 
  printFlowButtons(true,false,true,controllerButtons);
  printControllerSettings();
  _buttons.drawButtons();
}

void GUI::processTouchController(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == controllerButtons[0]) {
    //Go to main menu
    _actScreen = 1;
    _lcd->fillScr(VGA_WHITE);
    drawMainMenu();
  //Exit
  } else if (buttonIndex == controllerButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  
  //Time & Date  
  } else if (buttonIndex == controllerButtons[3]) {
    _actScreen = 4;
    _lcd->fillScr(VGA_WHITE);
    drawTimeSettings();
  //Sensor polling
  } else if (buttonIndex == controllerButtons[4]) {
    _actScreen = 5;
    _lcd->fillScr(VGA_WHITE);
    drawSensorPolling();
  //SD Card
  } else if (buttonIndex == controllerButtons[5]) {
    _actScreen = 6;
    _lcd->fillScr(VGA_WHITE);
    drawSDcard();
  //Sound toggle
  } else if (buttonIndex == controllerButtons[6]) {
    processTouchSound(x,y);
  //Serial debugging toggle
  } else if (buttonIndex == controllerButtons[7]) {
    processTouchSerial(x,y);
  }
  
}

void GUI::printTimeSettings() {
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
  int hourTMP = hour(t);
  int minTMP = minute(t);
  int secTMP = second(t);
  int dayTMP = day(t);
  int monthTMP = month(t);
  int yearTMP = year(t);

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
  int fontSize = _lcd->getFontXsize();
  char* timeS = "Time";
  _lcd->setFont(hallfetica_normal);  
  _lcd->print(timeS, xSpacer, yTime);    
  _lcd->setFont(Sinclair_S);
  char* hhmmssS = "(HH:MM:SS)";
  //(strlen(timeS)*fontSize)/2 is middle point of "Time". (strlen(hhmmss)*_lcd->getFontXsize())/2 is middle point of "(HH:MM:SS)"
  //So the x coord is xTime + middle "Time" - middle of "(HH:MM:SS)"
  _lcd->print(hhmmssS, xSpacer+(strlen(timeS)*fontSize)/2-(strlen(hhmmssS)*_lcd->getFontXsize())/2, yTime+fontSize+2);  
  
  fontSize = _lcd->getFontXsize();
  _lcd->setFont(hallfetica_normal); 
  _lcd->printNumI(hourTMP,houU[0]+fontSize/2-_lcd->getFontXsize()+2,yTime,2,'0');
  _lcd->print(":",houU[0]+39,yTime);
  _lcd->printNumI(minTMP,minU[0]+fontSize/2-_lcd->getFontXsize()+2,yTime,2,'0');
  _lcd->print(":",minU[0]+39,yTime);
  _lcd->printNumI(secTMP,secU[0]+fontSize/2-_lcd->getFontXsize()+2,yTime,2,'0');

  //DATE 
  fontSize = _lcd->getFontXsize();
  char* dateS = "Date";
  _lcd->print(dateS, xSpacer, yDate);    
  _lcd->setFont(Sinclair_S);
  char* ddmmyyyyS = "(DD/MM/YYYY)";
  _lcd->print(ddmmyyyyS, xSpacer+(strlen(dateS)*fontSize)/2-(strlen(ddmmyyyyS)*_lcd->getFontXsize())/2, yDate+fontSize+2); 
  
  fontSize = _lcd->getFontXsize();
  _lcd->setFont(hallfetica_normal); 
  _lcd->printNumI(dayTMP, dayU[0]+fontSize/2-_lcd->getFontXsize()+2, yDate,2,'0');
  _lcd->print("/", dayU[0]+39, yDate);
  _lcd->printNumI(monthTMP, monU[0]+fontSize/2-_lcd->getFontXsize()+2, yDate,2,'0');
  _lcd->print("/", monU[0]+39, yDate);
  _lcd->printNumI(yearTMP, yeaU[0]+fontSize/2-_lcd->getFontXsize()+2, yDate,4);
  
}

void GUI::drawTimeSettings() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Time & Date -");
  printFlowButtons(true,true,true,timeButtons);
  printTimeSettings();  
  _buttons.drawButtons();
}

void GUI::processTouchTime(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == timeButtons[0]) {
    //Go to controller menu
    _actScreen = 3;
    _lcd->fillScr(VGA_WHITE);
    drawControllerSettings();
  //Save
  } else if (buttonIndex == timeButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == timeButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }
}

void GUI::printSensorPolling() {
  const int yFirstLine = 60;
  const int ySecondLine = 135;
  const int xSpacer = 25;
  const int xSpacer2 = 72;
  
  const int minU[] = {xSpacer2+8, ySecondLine-22};       //min up
  const int secU[] = {xSpacer2+8+9*_lcd->getFontXsize(), ySecondLine-22};       //sec up
  const int minD[] = {xSpacer2+8, ySecondLine+22};       //min down
  const int secD[] = {xSpacer2+8+9*_lcd->getFontXsize(), ySecondLine+22};       //sec down
  
  int minTMP = _settings->getSensorMinute();
  int secTMP = _settings->getSensorSecond();
  
  _lcd->setColor(grey[0],grey[1],grey[2]);  
  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(hallfetica_normal);
  
  char* sensorPollingText1 = "Time between readings:";
  char* sensorPollingText2 = "mins,";
  char* sensorPollingText3 = "secs";
  
  //Time between readings text
  _lcd->print(sensorPollingText1, xSpacer, yFirstLine);
  //XX
  _lcd->printNumI(minTMP,xSpacer2,ySecondLine,2,'0');
  //mins,
  int x = xSpacer2+3*_lcd->getFontXsize();
  _lcd->print(sensorPollingText2, x, ySecondLine);
  //XX
  x += _lcd->getFontXsize()*(strlen(sensorPollingText2)+1);
  _lcd->printNumI(secTMP,x,ySecondLine,2,'0');
  //secs
  x += _lcd->getFontXsize()*3;
  _lcd->print(sensorPollingText3,x,ySecondLine);
  
  //Make +/- buttons
  sensorPollingButtons[3] = _buttons.addButton(minU[0],minU[1],sensorPollingButtonText[0],BUTTON_SYMBOL);
  sensorPollingButtons[4] = _buttons.addButton(secU[0],secU[1],sensorPollingButtonText[1],BUTTON_SYMBOL);
  sensorPollingButtons[5] = _buttons.addButton(minD[0],minD[1],sensorPollingButtonText[2],BUTTON_SYMBOL);
  sensorPollingButtons[6] = _buttons.addButton(secD[0],secD[1],sensorPollingButtonText[3],BUTTON_SYMBOL);
 
}

void GUI::drawSensorPolling() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Sensor Polling -");
  printFlowButtons(true,true,true,sensorPollingButtons);
  printSensorPolling();  
  _buttons.drawButtons();
}

void GUI::processTouchSensorPolling(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == sensorPollingButtons[0]) {
    //Go to controller menu
    _actScreen = 3;
    _lcd->fillScr(VGA_WHITE);
    drawControllerSettings();
  //Save
  } else if (buttonIndex == sensorPollingButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == sensorPollingButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }
  
}

void GUI::printSDcard() {
  const int yFirstLine = 60;
  const int ySecondLine = 135;
  const int xSpacer = 25;
  
  const int houU[] = {217, ySecondLine-22};       //hour up
  const int minU[] = {280, ySecondLine-22};       //min up
  const int houD[] = {217, ySecondLine+22};       //hour down
  const int minD[] = {280, ySecondLine+22};       //min down
  
  int houTMP = _settings->getSDhour();
  int minTMP = _settings->getSDminute();
  
  //Triangle symbol
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(various_symbols);
  _lcd->print("T",xSpacer,yFirstLine);
  //First line button
  sdCardButtons[3] = _buttons.addButton(xSpacer+2*_lcd->getFontXsize(),yFirstLine,sdCardButtonsText[0]);
  //On - off symbol - 
  _lcd->setFont(hallfetica_normal);
  if (_settings->getSDactive())
	_lcd->print("ON",xSpacer+((3+strlen(sdCardButtonsText[0]))*_lcd->getFontXsize()),yFirstLine);
  else
	_lcd->print("OFF",xSpacer+((3+strlen(sdCardButtonsText[0]))*_lcd->getFontXsize()),yFirstLine);
  
  //Second line
  _lcd->setColor(grey[0],grey[1],grey[2]);  
  char* sdCardText1 = "Save every";
  _lcd->print(sdCardText1,xSpacer,ySecondLine);
  //hours
  int x = houU[0]+_lcd->getFontXsize()/2-_lcd->getFontXsize()+2;
  _lcd->printNumI(houTMP,x,ySecondLine,2,'0');
  x += 2*_lcd->getFontXsize();
  _lcd->print("h",x,ySecondLine);
  //Mins
  x += 2*_lcd->getFontXsize();
  _lcd->printNumI(minTMP,x,ySecondLine,2,'0');
  x += 2*_lcd->getFontXsize();
  _lcd->print("m",x,ySecondLine);
 
  //Make +/- buttons
  sdCardButtons[4] = _buttons.addButton(houU[0],houU[1],sdCardButtonsText[1],BUTTON_SYMBOL);
  sdCardButtons[5] = _buttons.addButton(minU[0],minU[1],sdCardButtonsText[2],BUTTON_SYMBOL);
  sdCardButtons[6] = _buttons.addButton(houD[0],houD[1],sdCardButtonsText[3],BUTTON_SYMBOL);
  sdCardButtons[7] = _buttons.addButton(minD[0],minD[1],sdCardButtonsText[4],BUTTON_SYMBOL);
 
}

void GUI::drawSDcard() {
  _buttons.deleteAllButtons();
  printMenuHeader("- SD Card -");
  printFlowButtons(true,true,true,sdCardButtons);
  printSDcard();  
  _buttons.drawButtons();
}

void GUI::processTouchSDcard(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == sdCardButtons[0]) {
    //Go to controller menu
    _actScreen = 3;
    _lcd->fillScr(VGA_WHITE);
    drawControllerSettings();
  //Save
  } else if (buttonIndex == sdCardButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == sdCardButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }  
}

void GUI::processTouchSound(int x, int y) {
  //TODO: toggle sound, redraw ON/OFF
}

void GUI::processTouchSerial(int x, int y) {
  //TODO: toggle serial, redraw ON/OFF
}

void GUI::printWaterCycle() {
  const int yFirstLine = 50;
  const int ySecondLine = 100;
  const int yThirdLine = 160;
  const int xSpacer = 25;
  
  const int houU[] = {240, ySecondLine-22};       //hour up
  const int houD[] = {240, ySecondLine+22};       //hour down
  const int minU[] = {305, ySecondLine-22};       //min up
  const int minD[] = {305, ySecondLine+22};       //min down
  const int fMinU[] = {225, yThirdLine-22};       //active for min
  const int fMinD[] = {225, yThirdLine+22};       //active for min
  
  int wHour = _settings->getWaterHour();
  int wMin = _settings->getWaterMinute();
  int fMin = _settings->getFloodMinute();
  
  //First Line - Triangle
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setFont(various_symbols);
  _lcd->print("T",xSpacer,yFirstLine);
  //Water mode button
  _lcd->setFont(hallfetica_normal);
  int x = xSpacer + 2*_lcd->getFontXsize();
  waterCycleButtons[4] = _buttons.addButton(x,yFirstLine,waterCycleButtonsText[0]);
  //Continuous/timed text
  x += (1+strlen(waterCycleButtonsText[0]))*_lcd->getFontXsize();
  if (_settings->getWaterTimed())
	_lcd->print("Timed",x,yFirstLine);
  else
	_lcd->print("Continuous",x,yFirstLine);
	
  //Second Line
  _lcd->setColor(grey[0],grey[1],grey[2]);
  x = xSpacer;
  _lcd->print("Water every:",x,ySecondLine);
  x += 13*_lcd->getFontXsize();
  _lcd->printNumI(wHour,x,ySecondLine,2,'0');
  x += 2*_lcd->getFontXsize();
  _lcd->print("h",x,ySecondLine);
  x += 2*_lcd->getFontXsize();
  _lcd->printNumI(wMin,x,ySecondLine,2,'0'); 
  x += 2*_lcd->getFontXsize();  
  _lcd->print("m",x,ySecondLine);
  waterCycleButtons[5] = _buttons.addButton(houU[0],houU[1],waterCycleButtonsText[1],BUTTON_SYMBOL);
  waterCycleButtons[6] = _buttons.addButton(houD[0],houD[1],waterCycleButtonsText[2],BUTTON_SYMBOL);
  waterCycleButtons[7] = _buttons.addButton(minU[0],minU[1],waterCycleButtonsText[3],BUTTON_SYMBOL);
  waterCycleButtons[8] = _buttons.addButton(minD[0],minD[1],waterCycleButtonsText[4],BUTTON_SYMBOL);
  
  //Third line
  x = xSpacer;
  _lcd->print("Active for:",x,yThirdLine);
  x += 12*_lcd->getFontXsize();
  _lcd->printNumI(fMin,x,yThirdLine,2,'0'); 
  x += 3*_lcd->getFontXsize();
  _lcd->print("minutes",x,yThirdLine);
  waterCycleButtons[9] = _buttons.addButton(fMinU[0],fMinU[1],waterCycleButtonsText[5],BUTTON_SYMBOL);
  waterCycleButtons[10] = _buttons.addButton(fMinD[0],fMinD[1],waterCycleButtonsText[6],BUTTON_SYMBOL);

}

void GUI::drawWaterCycle() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Watering Cycle -");
  printFlowButtons(true,true,true,waterCycleButtons);
  printWaterCycle();  
  _buttons.drawButtons();
}

void GUI::processTouchWaterCycle(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == waterCycleButtons[0]) {
    //Go to system menu
    _actScreen = 2;
    _lcd->fillScr(VGA_WHITE);
    drawSystemSettings();
  //Save
  } else if (buttonIndex == waterCycleButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == waterCycleButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  } 
}

void GUI::printSensorAlarms() {
  const int xSpacer = 15;
  const int ySpacer = 50;
  
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  //Print triangles
  _lcd->setFont(various_symbols);
  for (int i = 0; i < nSensorAlarmsButtons - 3; i++) {
    _lcd->print("T",xSpacer,ySpacer+_lcd->getFontXsize()*2.5*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nSensorAlarmsButtons - 3; i++) {
    sensorAlarmsButtons[i + 3] = _buttons.addButton(xSpacer+_lcd->getFontXsize()*2,ySpacer+_lcd->getFontXsize()*2.5*i,sensorAlarmsButtonsText[i]);
  }  
}

void GUI::drawSensorAlarms() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Sensor Alarms -");
  printFlowButtons(true,false,true,sensorAlarmsButtons);
  printSensorAlarms();  
  _buttons.drawButtons();
}

void GUI::processTouchSensorAlarms(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == sensorAlarmsButtons[0]) {
    //Go to system menu
    _actScreen = 2;
    _lcd->fillScr(VGA_WHITE);
    drawSystemSettings();
  //Exit
  } else if (buttonIndex == sensorAlarmsButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen(); 
  
  //Auto config
  } else if (buttonIndex == sensorAlarmsButtons[3]) {
    _actScreen = 12;
    _lcd->fillScr(VGA_WHITE);
    drawAutoConfig();
  //pH Thresholds
  } else if (buttonIndex == sensorAlarmsButtons[4]) {
    _actScreen = 9;
    _lcd->fillScr(VGA_WHITE);
    drawPHalarms();
  //EC Thresholds
  } else if (buttonIndex == sensorAlarmsButtons[5]) {
    _actScreen = 10;
    _lcd->fillScr(VGA_WHITE);
    drawECalarms();
  //Nutrient Level
  } else if (buttonIndex == sensorAlarmsButtons[6]) {
    _actScreen = 11;
    _lcd->fillScr(VGA_WHITE);
    drawWaterAlarms();
  }
}

void GUI::printPHalarms() {
  const int yFirstLine = 65;
  const int ySecondLine = 140;
  const int xSpacer = 25;
  const int signSpacer = 22; 
  
  float phUlimit = _settings->getPHalarmUp();
  float phDlimit = _settings->getPHalarmDown();
  
  char* uLimit = "Upper Limit:";
  char* dLimit = "Lower Limit:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(uLimit,xSpacer,yFirstLine);
  _lcd->print(dLimit,xSpacer,ySecondLine);
  //Numbers
  int x = (4+strlen(uLimit))*_lcd->getFontXsize();
  _lcd->printNumF(phUlimit,2,x,yFirstLine,'.',4);
  _lcd->printNumF(phDlimit,2,x,ySecondLine,'.',4);
  //Buttons
  x += 1.5*_lcd->getFontXsize();
  phAlarmsButtons[4] = _buttons.addButton(x,yFirstLine-signSpacer,phAlarmsButtonsText[0],BUTTON_SYMBOL);
  phAlarmsButtons[5] = _buttons.addButton(x,yFirstLine+signSpacer,phAlarmsButtonsText[1],BUTTON_SYMBOL);
  phAlarmsButtons[6] = _buttons.addButton(x,ySecondLine-signSpacer,phAlarmsButtonsText[2],BUTTON_SYMBOL);
  phAlarmsButtons[7] = _buttons.addButton(x,ySecondLine+signSpacer,phAlarmsButtonsText[3],BUTTON_SYMBOL);
}

void GUI::drawPHalarms() {
  _buttons.deleteAllButtons();
  printMenuHeader("- pH Alarms -");
  printFlowButtons(true,true,true,phAlarmsButtons);
  printPHalarms();  
  _buttons.drawButtons();
}

void GUI::processTouchPHalarms(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == phAlarmsButtons[0]) {
    //Go to alarms menu
    _actScreen = 8;
    _lcd->fillScr(VGA_WHITE);
    drawSensorAlarms();
  //Save
  } else if (buttonIndex == phAlarmsButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == phAlarmsButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }   
}

void GUI::printECalarms() {
  const int yFirstLine = 65;
  const int ySecondLine = 140;
  const int xSpacer = 25;
  const int signSpacer = 22; 
  
  uint32_t ecUlimit = _settings->getECalarmUp();
  uint32_t ecDlimit = _settings->getECalarmDown();
  
  char* uLimit = "Upper Limit:";
  char* dLimit = "Lower Limit:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(uLimit,xSpacer,yFirstLine);
  _lcd->print(dLimit,xSpacer,ySecondLine);
  //Numbers
  int x = (4+strlen(uLimit))*_lcd->getFontXsize();
  _lcd->printNumI(ecUlimit,x,yFirstLine,4);
  _lcd->printNumI(ecDlimit,x,ySecondLine,4);
  //Buttons
  x += 1.5*_lcd->getFontXsize();
  ecAlarmsButtons[4] = _buttons.addButton(x,yFirstLine-signSpacer,ecAlarmsButtonsText[0],BUTTON_SYMBOL);
  ecAlarmsButtons[5] = _buttons.addButton(x,yFirstLine+signSpacer,ecAlarmsButtonsText[1],BUTTON_SYMBOL);
  ecAlarmsButtons[6] = _buttons.addButton(x,ySecondLine-signSpacer,ecAlarmsButtonsText[2],BUTTON_SYMBOL);
  ecAlarmsButtons[7] = _buttons.addButton(x,ySecondLine+signSpacer,ecAlarmsButtonsText[3],BUTTON_SYMBOL);
  //uS Text
  x += 3.5*_lcd->getFontXsize();
  _lcd->print("uS",x,yFirstLine);
  _lcd->print("uS",x,ySecondLine);
}  

void GUI::drawECalarms() {
  _buttons.deleteAllButtons();
  printMenuHeader("- EC Alarms -");
  printFlowButtons(true,true,true,ecAlarmsButtons);
  printECalarms();  
  _buttons.drawButtons();
}

void GUI::processTouchECalarms(int x,int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == ecAlarmsButtons[0]) {
    //Go to alarms menu
    _actScreen = 8;
    _lcd->fillScr(VGA_WHITE);
    drawSensorAlarms();
  //Save
  } else if (buttonIndex == ecAlarmsButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == ecAlarmsButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }    
}

void GUI::printWaterAlarms() {
  const int yFirstLine = 100;
  const int xSpacer = 25;
  const int signSpacer = 22; 
  
  int wLimit = _settings->getWaterAlarm();
  
  char* wLimitS = "Lower Limit:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(wLimitS,xSpacer,yFirstLine);
  //Numbers
  int x = (4+strlen(wLimitS))*_lcd->getFontXsize();
  _lcd->printNumI(wLimit,x,yFirstLine);
  //Buttons
  x += 0.5*_lcd->getFontXsize();
  waterAlarmsButtons[4] = _buttons.addButton(x,yFirstLine-signSpacer,waterAlarmsButtonsText[0],BUTTON_SYMBOL);
  waterAlarmsButtons[5] = _buttons.addButton(x,yFirstLine+signSpacer,waterAlarmsButtonsText[1],BUTTON_SYMBOL);
  //percent sign
  x += 2.5*_lcd->getFontXsize();
  _lcd->print("%",x,yFirstLine);
}

void GUI::drawWaterAlarms() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Nutrient Alarms -");
  printFlowButtons(true,true,true,waterAlarmsButtons);
  printWaterAlarms();  
  _buttons.drawButtons(); 
}

void GUI::processTouchWaterAlarms(int x,int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == waterAlarmsButtons[0]) {
    //Go to alarms menu
    _actScreen = 8;
    _lcd->fillScr(VGA_WHITE);
    drawSensorAlarms();
  //Save
  } else if (buttonIndex == waterAlarmsButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == waterAlarmsButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }    
}

void GUI::printAutoConfig() {
 
}

void GUI::drawAutoConfig() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Auto Configuration -");
  printFlowButtons(true,true,true,autoConfigButtons);
  printAutoConfig();  
  _buttons.drawButtons();   
}

void GUI::processTouchAutoConfig(int x,int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == autoConfigButtons[0]) {
    //Go to alarms menu
    _actScreen = 8;
    _lcd->fillScr(VGA_WHITE);
    drawSensorAlarms();
  //Save
  } else if (buttonIndex == autoConfigButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == autoConfigButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }    
}

void GUI::printSensorCalibration() {
  const int xSpacer = 15;
  const int ySpacer = 45;
  
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  //Print triangles
  _lcd->setFont(various_symbols);
  for (int i = 0; i < nSensorCalibrationButtons - 3; i++) {
    _lcd->print("T",xSpacer,ySpacer+_lcd->getFontXsize()*2.5*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nSensorCalibrationButtons - 3; i++) {
    sensorCalibrationButtons[i + 3] = _buttons.addButton(xSpacer+_lcd->getFontXsize()*2,ySpacer+_lcd->getFontXsize()*2.5*i,sensorCalibrationButtonsText[i]);
  }  
}

void GUI::drawSensorCalibration() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Sensor Calibration -");
  printFlowButtons(true,false,true,sensorCalibrationButtons);
  printSensorCalibration();  
  _buttons.drawButtons();   
}

void GUI::processTouchSensorCalibration(int x, int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == sensorCalibrationButtons[0]) {
    //Go to system menu
    _actScreen = 2;
    _lcd->fillScr(VGA_WHITE);
    drawSystemSettings();
  //Exit
  } else if (buttonIndex == sensorCalibrationButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  
  //Water calibration
  } else if (buttonIndex == sensorCalibrationButtons[3]) {
    _actScreen = 14;
    _lcd->fillScr(VGA_WHITE);
    drawWaterCalibration();
  //pH calibration
  } else if (buttonIndex == sensorCalibrationButtons[4]) {
    _actScreen = 15;
    _lcd->fillScr(VGA_WHITE);
    drawPHcalibration();
  //EC calibration
  } else if (buttonIndex == sensorCalibrationButtons[5]) {
    _actScreen = 16;
    _lcd->fillScr(VGA_WHITE);
    drawECcalibration();
  //Light Calibration
  } else if (buttonIndex == sensorCalibrationButtons[6]) {
    _actScreen = 17;
    _lcd->fillScr(VGA_WHITE);
    drawLightCalibration();
  }
}

void GUI::printWaterCalibration() {
  
}

void GUI::drawWaterCalibration() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Nutrient Calibration -");
  printFlowButtons(true,true,true,waterLevelButtons);
  printWaterCalibration();  
  _buttons.drawButtons();   
}

void GUI::processTouchWaterCalibration(int x,int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == waterLevelButtons[0]) {
    //Go to calibration menu
    _actScreen = 13;
    _lcd->fillScr(VGA_WHITE);
    drawSensorCalibration();
  //Save
  } else if (buttonIndex == waterLevelButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == waterLevelButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }    
}

void GUI::printPHcalibration() {
    
}

void GUI::drawPHcalibration() {
  _buttons.deleteAllButtons();
  printMenuHeader("- pH Calibration -");
  printFlowButtons(true,true,true,phCalibrationButtons);
  printPHcalibration();  
  _buttons.drawButtons();  
}

void GUI::processTouchPHcalibration(int x,int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == phCalibrationButtons[0]) {
    //Go to calibration menu
    _actScreen = 13;
    _lcd->fillScr(VGA_WHITE);
    drawSensorCalibration();
  //Save
  } else if (buttonIndex == phCalibrationButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == phCalibrationButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }   
}

void GUI::printECcalibration() {
  
}

void GUI::drawECcalibration() {
  _buttons.deleteAllButtons();
  printMenuHeader("- EC Calibration -");
  printFlowButtons(true,true,true,ecCalibrationButtons);
  printECcalibration();  
  _buttons.drawButtons();  
}

void GUI::processTouchECcalibration(int x,int y) {
  int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == ecCalibrationButtons[0]) {
    //Go to calibration menu
    _actScreen = 13;
    _lcd->fillScr(VGA_WHITE);
    drawSensorCalibration();
  //Save
  } else if (buttonIndex == ecCalibrationButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == ecCalibrationButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  }    
}

void GUI::printLightCalibration() {
  
}

void GUI::drawLightCalibration() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Light Calibration -");
  printFlowButtons(true,true,true,lightCalibrationButtons);
  printPHcalibration();  
  _buttons.drawButtons();    
}

void GUI::processTouchLightCalibration(int x, int y) {
    int buttonIndex = _buttons.checkButtons(x,y);
  //Back
  if (buttonIndex == lightCalibrationButtons[0]) {
    //Go to calibration menu
    _actScreen = 13;
    _lcd->fillScr(VGA_WHITE);
    drawSensorCalibration();
  //Save
  } else if (buttonIndex == lightCalibrationButtons[1]) {
    //TODO: Do something!
  //Exit
  } else if (buttonIndex == lightCalibrationButtons[2]) {
    //Go to main screen
    _actScreen = 0;
    _lcd->fillScr(VGA_WHITE);
    drawMainScreen();
  } 
}

void GUI::processTouchWaterAtNight(int x, int y) {
  //Toggle night watering
}

void GUI::processTouchManualPump(int x, int y) {
  //Toggle Manual pump.
  //Change ON/OFF - Color RED
  //Go to main screen?
}
