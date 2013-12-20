#include "GUI.h"

//TODO: processTouchSystem & processTouchController:
//Save booleans to EEPROM when toggled or add SAVE button

//Constructors
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

//Destructor
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

//Getters
int GUI::getActScreen() const {
	return _actScreen;
}

//Reads x,y press and calls one function or another depending on active screen
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

//Prints header background and separator line
void GUI::printHeaderBackground() {
  const int headerHeight = 20; 
  //Header background
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->fillRect(0,0,xSize,headerHeight);
  //Separtator line
  _lcd->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
  _lcd->drawLine(0, headerHeight, xSize, headerHeight);  
}

//Prints main header text and clock
void GUI::printMainHeader() {    
  //Get actual time
  time_t t = now();
  uint8_t hou = hour(t);
  uint8_t min = minute(t);
  //uint8_t sec = second(t);

  printHeaderBackground(); 
  //Small TextS
  _lcd->setFont(Sinclair_S);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->print("Huertomato 1.2",10,6); 

  //Clock display
  //X is calculated from the end of size
  _lcd->printNumI(hou,xSize-(5*smallFontSize)-2,6,2,'0');
  _lcd->print(":",xSize-(3*smallFontSize)-2,6);
  _lcd->printNumI(min,xSize-(2*smallFontSize)-2,6,2,'0');
  //_lcd->print(":",xSize-(3*smallFontSize)-2,6);
  //_lcd->printNumI(sec,xSize-(2*smallFontSize)-2,6,2,'0');
  
    //Left
    //_lcd->print("20:55:33",2,2);
    //Center
    //_lcd->print("20:55:33",xSize/2-bigFontSize*4,2);
    //RIGHT
    //max X - 16px per letter X 8 letters - 2px spacing
    //_lcd->print("20:57:23",xSize-(bigFontSize*8)-2, 2);
    
    //Spacing is 4px between imgs (so its orig x + 16 + 4)
    //if (alarm)
    //  _lcd->drawBitmap(2, 2, 16, 16, alarm16);
    //  //if (manual control)
    //  _lcd->drawBitmap(22, 2, 16, 16, water16);
    //  //if (watering plants)
    //  _lcd->drawBitmap(42, 2, 16, 16, mano16);
}

//Updates main header's clock
void GUI::updateMainHeader() {
  //Get actual time
  time_t t = now();
  uint8_t hou = hour(t);
  uint8_t min = minute(t);
  //uint8_t sec = second(t);
  
  _lcd->setFont(Sinclair_S);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  //Clock display
  //X is calculated from the end of size
  _lcd->printNumI(hou,xSize-(5*smallFontSize)-2,6,2,'0');
  _lcd->print(":",xSize-(3*smallFontSize)-2,6);
  _lcd->printNumI(min,xSize-(2*smallFontSize)-2,6,2,'0');
  //_lcd->print(":",xSize-(3*smallFontSize)-2,6);
  //_lcd->printNumI(sec,xSize-(2*smallFontSize)-2,6,2,'0');
}

//Prints header with centered text
void GUI::printMenuHeader(char* c) {
  printHeaderBackground();
  _lcd->setFont(hallfetica_normal);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]); 
  //Print title centered
  _lcd->print(c,xSize/2-(bigFontSize*(strlen(c)/2)),2);
}

//Print sensor info on main screen. Data will turn red if theres an alarm triggered
void GUI::printSensorInfo() {
  const int xSpacer = xSize - 25;
  const int ySpacer = 35;
  	
  _lcd->setFont(hallfetica_normal);
  _lcd->setColor(grey[0], grey[1], grey[2]);
  _lcd->setBackColor(VGA_WHITE);

  //For x coord we take maxSize and extract letterSize*letters plus arbitrary spacing
  //Humidity
  int x = xSpacer-(bigFontSize*(strlen(sensorText[0])+4));
  _lcd->print(sensorText[0],x,ySpacer);
  _lcd->printNumI(_sensors->getHumidity(),xSpacer-bigFontSize*4,ySpacer,3,' '); 
  _lcd->print("%",xSpacer-bigFontSize,ySpacer);
  
  //Temp
  x = xSpacer-(bigFontSize*(strlen(sensorText[1])+7));
  int y = ySpacer+(bigFontSize+8);
  _lcd->print(sensorText[1],x,y);
  _lcd->printNumF(_sensors->getTemp(),2,xSpacer-bigFontSize*5,y,'.',4);
  _lcd->print("C",xSpacer-bigFontSize,y);
  
  //Light
  x = xSpacer-(bigFontSize*(strlen(sensorText[2])+4));
  y = ySpacer+(bigFontSize+8)*2;
  _lcd->print(sensorText[2],x,y);
  _lcd->printNumI(_sensors->getLight(),xSpacer-bigFontSize*4,y,3);
  _lcd->print("%",xSpacer-bigFontSize,y);
  
  //pH
  x = xSpacer-(bigFontSize*(strlen(sensorText[3])+5));
  y = ySpacer+(bigFontSize+8)*3;
  float ph = _sensors->getPH();
  if (ph > _settings->getPHalarmUp() || (ph < _settings->getPHalarmDown()))
    _lcd->setColor(red[0],red[1],red[2]);
  else
    _lcd->setColor(grey[0], grey[1], grey[2]);

  _lcd->print(sensorText[3],x,y);
  _lcd->printNumF(ph,2,xSpacer-bigFontSize*4,y,'.',4);
  
  //EC
  x = xSpacer-(bigFontSize*(strlen(sensorText[4])+7));
  y = ySpacer+(bigFontSize+8)*4;
  uint16_t ec = _sensors->getEC();
  if (ec > _settings->getECalarmUp() || (ec < _settings->getECalarmDown()))
    _lcd->setColor(red[0],red[1],red[2]);
  else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	
  _lcd->print(sensorText[4],x,y);
  _lcd->printNumI(ec,xSpacer-bigFontSize*6,y,4);
  _lcd->print("uS",xSpacer-bigFontSize*2,y);
  
  //Deposit level
  x = xSpacer-(bigFontSize*(strlen(sensorText[5])+4));
  y = ySpacer+(bigFontSize+8)*5;
  uint8_t lvl = _sensors->getWaterLevel();
  if (lvl < _settings->getWaterAlarm())
    _lcd->setColor(red[0],red[1],red[2]);
  else
	_lcd->setColor(grey[0], grey[1], grey[2]);
	
  _lcd->print(sensorText[5],x,y);
  _lcd->printNumI(lvl,xSpacer-bigFontSize*4,y,3);
  _lcd->print("%",xSpacer-bigFontSize,y);
}

//Redraws only sensor numbers. Changes colour if needed (alarm triggered)
void GUI::updateSensorInfo() {
    const int xSpacer = xSize - 25;
    const int ySpacer = 35;
    
    _lcd->setFont(hallfetica_normal);
    _lcd->setColor(grey[0], grey[1], grey[2]);
    _lcd->setBackColor(VGA_WHITE);

    //Humidity
    _lcd->printNumI(_sensors->getHumidity(),xSpacer-bigFontSize*4,ySpacer,3,' ');    
    //Temp
    int y = ySpacer+(bigFontSize+8);
    _lcd->printNumF(_sensors->getTemp(),2,xSpacer-bigFontSize*5,y,'.',4);   
    //Light
    y = ySpacer+(bigFontSize+8)*2;
    _lcd->printNumI(_sensors->getLight(),xSpacer-bigFontSize*4,y,3);    
    //pH
    y = ySpacer+(bigFontSize+8)*3;
    float ph = _sensors->getPH();
    if (ph > _settings->getPHalarmUp() || (ph < _settings->getPHalarmDown()))
      _lcd->setColor(red[0],red[1],red[2]);
    else
      _lcd->setColor(grey[0], grey[1], grey[2]);
    _lcd->printNumF(ph,2,xSpacer-bigFontSize*4,y,'.',4);   
    //EC
    y = ySpacer+(bigFontSize+8)*4;
    uint16_t ec = _sensors->getEC();
    if (ec > _settings->getECalarmUp() || (ec < _settings->getECalarmDown()))
      _lcd->setColor(red[0],red[1],red[2]);
    else
      _lcd->setColor(grey[0], grey[1], grey[2]);
    _lcd->printNumI(ec,xSpacer-bigFontSize*6,y,4);   
    //Deposit level
    y = ySpacer+(bigFontSize+8)*5;
    uint8_t lvl = _sensors->getWaterLevel();
    if (lvl < _settings->getWaterAlarm())
      _lcd->setColor(red[0],red[1],red[2]);
    else
      _lcd->setColor(grey[0], grey[1], grey[2]);    
    _lcd->printNumI(lvl,xSpacer-bigFontSize*4,y,3);
}

//Load img from SD: http://arduinodev.com/arduino-sd-card-image-viewer-with-tft-shield/
//Shows system status in main screen
void GUI::printIconAndStatus() {
  //Change Y coord.
  
  //If theres an alarm
  if (_settings->getAlarmTriggered()) {
    //_lcd->drawBitmap (15, 25+bigFontSize, 126, 126, alarm126);    
    _lcd->setColor(red[0],red[1],red[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("Alarm - Check Solution",10+bigFontSize*2,200);
  
  //watering and not in continuous mode
  } else if (_settings->getWaterTimed() && _settings->getWateringPlants()) {  
    //_lcd->drawBitmap (15, 25+bigFontSize, 126, 126, logo126);     
    _lcd->setColor(blue[0],blue[1],blue[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("Huertomato Watering",10+bigFontSize*2,200); 
  
  //water stopped because its night  
  } else if (_settings->getNightWateringStopped()) {
    //_lcd->drawBitmap (15, 25+bigFontSize, 126, 126, moon126);
    _lcd->setColor(grey[0],grey[1],grey[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("No Watering @ Night",10+bigFontSize*2,200);
    
  //water pump in manual mode    
  } else if (_settings->getManualPump()) {
	//_lcd->drawBitmap (15, 25+bigFontSize, 126, 126, hand126); 
	_lcd->setColor(red[0],red[1],red[2]);
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setFont(various_symbols);
	_lcd->print("T",10,200);
	_lcd->setFont(hallfetica_normal);
	_lcd->print("Pump Manually Enabled",10+bigFontSize*2,200);  

  //Normal operation    
  } else {
    _lcd->drawBitmap (15, 25+bigFontSize, 126, 126, plant126);  
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->setBackColor(VGA_WHITE);
    _lcd->setFont(various_symbols);
    _lcd->print("T",10,200);
    _lcd->setFont(hallfetica_normal);
    _lcd->print("Next Watering @ 17:56",10+bigFontSize*2,200);
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

//Redraws only values that change over time
//Used for refreshing from main sketch
void GUI::updateMainScreen() {
  updateMainHeader();
  //Should be changed to updateSensorInfo
  updateSensorInfo(); 
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
    _lcd->drawLine(backX-1,backY-5,backX+bigFontSize*strlen(backText),backY-5);
    buttonArray[0] = _buttons.addButton(backX, backY, backText);
    
    //Coords should be x1, y1, x2, y2
    //x1,y1 are line's x1,y1. x2 is line's x2. y2 is backY+bigFontSize+something (or maxY)
    //const int backB[]= {backX-1, backY-5, backX+bigFontSize*strlen(backS), maxY};
    
  } else {
    buttonArray[0] = -1;
  }
  
  if (saveButton) {
    char* saveText = " Save ";
    const int saveX = xSize/2 - bigFontSize*strlen(saveText)/2;
    const int saveY = 215;
    _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
    //_lcd->print(saveS,saveX,saveY);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(saveX-1,saveY-5,saveX+bigFontSize*strlen(saveText),saveY-5);
    buttonArray[1] = _buttons.addButton(saveX, saveY, saveText);
    
  } else {
    buttonArray[1] = -1; 
  }
  
  if (exitButton) { 
    char* cancelText = " Exit ";
    const int cancelX = xSize - 15 - bigFontSize*strlen(cancelText);
    const int cancelY = 215;  
    _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
    //_lcd->print(cancelS,cancelX,cancelY);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(cancelX-1,cancelY-5,cancelX+bigFontSize*strlen(cancelText),cancelY-5);
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
  
  nightWater = _settings->getNightWatering();
  waterPumpState = _settings->getManualPump();

  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  //Print triangles
  _lcd->setFont(various_symbols);
  //Before 3 there are the flow buttons
  for (int i = 0; i < nSystemButtons -3; i++) {
    _lcd->print("T",xSpacer,ySpacer+bigFontSize*2*i);
  }  
  
  //Make menu buttons
  //-3 because back/save/exit are already added at this stage
  for (int i = 0; i < nSystemButtons - 3; i++) {
    systemButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*2*i,systemButtonText[i]);
  } 
   _lcd->setFont(hallfetica_normal);
  
  //Watering at night ON/OFF 
  if (nightWater)
	_lcd->print("ON",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[3]),ySpacer+bigFontSize*2*3);
  else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[3]),ySpacer+bigFontSize*2*3);
  
  //Manual Pump ON/OFF	
  if (waterPumpState)
    _lcd->print("ON",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);
  else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);

  //7 buttons
  // _lcd->print("T",xSpacer,ySpacer+bigFontSize*1.5*i);
  //systemButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*1.5*i,systemButtonText[i]);
  //if (waterAtNight)
  //_lcd->print("ON",xSpacer+bigFontSize*3+bigFontSize*strlen(systemButtonText[5]),ySpacer+bigFontSize*1.5*5);
  //if !(manualWaterPump)
  //_lcd->print("OFF",xSpacer+bigFontSize*3+bigFontSize*strlen(systemButtonText[6]),ySpacer+bigFontSize*1.5*6);
}

void GUI::drawSystemSettings() {
  _buttons.deleteAllButtons();
  printMenuHeader("- System Settings -");
  printFlowButtons(true,false,true,systemButtons);
  printSystemSettings(); 
  _buttons.drawButtons();
}

//Redraws only system settings text from inner temp vars
//Used when +- signs are pressed
void GUI::updateSystemSettings() {
  const int xSpacer = 15;
  const int ySpacer = 35;
    
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setFont(hallfetica_normal);
	
  //Watering at night ON/OFF
  if (nightWater)
    _lcd->print("ON ",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[3]),ySpacer+bigFontSize*2*3);
  else
    _lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[3]),ySpacer+bigFontSize*2*3);	  
  //Manual Pump ON/OFF
  if (waterPumpState)
    _lcd->print("ON ",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);
  else
    _lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);

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
  } else if (buttonIndex == systemButtons[6]) {
	nightWater = !nightWater;
	//TODO: Save variables to EEPROM!?
    updateSystemSettings();
    //Manual pump toggle
  } else if (buttonIndex == systemButtons[7]) {
	waterPumpState = !waterPumpState;
	//TODO: Save variables to EEPROM!?
	//TODO: Go to main menu?
	updateSystemSettings();
  }

}

void GUI::printControllerSettings() {
  const int xSpacer = 15;
  const int ySpacer = 40;
  
  soundActive = _settings->getSound();
  serialActive = _settings->getSerialDebug();
  
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  
  //Print triangles
  _lcd->setFont(various_symbols);
  for (int i = 0; i < nControllerButtons - 3; i++) {
    _lcd->print("T",xSpacer,ySpacer+bigFontSize*2*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nControllerButtons - 3; i++) {
    controllerButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*2*i,controllerButtonText[i]);
  }
  
  _lcd->setFont(hallfetica_normal); 
  
  //Sound ON/OFF
  if (soundActive)
    _lcd->print("ON",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[3]),ySpacer+bigFontSize*2*3);
  else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[3]),ySpacer+bigFontSize*2*3);
  //Serial Debug ON/OFF
  if (serialActive)
    _lcd->print("ON",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[4]),ySpacer+bigFontSize*2*4);
  else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[4]),ySpacer+bigFontSize*2*4);
  
  //This goes to waitForControllerSettings or Buttons.h
//  _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
//  _lcd->setBackColor(VGA_WHITE);
//  _lcd->setFont(various_symbols);
//  _lcd->print("e",xSpacer,40+bigFontSize*2*3);
//  _lcd->setFont(hallfetica_normal);
//  _lcd->print(buttonText[3],xSpacer+bigFontSize*2,40+bigFontSize*2*3);
}

void GUI::drawControllerSettings() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Controller Settings -"); 
  printFlowButtons(true,false,true,controllerButtons);
  printControllerSettings();
  _buttons.drawButtons();
}

//Redraws only controller settings text from inner temp vars
//Used when +- signs are pressed
void GUI::updateControllerSettings() {
  const int xSpacer = 15;
  const int ySpacer = 40;
	
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setFont(hallfetica_normal);
	  
  //Sound ON/OFF
  if (soundActive)
    _lcd->print("ON ",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[3]),ySpacer+bigFontSize*2*3);
  else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[3]),ySpacer+bigFontSize*2*3);
  //Serial Debug ON/OFF
  if (serialActive)
	_lcd->print("ON ",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[4]),ySpacer+bigFontSize*2*4);
  else
	_lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(controllerButtonText[4]),ySpacer+bigFontSize*2*4);
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
    soundActive = !soundActive;
	//TODO: Save variables to EEPROM!?
	updateControllerSettings();
  //Serial debugging toggle
  } else if (buttonIndex == controllerButtons[7]) {
    serialActive = !serialActive;
	//TODO: Save variables to EEPROM!?
	updateControllerSettings();
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
  sysHour = hour(t);
  sysMin = minute(t);
  sysSec = second(t);
  sysDay = day(t);
  sysMonth = month(t);
  sysYear = year(t);

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
  _lcd->print(hhmmssS, xSpacer+(strlen(timeS)*bigFontSize)/2-(strlen(hhmmssS)*smallFontSize)/2, yTime+bigFontSize+2);  
  
  _lcd->setFont(hallfetica_normal); 
  _lcd->printNumI(sysHour,houU[0]+smallFontSize/2-bigFontSize+2,yTime,2,'0');
  _lcd->print(":",houU[0]+39,yTime);
  _lcd->printNumI(sysMin,minU[0]+smallFontSize/2-bigFontSize+2,yTime,2,'0');
  _lcd->print(":",minU[0]+39,yTime);
  _lcd->printNumI(sysSec,secU[0]+smallFontSize/2-bigFontSize+2,yTime,2,'0');

  //DATE 
  char* dateS = "Date";
  _lcd->print(dateS, xSpacer, yDate);    
  _lcd->setFont(Sinclair_S);
  char* ddmmyyyyS = "(DD/MM/YYYY)";
  _lcd->print(ddmmyyyyS, xSpacer+(strlen(dateS)*bigFontSize)/2-(strlen(ddmmyyyyS)*smallFontSize)/2, yDate+bigFontSize+2); 
  
  _lcd->setFont(hallfetica_normal); 
  _lcd->printNumI(sysDay, dayU[0]+smallFontSize/2-bigFontSize+2, yDate,2,'0');
  _lcd->print("/", dayU[0]+39, yDate);
  _lcd->printNumI(sysMonth, monU[0]+smallFontSize/2-bigFontSize+2, yDate,2,'0');
  _lcd->print("/", monU[0]+39, yDate);
  _lcd->printNumI(sysYear, yeaU[0]+smallFontSize/2-bigFontSize+2, yDate,4);
  
}

void GUI::drawTimeSettings() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Time & Date -");
  printFlowButtons(true,true,true,timeButtons);
  printTimeSettings();  
  _buttons.drawButtons();
}

//Redraws only time & date numbers from inner temp vars
//Used when +- signs are pressed
void GUI::updateTimeSettings() {
	const int yTime = 60;
	const int yDate = 135;
	  
	const int houU[] = {150, yTime-22};       //hour up
	const int minU[] = {220, yTime-22};       //min up
	const int secU[] = {290, yTime-22};       //sec up
	const int dayU[] = {150, yDate-22};       //day up
	const int monU[] = {220, yDate-22};       //month up
	const int yeaU[] = {290, yDate-22};       //year up
		  
	_lcd->setFont(hallfetica_normal); 
	_lcd->printNumI(sysHour,houU[0]+smallFontSize/2-bigFontSize+2,yTime,2,'0');
	_lcd->printNumI(sysMin,minU[0]+smallFontSize/2-bigFontSize+2,yTime,2,'0');
	_lcd->printNumI(sysSec,secU[0]+smallFontSize/2-bigFontSize+2,yTime,2,'0');
	
	_lcd->printNumI(sysDay, dayU[0]+smallFontSize/2-bigFontSize+2, yDate,2,'0');
	_lcd->printNumI(sysMonth, monU[0]+smallFontSize/2-bigFontSize+2, yDate,2,'0');
	_lcd->printNumI(sysYear, yeaU[0]+smallFontSize/2-bigFontSize+2, yDate,4);
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
  
  //Hour up
  } else if (buttonIndex == timeButtons[3]) {
	sysHour++;
	(sysHour > 23) ? sysHour=0 : 0;
	updateTimeSettings();  
  //Min up		  
  } else if (buttonIndex == timeButtons[4]) {
    sysMin++;
	(sysMin > 59) ? sysMin=0 : 0;
	updateTimeSettings();  	
  //Sec up
  } else if (buttonIndex == timeButtons[5]) {
	sysSec++;
	(sysSec > 59) ? sysSec=0 : 0;
	updateTimeSettings();
  //Hour down
  } else if (buttonIndex == timeButtons[6]) {
	//We cant just -- and compare with <0 as it overflows to 255
	(sysHour <= 0) ? sysHour=23 : sysHour--;
	updateTimeSettings();	
  //Min down
  } else if (buttonIndex == timeButtons[7]) {
	(sysMin <= 0) ? sysMin=59 : sysMin--;
	updateTimeSettings();
  //Sec down
  } else if (buttonIndex == timeButtons[8]) {
	(sysSec <= 0) ? sysSec=59 : sysSec--;
	updateTimeSettings();
  //Day up
  } else if (buttonIndex == timeButtons[9]) {
	sysDay++;
	(sysDay > 31) ? sysDay=1 : 0;
	updateTimeSettings();
  //Month up
  } else if (buttonIndex == timeButtons[10]) {
	sysMonth++;
	(sysMonth > 12) ? sysMonth=1 : 0;
	updateTimeSettings();
  //Year up
  } else if (buttonIndex == timeButtons[11]) {
	sysYear++;
	(sysYear > 2100) ? sysYear=1970 : 0;
	updateTimeSettings();
  //Day down
  } else if (buttonIndex == timeButtons[12]) {
	sysDay--;
	(sysDay < 1) ? sysDay=31 : 0;
	updateTimeSettings();
  //Month down
  } else if (buttonIndex == timeButtons[13]) {
	sysMonth--;
	(sysMonth < 1) ? sysMonth=12 : 0;
	updateTimeSettings();
  //Year down
  } else if (buttonIndex == timeButtons[14]) {
	sysYear--;
	(sysYear < 1970) ? sysYear=2100 : 0;
	updateTimeSettings();
  }
}

void GUI::printSensorPolling() {
  const int yFirstLine = 60;
  const int ySecondLine = 135;
  const int xSpacer = 25;
  const int xSpacer2 = 72;
  
  const int minU[] = {xSpacer2+8, ySecondLine-22};       //min up
  const int secU[] = {xSpacer2+8+9*bigFontSize, ySecondLine-22};       //sec up
  const int minD[] = {xSpacer2+8, ySecondLine+22};       //min down
  const int secD[] = {xSpacer2+8+9*bigFontSize, ySecondLine+22};       //sec down
  
  pollMin = _settings->getSensorMinute();
  pollSec = _settings->getSensorSecond();
  
  _lcd->setColor(grey[0],grey[1],grey[2]);  
  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(hallfetica_normal);
  
  char* sensorPollingText1 = "Time between readings:";
  char* sensorPollingText2 = "mins,";
  char* sensorPollingText3 = "secs";
  
  //Time between readings text
  _lcd->print(sensorPollingText1, xSpacer, yFirstLine);
  //XX
  _lcd->printNumI(pollMin,xSpacer2,ySecondLine,2,'0');
  //mins,
  int x = xSpacer2+3*bigFontSize;
  _lcd->print(sensorPollingText2, x, ySecondLine);
  //XX
  x += bigFontSize*(strlen(sensorPollingText2)+1);
  _lcd->printNumI(pollSec,x,ySecondLine,2,'0');
  //secs
  x += bigFontSize*3;
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

//Redraws only sensor polling numbers from inner temp vars
//Used when +- signs are pressed
void GUI::updateSensorPolling() {
    const int ySecondLine = 135;
    const int xSpacer2 = 72;
	char* sensorPollingText2 = "mins,";
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(pollMin,xSpacer2,ySecondLine,2,'0');
	int x = xSpacer2+3*bigFontSize;
	x += bigFontSize*(strlen(sensorPollingText2)+1);
	_lcd->printNumI(pollSec,x,ySecondLine,2,'0');
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
  
  //Min up
  } else if (buttonIndex == sensorPollingButtons[3]) {
	  pollMin++;
	  (pollMin > 59) ? pollMin=0 : 0;
	  updateSensorPolling();
  //Sec up
  } else if (buttonIndex == sensorPollingButtons[4]) {
	  pollSec++;
	  (pollSec > 59) ? pollSec=0 : 0;
	  updateSensorPolling();
  //Min down
  } else if (buttonIndex == sensorPollingButtons[5]) {
	  (pollMin <= 0) ? pollMin=59 : pollMin--;
	  updateSensorPolling();
  //Sec down
  } else if (buttonIndex == sensorPollingButtons[6]) {
	  (pollSec <= 0) ? pollSec=59 : pollSec--;
	  updateSensorPolling();  
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
  
  sdActive = _settings->getSDactive();
  sdHour = _settings->getSDhour();
  sdMin = _settings->getSDminute();
  
  //Triangle symbol
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(various_symbols);
  _lcd->print("T",xSpacer,yFirstLine);
  //First line button
  sdCardButtons[3] = _buttons.addButton(xSpacer+2*bigFontSize,yFirstLine,sdCardButtonsText[0]);
  //On - off symbol - 
  _lcd->setFont(hallfetica_normal);
  if (sdActive)
	_lcd->print("ON",xSpacer+((3+strlen(sdCardButtonsText[0]))*bigFontSize),yFirstLine);
  else
	_lcd->print("OFF",xSpacer+((3+strlen(sdCardButtonsText[0]))*bigFontSize),yFirstLine);
  
  //Second line
  _lcd->setColor(grey[0],grey[1],grey[2]);  
  char* sdCardText1 = "Save every";
  _lcd->print(sdCardText1,xSpacer,ySecondLine);
  //hours
  int x = houU[0]+bigFontSize/2-bigFontSize+2;
  _lcd->printNumI(sdHour,x,ySecondLine,2,'0');
  x += 2*bigFontSize;
  _lcd->print("h",x,ySecondLine);
  //Mins
  x += 2*bigFontSize;
  _lcd->printNumI(sdMin,x,ySecondLine,2,'0');
  x += 2*bigFontSize;
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

//Redraws only SD numbers & text from inner temp vars
//Used when +- signs are pressed
void GUI::updateSDcard() {
  const int yFirstLine = 60;
  const int ySecondLine = 135;
  const int xSpacer = 25;
  const int houU[] = {217, ySecondLine-22};       //hour up

  _lcd->setFont(hallfetica_normal);
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  if (sdActive)
    _lcd->print("ON ",xSpacer+((3+strlen(sdCardButtonsText[0]))*bigFontSize),yFirstLine);
  else
    _lcd->print("OFF",xSpacer+((3+strlen(sdCardButtonsText[0]))*bigFontSize),yFirstLine);

  _lcd->setColor(grey[0],grey[1],grey[2]); 
  //hours
  int x = houU[0]+bigFontSize/2-bigFontSize+2;
  _lcd->printNumI(sdHour,x,ySecondLine,2,'0');
  x += 2*bigFontSize;
  //Mins
  x += 2*bigFontSize;
  _lcd->printNumI(sdMin,x,ySecondLine,2,'0');
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
  
  //Card active toggle
  } else if (buttonIndex == sdCardButtons[3]) {
    sdActive = !sdActive;
	updateSDcard();
  //Hour up
  } else if (buttonIndex == sdCardButtons[4]) {
	sdHour++;
	(sdHour > 23) ? sdHour=0 : 0;
	updateSDcard();
  //Minute up
  } else if (buttonIndex == sdCardButtons[5]) {
	sdMin++;
	(sdMin > 59) ? sdMin=0 : 0;
	updateSDcard();	  
  //Hour down
  } else if (buttonIndex == sdCardButtons[6]) {
	(sdHour <= 0) ? sdHour=23 : sdHour--;
	updateSDcard();
  //Minute down
  } else if (buttonIndex == sdCardButtons[7]) {
	(sdMin <= 0) ? sdMin=59 : sdMin--;
	updateSDcard();
  }
  
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
  
  waterTimed = _settings->getWaterTimed();
  waterHour = _settings->getWaterHour();
  waterMin = _settings->getWaterMinute();
  floodMin = _settings->getFloodMinute();
  
  //First Line - Triangle
  _lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _lcd->setFont(various_symbols);
  _lcd->print("T",xSpacer,yFirstLine);
  //Water mode button
  _lcd->setFont(hallfetica_normal);
  int x = xSpacer + 2*bigFontSize;
  waterCycleButtons[3] = _buttons.addButton(x,yFirstLine,waterCycleButtonsText[0]);
  //Continuous/timed text
  x += (1+strlen(waterCycleButtonsText[0]))*bigFontSize;
  if (waterTimed)
	_lcd->print("Timed",x,yFirstLine);
  else
	_lcd->print("Continuous",x,yFirstLine);
	
  //Second Line
  _lcd->setColor(grey[0],grey[1],grey[2]);
  x = xSpacer;
  _lcd->print("Water every:",x,ySecondLine);
  x += 13*bigFontSize;
  _lcd->printNumI(waterHour,x,ySecondLine,2,'0');
  x += 2*bigFontSize;
  _lcd->print("h",x,ySecondLine);
  x += 2*bigFontSize;
  _lcd->printNumI(waterMin,x,ySecondLine,2,'0'); 
  x += 2*bigFontSize;  
  _lcd->print("m",x,ySecondLine);
  waterCycleButtons[4] = _buttons.addButton(houU[0],houU[1],waterCycleButtonsText[1],BUTTON_SYMBOL);
  waterCycleButtons[5] = _buttons.addButton(houD[0],houD[1],waterCycleButtonsText[2],BUTTON_SYMBOL);
  waterCycleButtons[6] = _buttons.addButton(minU[0],minU[1],waterCycleButtonsText[3],BUTTON_SYMBOL);
  waterCycleButtons[7] = _buttons.addButton(minD[0],minD[1],waterCycleButtonsText[4],BUTTON_SYMBOL);
  
  //Third line
  x = xSpacer;
  _lcd->print("Active for:",x,yThirdLine);
  x += 12*bigFontSize;
  _lcd->printNumI(floodMin,x,yThirdLine,2,'0'); 
  x += 3*bigFontSize;
  _lcd->print("minutes",x,yThirdLine);
  waterCycleButtons[8] = _buttons.addButton(fMinU[0],fMinU[1],waterCycleButtonsText[5],BUTTON_SYMBOL);
  waterCycleButtons[9] = _buttons.addButton(fMinD[0],fMinD[1],waterCycleButtonsText[6],BUTTON_SYMBOL);

}

void GUI::drawWaterCycle() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Watering Cycle -");
  printFlowButtons(true,true,true,waterCycleButtons);
  printWaterCycle();  
  _buttons.drawButtons();
}

//Redraws only water cycle numbers & text from inner temp vars
//Used when +- signs are pressed
void GUI::updateWaterCycle() {
	const int yFirstLine = 50;
	const int ySecondLine = 100;
	const int yThirdLine = 160;
	const int xSpacer = 25;
	
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	
	//Continuous/timed text
	int x = xSpacer + 2*bigFontSize;;
	x += (1+strlen(waterCycleButtonsText[0]))*bigFontSize;
	if (waterTimed)
		_lcd->print("Timed     ",x,yFirstLine);
	else
		_lcd->print("Continuous",x,yFirstLine);
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	//Water every
	x = xSpacer;
	x += 13*bigFontSize;
	_lcd->printNumI(waterHour,x,ySecondLine,2,'0');
	x += 2*bigFontSize;
	x += 2*bigFontSize;
	_lcd->printNumI(waterMin,x,ySecondLine,2,'0');
	//Flood time
	x = xSpacer;
	x += 12*bigFontSize;
	_lcd->printNumI(floodMin,x,yThirdLine,2,'0');
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
  
  //Water mode
  } else if (buttonIndex == waterCycleButtons[3]) {
	  waterTimed = !waterTimed;
	  updateWaterCycle();
  //Hour up  
  } else if (buttonIndex == waterCycleButtons[4]) {
	  waterHour++;
	  (waterHour > 23) ? waterHour=0 : 0;
	  updateWaterCycle();
  //Hour down
  } else if (buttonIndex == waterCycleButtons[5]) {
	  (waterHour <= 0) ? waterHour=23 : waterHour--;
	  updateWaterCycle();
  //Minute up
  } else if (buttonIndex == waterCycleButtons[6]) {
	  waterMin++;
	  (waterMin > 59) ? waterMin=0 : 0;
	  updateWaterCycle();
  //Minute down
  } else if (buttonIndex == waterCycleButtons[7]) {
	  (waterMin <= 0) ? waterMin=59 : waterMin--;
	  updateWaterCycle();
  //Flood minute up
  } else if (buttonIndex == waterCycleButtons[8]) {
	  floodMin++;
	  (floodMin > 59) ? floodMin=0 : 0;
	  updateWaterCycle();
  //Flood minute down
  } else if (buttonIndex == waterCycleButtons[9]) {
	  (floodMin <= 0) ? floodMin=59 : floodMin--;
	  updateWaterCycle();
  
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
    _lcd->print("T",xSpacer,ySpacer+bigFontSize*2.5*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nSensorAlarmsButtons - 3; i++) {
    sensorAlarmsButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*2.5*i,sensorAlarmsButtonsText[i]);
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
  
  phAlarmMax = _settings->getPHalarmUp();
  phAlarmMin = _settings->getPHalarmDown();
  
  char* uLimit = "Upper Limit:";
  char* dLimit = "Lower Limit:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(uLimit,xSpacer,yFirstLine);
  _lcd->print(dLimit,xSpacer,ySecondLine);
  //Numbers
  int x = (4+strlen(uLimit))*bigFontSize;
  _lcd->printNumF(phAlarmMax,2,x,yFirstLine,'.',5);
  _lcd->printNumF(phAlarmMin,2,x,ySecondLine,'.',5);
  //Buttons
  x += 2*bigFontSize;
  phAlarmsButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,phAlarmsButtonsText[0],BUTTON_SYMBOL);
  phAlarmsButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,phAlarmsButtonsText[1],BUTTON_SYMBOL);
  phAlarmsButtons[5] = _buttons.addButton(x,ySecondLine-signSpacer,phAlarmsButtonsText[2],BUTTON_SYMBOL);
  phAlarmsButtons[6] = _buttons.addButton(x,ySecondLine+signSpacer,phAlarmsButtonsText[3],BUTTON_SYMBOL);
}

void GUI::drawPHalarms() {
  _buttons.deleteAllButtons();
  printMenuHeader("- pH Alarms -");
  printFlowButtons(true,true,true,phAlarmsButtons);
  printPHalarms();  
  _buttons.drawButtons();
}

void GUI::updatePHalarms() {
	const int yFirstLine = 65;
	const int ySecondLine = 140;
	char* uLimit = "Upper Limit:";
	
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen(uLimit))*bigFontSize;
	_lcd->printNumF(phAlarmMax,2,x,yFirstLine,'.',5);
	_lcd->printNumF(phAlarmMin,2,x,ySecondLine,'.',5);	
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
	
  //Max up
  } else if(buttonIndex == phAlarmsButtons[3]) {
	  phAlarmMax += 0.05;
	  (phAlarmMax > 14) ? phAlarmMax=0 : 0;
	  updatePHalarms();
  //Max down	  
  } else if(buttonIndex == phAlarmsButtons[4]) {
	  phAlarmMax -= 0.05;
	  (phAlarmMax < 0) ? phAlarmMax=14 : 0;
	  updatePHalarms();
  //Min up
  } else if(buttonIndex == phAlarmsButtons[5]) {
	  phAlarmMin += 0.05;
	  (phAlarmMin > 14) ? phAlarmMin=0 : 0;
	  updatePHalarms();
  //Min down
  } else if(buttonIndex == phAlarmsButtons[6]) {
	  phAlarmMin -= 0.05;
	  (phAlarmMin < 0) ? phAlarmMin=14 : 0;
	  updatePHalarms();
  }   
}

void GUI::printECalarms() {
  const int yFirstLine = 65;
  const int ySecondLine = 140;
  const int xSpacer = 25;
  const int signSpacer = 22; 
  
  ecAlarmMax = _settings->getECalarmUp();
  ecAlarmMin = _settings->getECalarmDown();
  
  char* uLimit = "Upper Limit:";
  char* dLimit = "Lower Limit:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(uLimit,xSpacer,yFirstLine);
  _lcd->print(dLimit,xSpacer,ySecondLine);
  //Numbers
  int x = (4+strlen(uLimit))*bigFontSize;
  _lcd->printNumI(ecAlarmMax,x,yFirstLine,4);
  _lcd->printNumI(ecAlarmMin,x,ySecondLine,4);
  //Buttons
  x += 1.5*bigFontSize;
  ecAlarmsButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,ecAlarmsButtonsText[0],BUTTON_SYMBOL);
  ecAlarmsButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,ecAlarmsButtonsText[1],BUTTON_SYMBOL);
  ecAlarmsButtons[5] = _buttons.addButton(x,ySecondLine-signSpacer,ecAlarmsButtonsText[2],BUTTON_SYMBOL);
  ecAlarmsButtons[6] = _buttons.addButton(x,ySecondLine+signSpacer,ecAlarmsButtonsText[3],BUTTON_SYMBOL);
  //uS Text
  x += 3.5*bigFontSize;
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

void GUI::updateECalarms() {
	const int yFirstLine = 65;
	const int ySecondLine = 140;
	char* uLimit = "Upper Limit:";
	
	_lcd->setFont(hallfetica_normal); 
	int x = (4+strlen(uLimit))*bigFontSize;
	_lcd->printNumI(ecAlarmMax,x,yFirstLine,4);
	_lcd->printNumI(ecAlarmMin,x,ySecondLine,4);
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
	
  //Max up	
  } else if(buttonIndex == ecAlarmsButtons[3]) {
	ecAlarmMax += 10;
	(ecAlarmMax > 9999) ? ecAlarmMax=1000 : 0;
	updateECalarms();
  //Max down
  } else if(buttonIndex == ecAlarmsButtons[4]) {
	ecAlarmMax -= 10;
	(ecAlarmMax < 1000) ? ecAlarmMax=9999 : 0;
	updateECalarms();
  //Min up
  } else if(buttonIndex == ecAlarmsButtons[5]) {
	ecAlarmMin += 10;
	(ecAlarmMin > 9999) ? ecAlarmMin=1000 : 0;
	updateECalarms();
  //Min down
  } else if(buttonIndex == ecAlarmsButtons[6]) {
	ecAlarmMin -= 10;
	(ecAlarmMin < 1000) ? ecAlarmMin=9999 : 0;
	updateECalarms();
  }
}

void GUI::printWaterAlarms() {
  const int yFirstLine = 100;
  const int xSpacer = 25;
  const int signSpacer = 22; 
  
  waterAlarmMin = _settings->getWaterAlarm();
  
  char* wLimitS = "Lower Limit:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(wLimitS,xSpacer,yFirstLine);
  //Numbers
  int x = (4+strlen(wLimitS))*bigFontSize;
  _lcd->printNumI(waterAlarmMin,x,yFirstLine);
  //Buttons
  x += 0.5*bigFontSize;
  waterAlarmsButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,waterAlarmsButtonsText[0],BUTTON_SYMBOL);
  waterAlarmsButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,waterAlarmsButtonsText[1],BUTTON_SYMBOL);
  //percent sign
  x += 2.5*bigFontSize;
  _lcd->print("%",x,yFirstLine);
}

void GUI::drawWaterAlarms() {
  _buttons.deleteAllButtons();
  printMenuHeader("- Nutrient Alarms -");
  printFlowButtons(true,true,true,waterAlarmsButtons);
  printWaterAlarms();  
  _buttons.drawButtons(); 
}

void GUI::updateWaterAlarms() {
	const int yFirstLine = 100;
	char* wLimitS = "Lower Limit:";
	
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen(wLimitS))*bigFontSize;
	_lcd->printNumI(waterAlarmMin,x,yFirstLine);
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
  
  //Up
  } else if (buttonIndex == waterAlarmsButtons[3]) {
	waterAlarmMin++;
	(waterAlarmMin > 100) ? waterAlarmMin=0 : 0;
	updateWaterAlarms();
  //Down  
  } else if (buttonIndex == waterAlarmsButtons[4]) {
	(waterAlarmMin <= 0) ? waterAlarmMin=100 : waterAlarmMin--;
	updateWaterAlarms();
  } 
}

void GUI::printAutoConfig() {
  //TODO
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
    _lcd->print("T",xSpacer,ySpacer+bigFontSize*2.5*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nSensorCalibrationButtons - 3; i++) {
    sensorCalibrationButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*2.5*i,sensorCalibrationButtonsText[i]);
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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