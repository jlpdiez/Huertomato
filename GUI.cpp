#include "GUI.h"

//Constructors
GUI::GUI(UTFT *lcd, UTouch *touch, Sensors *sensors, Settings *settings)
: _lcd(lcd), _touch(touch), _sensors(sensors), _settings(settings), _window(lcd, touch) { }

GUI::GUI(const GUI &other) : _window(other._window) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	//_window = other._window;
}

GUI& GUI::operator=(const GUI &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	_sensors = other._sensors;
	_settings = other._settings;
	//*_window = *other._window;
	_window = other._window;
	
	return *this;
}

//Destructor
GUI::~GUI() { }

void GUI::init() {
	_lcd->InitLCD();
	_lcd->clrScr();
	_lcd->fillScr(VGA_WHITE);
	_touch->InitTouch();
	_touch->setPrecision(PREC_HI);
	//_window->drawSplashScreen();
}

void GUI::refresh() {
	//Refresh screens if needed
	//if (gui.getActScreen() == 0)
	//TODO:
	//gui.updateMainScreen();
	//Nutrient level calibration
	//else if (gui.getActScreen() == 14)
	//TODO:
	//gui.updateWaterCalibration();
	//Night threshold calibration
	//else if (gui.getActScreen() == 15)
	//TODO:
	//gui.updateLightCalibration();
}

boolean GUI::isMainScreen() {
	return _actScreen == 0;
}



void GUI::printWindow(const int screen) {
	switch (screen) {
		default:
		break;
	}
}

//Reads x,y press and calls one function or another depending on active screen
void GUI::processTouch() {
	if (_touch->dataAvailable()) {
		_touch->read();
		int x = _touch->getX();
		int y = _touch->getY();
    
		/*switch (_actScreen) {
			//Main Screen
			case 0: 
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
				//processTouchAutoConfig(x,y);
				break;
			//Sensor Calibration
			case 13:
				processTouchSensorCalibration(x,y);
				break;
			//Water Level Calibration
			case 14:
				processTouchWaterCalibration(x,y);
				break;
			//Light Calibration
			case 15:
				processTouchLightCalibration(x,y);
				break;
			//Pump Protection
			case 16:
				processTouchPumpProtection(x,y);
				break;
		}*/
	}
}
/*
//Draw splash Screen
//TODO: warn when no RTC or SD present- would be cool if we asked for a touchScreen
void GUI::drawSplashScreen() {
	const int iconSize = 126;
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(VGA_WHITE);
	//Shows centered icon
	_lcd->drawBitmap(xSize/2-(iconSize/2),10,iconSize,iconSize,logo126); 	
	//Shows centered text
	char* message = "Huertomato is loading...";
	_lcd->print(message,xSize/2-(bigFontSize*(strlen(message)/2)),50+iconSize);
}

//Prints header background and separator line
void GUI::printHeaderBackground() {
	const int headerHeight = 20; 
	//Header background
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->fillRect(0,0,xSize,headerHeight);
	//Separator line
	_lcd->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
	_lcd->drawLine(0, headerHeight, xSize, headerHeight);  
}

//Prints mainscreen header text and clock
void GUI::printMainHeader() {  
	//For small font y = 6. For big one y = 2;
	const int ySpacer = 2;
    
	//Get actual time
	time_t t = now();
	uint8_t hou = hour(t);
	uint8_t min = minute(t);
	uint8_t sec = second(t);

	printHeaderBackground(); 
  
	//Header title text
	_lcd->setFont(hallfetica_normal);
	_lcd->setColor(grey[0], grey[1], grey[2]);
	_lcd->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->print("Huertomato 1.2",10,ySpacer); 

	//Clock display HH:MM
	//X is calculated from the end of size
	_lcd->printNumI(hou,xSize-(5*bigFontSize)-2,ySpacer,2,'0');
	_lcd->print(":",xSize-(3*bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,xSize-(2*bigFontSize)-2,ySpacer,2,'0');
}

//Updates main header's clock
void GUI::updateMainHeader() {
	const int ySpacer = 2;
  
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
	_lcd->printNumI(hou,xSize-(5*bigFontSize)-2,ySpacer,2,'0');
	_lcd->print(":",xSize-(3*bigFontSize)-2,ySpacer);
	_lcd->printNumI(min,xSize-(2*bigFontSize)-2,ySpacer,2,'0');
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
	_lcd->printNumF(_sensors->getTemp(),2,xSpacer-bigFontSize*6,y,'.',5);
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

//Redraws only sensor numbers and changes colour if needed (alarm triggered)
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
    _lcd->printNumF(_sensors->getTemp(),2,xSpacer-bigFontSize*6,y,'.',5);   
    //Light
    y = ySpacer+(bigFontSize+8)*2;
    _lcd->printNumI(_sensors->getLight(),xSpacer-bigFontSize*4,y,3);    
	//pH
	int x = xSpacer-(bigFontSize*(strlen(sensorText[3])+5));
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

//Shows system status in main screen
//Loads img files from /PICTURE folder of the SD card
//TODO: make common part of alarm and normal state a function!
void GUI::printIconAndStatus() {
	const int xSpacer = 10;
	const int ySpacer = 200;
	const int imgSize = 126;
	File img;
	char* path;
	
	//Watering Stopped
	if (_settings->getNightWateringStopped()) {
		_lcd->setColor(grey[0],grey[1],grey[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		_lcd->print("No Watering @ Night   ",xSpacer+bigFontSize*2,ySpacer);
		path = "/PICTURE/moon126.RAW";
	
	//Timed mode and watering plants
	} else if (_settings->getWaterTimed() && _settings->getWateringPlants()) {
		_lcd->setColor(blue[0],blue[1],blue[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		char* watering = "Huertomato Watering";
		int x = xSpacer + bigFontSize*2;
		_lcd->print(watering,x,ySpacer);
		//3 blank chars afterwards to clear line
		x += bigFontSize*strlen(watering);
		_lcd->print("   ",x,ySpacer);
		path = "/PICTURE/logo126.RAW";
	
	//Alarm triggered
	} else if (_settings->getAlarmTriggered()) {
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		_lcd->setColor(red[0],red[1],red[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + bigFontSize*2;
		//Timed mode
		if (_settings->getWaterTimed()) {
			//Space char added after @ and last number to white out line
			char* nWater = "Next Watering @ ";
			_lcd->print(nWater,x,ySpacer);
			x += bigFontSize*(strlen(nWater));
			_lcd->printNumI(wHour,x,ySpacer,2,'0');
			x += bigFontSize*2;
			_lcd->print(":",x,ySpacer);
			x += bigFontSize;
			_lcd->printNumI(wMin,x,ySpacer,2,'0');
			x += 2*bigFontSize;
			_lcd->print(" ",x,ySpacer);
		//Continuous mode
		} else {
			char* nWater = "Alarm - Check Solution";
			_lcd->print(nWater,x,ySpacer);
		}
		path = "/PICTURE/alarm126.RAW";
	
	//Normal operation
	} else {
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		_lcd->print("T",xSpacer,ySpacer);
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + bigFontSize*2;
				
		//Timed mode
		if (_settings->getWaterTimed()) {
			//Space char added after @ and last number to white out line
			char* nWater = "Next Watering @ ";
			_lcd->print(nWater,x,ySpacer);
			x += bigFontSize*(strlen(nWater));
			_lcd->printNumI(wHour,x,ySpacer,2,'0');
			x += bigFontSize*2;
			_lcd->print(":",x,ySpacer);
			x += bigFontSize;
			_lcd->printNumI(wMin,x,ySpacer,2,'0');
			x += 2*bigFontSize;
			_lcd->print(" ",x,ySpacer);
		
		//Continuous
		} else {
			char* nWater = "System working fine   ";
			_lcd->print(nWater,x,ySpacer);
		}
		path = "/PICTURE/plant126.RAW";
	}
	
	//Read from SD line by line and display.
	//Requires new function at UTFT library 
	//Slow but effective
	if (SD.exists(path)) {
		img = SD.open(path,FILE_READ);
		for (int y = 0; y < imgSize && img.available(); y++) {
			uint16_t buf[imgSize];
			for (int x = imgSize - 1; x >= 0; x--) {
				byte l = img.read();
				byte h = img.read();
				buf[x] = ((uint16_t)h << 8) | l;
			}
			_lcd->drawPixelLine(15,y+25+bigFontSize,imgSize,buf);
		}
		img.close();
	} else
		_lcd->drawBitmap(15,25+bigFontSize,imgSize,imgSize,logo126); 
}

//Same as above except it only changes icon if system state changed from previous
//If not should only update next watering time
void GUI::updateIconAndStatus() {
	if (_settings->systemStateChanged())
		printIconAndStatus();
		
	//Updates next watering time if needed
	else if (!_settings->getNightWateringStopped() && !_settings->getWateringPlants()
				&& _settings->getWaterTimed()) {
		
		const int xSpacer = 10;
		const int ySpacer = 200;
		int wHour = _settings->getNextWhour();
		int wMin = _settings->getNextWminute();
		_lcd->setBackColor(VGA_WHITE);
		_lcd->setFont(various_symbols);
		
		//Alarm triggered
		if (_settings->getAlarmTriggered())		
			_lcd->setColor(red[0],red[1],red[2]);	
		//Normal operation
		else
			_lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
		
		_lcd->print("T",xSpacer,ySpacer);		
		_lcd->setFont(hallfetica_normal);
		int x = xSpacer + bigFontSize*2;
		//Space char added after @ and last number to white out line
		char* nWater = "Next Watering @ ";
		_lcd->print(nWater,x,ySpacer);
		x += bigFontSize*(strlen(nWater));
		_lcd->printNumI(wHour,x,ySpacer,2,'0');
		x += bigFontSize*2;
		_lcd->print(":",x,ySpacer);
		x += bigFontSize;
		_lcd->printNumI(wMin,x,ySpacer,2,'0');
		x += 2*bigFontSize;
		_lcd->print(" ",x,ySpacer);
	}
}

//GUI Starting point. Should be called from main sketch in setup()
//_actScreen == 0
void GUI::drawMainScreen() {
	_actScreen = 0;
	_lcd->fillScr(VGA_WHITE);
	printMainHeader();
	printSensorInfo();
	printIconAndStatus();
}

//Redraws only values that change over time
//Used for refreshing from main sketch
void GUI::updateMainScreen() {
	updateMainHeader();
	updateSensorInfo(); 
	updateIconAndStatus();
}

//These function should be the first to get its buttons into the array buttons
//It gets input button array and adds appropriate back/save/cancel to positions 0, 1 & 2 
void GUI::printFlowButtons(boolean backButton, boolean saveButton, boolean exitButton, int buttonArray[]) {
  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(hallfetica_normal);
  
  if (backButton) {
    const int backX = 15;
    const int backY = 215;
    char* backText = " Back ";
    //_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);   
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(backX-1,backY-5,backX+bigFontSize*strlen(backText),backY-5);
    buttonArray[0] = _buttons.addButton(backX, backY, backText);
  } else 
    buttonArray[0] = -1;
  
  if (saveButton) {
    char* saveText = " Save ";
    const int saveX = xSize/2 - bigFontSize*strlen(saveText)/2;
    const int saveY = 215;
    //_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(saveX-1,saveY-5,saveX+bigFontSize*strlen(saveText),saveY-5);
    buttonArray[1] = _buttons.addButton(saveX, saveY, saveText);    
  } else
    buttonArray[1] = -1; 
  
  if (exitButton) { 
    char* cancelText = " Exit ";
    const int cancelX = xSize - 15 - bigFontSize*strlen(cancelText);
    const int cancelY = 215;  
    //_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
    _lcd->setColor(darkGreen[0],darkGreen[1],darkGreen[2]);
    _lcd->drawLine(cancelX-1,cancelY-5,cancelX+bigFontSize*strlen(cancelText),cancelY-5);
    buttonArray[2] = _buttons.addButton(cancelX, cancelY, cancelText);   
  } else
    buttonArray[2] = -1; 
}

//Overlays "Saved" text over save button
//Used when button is pressed to inform the user values have been stored
void GUI::printSavedButton() {
	char* savedText = " Saved ";
	const int saveX = xSize/2 - bigFontSize*strlen(savedText)/2;
	const int saveY = 215;
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->print(savedText,saveX,saveY);
}

//Makes window decoration and buttons
void GUI::printMainMenu() {  
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
  
	const int controllerY = 135;
	const int xSpacer = 15;
	const int systemY = 60;
	_lcd->setFont(hallfetica_normal);
  
	//Make menu buttons. System and Controller settings
	mainMenuButtons[3] = _buttons.addButton(xSpacer+70,systemY,mainMenuButtonText[0]);
	mainMenuButtons[4] = _buttons.addButton(xSpacer+70,controllerY,mainMenuButtonText[1]);
	
	//Logos
	_lcd->drawBitmap (xSpacer, systemY-18, 64, 64, plant64);
	_lcd->drawBitmap (xSpacer, controllerY-18, 64, 64, settings64);
	//With transparent buttons
	mainMenuButtons[5] = _buttons.addButton(xSpacer, systemY-18, 64, 64, 0);	
	mainMenuButtons[6] = _buttons.addButton(xSpacer, controllerY-18, 64, 64, 0);
}

//Draws main menu into LCD
//_actScreen == 1
void GUI::drawMainMenu() {
	_actScreen = 1;
	_lcd->fillScr(VGA_WHITE);
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
	if (buttonIndex == mainMenuButtons[2]) { drawMainScreen(); }
	//System Settings
	else if ((buttonIndex == mainMenuButtons[3]) || (buttonIndex == mainMenuButtons[5])) { drawSystemSettings(); }
	//Controller Settings
	else if ((buttonIndex == mainMenuButtons[4]) || (buttonIndex == mainMenuButtons[6])) { drawControllerSettings(); }
}

//Prepares window for drawing
void GUI::printSystemSettings() {
  const int xSpacer = 15;
  const int ySpacer = 35;
 
  nightWater = _settings->getNightWatering();
  //waterPumpState = _settings->getManualPump();

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

//Draws entire screen System Settings
//_actScreen == 2
void GUI::drawSystemSettings() {
	_actScreen = 2;
	_lcd->fillScr(VGA_WHITE);
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
    _lcd->print("ON ",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);
  else
    _lcd->print("OFF",xSpacer+bigFontSize*2+bigFontSize*strlen(systemButtonText[4]),ySpacer+bigFontSize*2*4);	  

}

void GUI::processTouchSystem(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == systemButtons[0]) { drawMainMenu(); }
	//Exit
	else if (buttonIndex == systemButtons[2]) { drawMainScreen(); }
	//Watering Cycle  
	else if (buttonIndex == systemButtons[3]) { drawWaterCycle(); } 
	//Sensor Alarms  
	else if (buttonIndex == systemButtons[4]) { drawSensorAlarms(); }
	//Sensor Calibration  
	else if (buttonIndex == systemButtons[5]) { drawSensorCalibration(); }
	//Pump Protection
	else if (buttonIndex == systemButtons[6]) { drawPumpProtection(); }
	//Water at night Toggle 
	else if (buttonIndex == systemButtons[7]) {
		nightWater = !nightWater;
		_settings->setNightWatering(nightWater);
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
}

//Draws entire screen Controller Settings
//_actScreen == 3
void GUI::drawControllerSettings() {
	_actScreen = 3;
	_lcd->fillScr(VGA_WHITE);
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
	if (buttonIndex == controllerButtons[0]) { drawMainMenu(); }
	//Exit
	else if (buttonIndex == controllerButtons[2]) { drawMainScreen(); } 
	//Time & Date  
	else if (buttonIndex == controllerButtons[3]) { drawTimeSettings(); }
	//Sensor polling
	else if (buttonIndex == controllerButtons[4]) { drawSensorPolling(); }
	//SD Card
	else if (buttonIndex == controllerButtons[5]) { drawSDcard(); }
	//Sound toggle
	else if (buttonIndex == controllerButtons[6]) {
		soundActive = !soundActive;
		_settings->setSound(soundActive);
		updateControllerSettings();
	//Serial debugging toggle
	} else if (buttonIndex == controllerButtons[7]) {
		serialActive = !serialActive;
		_settings->setSerialDebug(serialActive);
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

//Draws entire screen Time Settings
//_actScreen == 4
void GUI::drawTimeSettings() {
	_actScreen = 4;
	_lcd->fillScr(VGA_WHITE);
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
		drawControllerSettings();
	//Save
	} else if (buttonIndex == timeButtons[1]) {	
		_sensors->setRTCtime(sysHour, sysMin, sysSec, sysDay, sysMonth, sysYear);
		printSavedButton();
	//Exit
	} else if (buttonIndex == timeButtons[2]) {
		//Go to main screen
		drawMainScreen();
  
	//Hour up
	} else if (buttonIndex == timeButtons[3]) {
		(sysHour >= 23) ? sysHour=0 : sysHour++;
		updateTimeSettings();  
	//Min up		  
	} else if (buttonIndex == timeButtons[4]) {
		(sysMin >= 59) ? sysMin=0 : sysMin++;
		updateTimeSettings(); 	
	//Sec up
	} else if (buttonIndex == timeButtons[5]) {
		(sysSec >= 59) ? sysSec=0 : sysSec++;;
		updateTimeSettings();
	//Hour down
	} else if (buttonIndex == timeButtons[6]) {
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
		(sysDay >= 31) ? sysDay=1 : sysDay++;
		updateTimeSettings();
	//Month up
	} else if (buttonIndex == timeButtons[10]) {
		(sysMonth >= 12) ? sysMonth=1 : sysMonth++;
		updateTimeSettings();
	//Year up
	} else if (buttonIndex == timeButtons[11]) {	
		//TimeAlarms can only handle 1971-2037
		(sysYear >= 2037) ? sysYear=1971 : sysYear++;
		updateTimeSettings();
	//Day down
	} else if (buttonIndex == timeButtons[12]) {
		(sysDay <= 1) ? sysDay=31 : sysDay--;
		updateTimeSettings();
	//Month down
	} else if (buttonIndex == timeButtons[13]) {
		(sysMonth <= 1) ? sysMonth=12 : sysMonth--;
		updateTimeSettings();
	//Year down
	} else if (buttonIndex == timeButtons[14]) {
		(sysYear <= 1971) ? sysYear=2037 : sysYear--;
		updateTimeSettings();
	}
}

void GUI::printSensorPolling() {
  const int yFirstLine = 60;
  const int ySecondLine = 135;
  const int xSpacer = 25;
  const int xSpacer2 = 72+3*bigFontSize;
  
  const int secU[] = {xSpacer2+bigFontSize/2, ySecondLine-22};       //sec up
  const int secD[] = {xSpacer2+bigFontSize/2, ySecondLine+22};       //sec down
  
  pollSec = _settings->getSensorSecond();
  
  _lcd->setColor(grey[0],grey[1],grey[2]);  
  _lcd->setBackColor(VGA_WHITE);
  _lcd->setFont(hallfetica_normal);
  
  char* sensorPollingText1 = "Time between readings:";
  char* sensorPollingText2 = "seconds";
  
  //Time between readings text
  _lcd->print(sensorPollingText1, xSpacer, yFirstLine);
  //XX
  _lcd->printNumI(pollSec,xSpacer2,ySecondLine,2,'0');
  //secs
  _lcd->print(sensorPollingText2,xSpacer2+3*bigFontSize,ySecondLine);
  
  //Make +/- buttons
  sensorPollingButtons[3] = _buttons.addButton(secU[0],secU[1],sensorPollingButtonText[0],BUTTON_SYMBOL);
  sensorPollingButtons[4] = _buttons.addButton(secD[0],secD[1],sensorPollingButtonText[1],BUTTON_SYMBOL);
 
}

//Draws entire screen Sensor Polling
//_actScreen == 5
void GUI::drawSensorPolling() {
	_actScreen = 5;
	_lcd->fillScr(VGA_WHITE);
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
    const int  xSpacer2 = 72+3*bigFontSize;
	
	_lcd->setFont(hallfetica_normal);
	_lcd->printNumI(pollSec,xSpacer2,ySecondLine,2,'0');
}

void GUI::processTouchSensorPolling(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorPollingButtons[0]) {
		//Go to controller menu
		drawControllerSettings();
	//Save
	} else if (buttonIndex == sensorPollingButtons[1]) {
		_settings->setSensorSecond(pollSec);
		printSavedButton();
	//Exit
	} else if (buttonIndex == sensorPollingButtons[2]) {
		//Go to main screen
		drawMainScreen();
  
	//Sec up
	} else if (buttonIndex == sensorPollingButtons[3]) {
		(pollSec >= 59) ? pollSec=1 : pollSec++;
		updateSensorPolling();
	//Sec down
	} else if (buttonIndex == sensorPollingButtons[4]) {
		(pollSec <= 1) ? pollSec=59 : pollSec--;
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
  
  //If first toggle is inactive we grey out buttons
  if (!sdActive) {
	  for (int i = 4; i < nSDcardButtons; i++)
	  _buttons.disableButton(sdCardButtons[i],true);
	  } else {
	  for (int i = 4; i < nSDcardButtons; i++)
	  _buttons.enableButton(sdCardButtons[i],true);
  } 
}

//Draws entire screen SD Card
//_actScreen == 6
void GUI::drawSDcard() {
	_actScreen = 6;
	_lcd->fillScr(VGA_WHITE);
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
  
  //If first toggle is inactive we grey out buttons
  if (!sdActive) {
	for (int i = 4; i < nSDcardButtons; i++)
	  _buttons.disableButton(sdCardButtons[i],true);
  } else {
	for (int i = 4; i < nSDcardButtons; i++)
	  _buttons.enableButton(sdCardButtons[i],true);
  }
}

void GUI::processTouchSDcard(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sdCardButtons[0]) {
		//Go to controller menu
		drawControllerSettings();
	//Save
	} else if (buttonIndex == sdCardButtons[1]) {
		_settings->setSDactive(sdActive);
		_settings->setSDhour(sdHour);
		_settings->setSDminute(sdMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == sdCardButtons[2]) {
		//Go to main screen
		drawMainScreen();
  
	//Card active toggle
	} else if (buttonIndex == sdCardButtons[3]) {
		sdActive = !sdActive;
		updateSDcard();
	//Hour up
	} else if (buttonIndex == sdCardButtons[4]) {
		(sdHour >= 23) ? sdHour=0 : sdHour++;
		updateSDcard();
	//Minute up
	} else if (buttonIndex == sdCardButtons[5]) {
		(sdMin >= 59) ? sdMin=0 : sdMin++;
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
  
  //If first toggle is inactive we grey out buttons
  if (!waterTimed) {
	for (int i = 4; i < nWaterCycleButtons; i++)
	  _buttons.disableButton(waterCycleButtons[i],true);
  } else {
	for (int i = 4; i < nWaterCycleButtons; i++)
	  _buttons.enableButton(waterCycleButtons[i],true);
  }

}

//Draws entire screen Watering Cycle
//_actScreen == 7
void GUI::drawWaterCycle() {
	_actScreen = 7;
	_lcd->fillScr(VGA_WHITE);
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
	
    //If first toggle is inactive we grey out buttons
    if (!waterTimed) {
	  for (int i = 4; i < nWaterCycleButtons; i++) 
	    _buttons.disableButton(waterCycleButtons[i],true);
	} else {
	  for (int i = 4; i < nWaterCycleButtons; i++) 
		_buttons.enableButton(waterCycleButtons[i],true);
    }
}	

void GUI::processTouchWaterCycle(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == waterCycleButtons[0]) {
		//Go to system menu
		drawSystemSettings();
	//Save
	} else if (buttonIndex == waterCycleButtons[1]) {
		boolean update = false;
		//Prevents flood time > time inactive as it will mess up alarms
		//As flood time always > 1 this also prevents a watering time of 0
		if ((waterHour == 0) && (floodMin >= waterMin)) {
			waterMin = floodMin + 1;
			update = true;
		}
		_settings->setWaterTimed(waterTimed);
		_settings->setWaterHour(waterHour);
		_settings->setWaterMinute(waterMin);	
		_settings->setFloodMinute(floodMin);
		if (update)
			updateWaterCycle();
		printSavedButton();
	//Exit
	} else if (buttonIndex == waterCycleButtons[2]) {
		//Go to main screen
		drawMainScreen();
  
	//Water mode
	} else if (buttonIndex == waterCycleButtons[3]) {
		waterTimed = !waterTimed;
		updateWaterCycle();
	//Hour up  
	} else if (buttonIndex == waterCycleButtons[4]) {
		(waterHour >= 23) ? waterHour=0 : waterHour++;
		updateWaterCycle();
	//Hour down
	} else if (buttonIndex == waterCycleButtons[5]) {
		(waterHour <= 0) ? waterHour=23 : waterHour--;
		updateWaterCycle();
	//Minute up
	} else if (buttonIndex == waterCycleButtons[6]) {
		(waterMin >= 59) ? waterMin=0 : waterMin++;;
		updateWaterCycle();
	//Minute down
	} else if (buttonIndex == waterCycleButtons[7]) {
		(waterMin <= 0) ? waterMin=59 : waterMin--;
		updateWaterCycle();
	//Flood minute up
	} else if (buttonIndex == waterCycleButtons[8]) {	  
		(floodMin >= 59) ? floodMin=1 : floodMin++;
		updateWaterCycle();
	//Flood minute down
	} else if (buttonIndex == waterCycleButtons[9]) {
		(floodMin <= 1) ? floodMin=59 : floodMin--;
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
    _lcd->print("T",xSpacer,ySpacer+bigFontSize*3*i);
  }
  
  //Make menu buttons
  //Before 3 there are flow buttons
  for (int i = 0; i < nSensorAlarmsButtons - 3; i++) {
    sensorAlarmsButtons[i + 3] = _buttons.addButton(xSpacer+bigFontSize*2,ySpacer+bigFontSize*3*i,sensorAlarmsButtonsText[i]);
  }  
}

//Draws entire screen Sensor Alarms
//_actScreen == 8
void GUI::drawSensorAlarms() {
	_actScreen = 8;
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Sensor Alarms -");
	printFlowButtons(true,false,true,sensorAlarmsButtons);
	printSensorAlarms();  
	_buttons.drawButtons();
}

void GUI::processTouchSensorAlarms(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorAlarmsButtons[0]) { drawSystemSettings(); }
	//Exit
	else if (buttonIndex == sensorAlarmsButtons[2]) { drawMainScreen();  }
	//Auto config
	//else if (buttonIndex == sensorAlarmsButtons[3]) { drawAutoConfig(); }
	//pH Thresholds
	else if (buttonIndex == sensorAlarmsButtons[3]) { drawPHalarms(); }
	//EC Thresholds
	else if (buttonIndex == sensorAlarmsButtons[4]) { drawECalarms(); }
	//Nutrient Level
	else if (buttonIndex == sensorAlarmsButtons[5]) { drawWaterAlarms(); }

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

//Draws entire screen pH Alarms
//_actScreen == 9
void GUI::drawPHalarms() {
	_actScreen = 9;
	_lcd->fillScr(VGA_WHITE);
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
		drawSensorAlarms();
	//Save
	} else if (buttonIndex == phAlarmsButtons[1]) {
		_settings->setPHalarmUp(phAlarmMax);
		_settings->setPHalarmDown(phAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == phAlarmsButtons[2]) {
		//Go to main screen
		drawMainScreen();
	
	//Max up
	} else if(buttonIndex == phAlarmsButtons[3]) {	  
		(phAlarmMax >= 14) ? phAlarmMax=0 : phAlarmMax += 0.05;
		updatePHalarms();
	//Max down	  
	} else if(buttonIndex == phAlarmsButtons[4]) {
		(phAlarmMax <= 0) ? phAlarmMax=14 : phAlarmMax -= 0.05;
		updatePHalarms();
	//Min up
	} else if(buttonIndex == phAlarmsButtons[5]) {
		(phAlarmMin >= 14) ? phAlarmMin=0 : phAlarmMin += 0.05;
		updatePHalarms();
	//Min down
	} else if(buttonIndex == phAlarmsButtons[6]) {
		(phAlarmMin <= 0) ? phAlarmMin=14 : phAlarmMin -= 0.05;
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

//Draws entire screen EC alarms
//_actScreen == 10
void GUI::drawECalarms() {
	_actScreen = 10;
	_lcd->fillScr(VGA_WHITE);
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
		drawSensorAlarms();
	//Save
	} else if (buttonIndex == ecAlarmsButtons[1]) {
		_settings->setECalarmUp(ecAlarmMax);
		_settings->setECalarmDown(ecAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == ecAlarmsButtons[2]) {
		//Go to main screen
		drawMainScreen();
	
	//Max up	
	} else if(buttonIndex == ecAlarmsButtons[3]) {
		(ecAlarmMax >= 9990) ? ecAlarmMax=0 : ecAlarmMax += 10;
		updateECalarms();
	//Max down
	} else if(buttonIndex == ecAlarmsButtons[4]) {
		(ecAlarmMax <= 0) ? ecAlarmMax=9990 : ecAlarmMax -= 10;
		updateECalarms();
	//Min up
	} else if(buttonIndex == ecAlarmsButtons[5]) {
		(ecAlarmMin >= 9990) ? ecAlarmMin=0 : ecAlarmMin += 10;
		updateECalarms();
	//Min down
	} else if(buttonIndex == ecAlarmsButtons[6]) {
		(ecAlarmMin <= 0) ? ecAlarmMin=9990 : 	ecAlarmMin -= 10;
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
  _lcd->printNumI(waterAlarmMin,x,yFirstLine,3);
  //Buttons
  x += 1.5*bigFontSize;
  waterAlarmsButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,waterAlarmsButtonsText[0],BUTTON_SYMBOL);
  waterAlarmsButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,waterAlarmsButtonsText[1],BUTTON_SYMBOL);
  //percent sign
  x += 2.5*bigFontSize;
  _lcd->print("%",x,yFirstLine);
}

//Draws entire screen Nutrient level alarms
//_actScreen == 11
void GUI::drawWaterAlarms() {
	_actScreen = 11;
	_lcd->fillScr(VGA_WHITE);
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
	_lcd->printNumI(waterAlarmMin,x,yFirstLine,3);
}

void GUI::processTouchWaterAlarms(int x,int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == waterAlarmsButtons[0]) {
		//Go to alarms menu
		drawSensorAlarms();
	//Save
	} else if (buttonIndex == waterAlarmsButtons[1]) {
		_settings->setWaterAlarm(waterAlarmMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == waterAlarmsButtons[2]) {
		//Go to main screen
		drawMainScreen();
  
	//Up
	} else if (buttonIndex == waterAlarmsButtons[3]) {
		(waterAlarmMin >= 100) ? waterAlarmMin=0 : waterAlarmMin++;
		updateWaterAlarms();
	//Down  
	} else if (buttonIndex == waterAlarmsButtons[4]) {
		(waterAlarmMin <= 0) ? waterAlarmMin=100 : waterAlarmMin--;
		updateWaterAlarms();
	} 
}

/*void GUI::printAutoConfig() {
  //TODO
}

//Draws entire screen Auto Config Alarms
//_actScreen == 12
void GUI::drawAutoConfig() {
	_actScreen = 12;
	_lcd->fillScr(VGA_WHITE);
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
		drawSensorAlarms();
	//Save
	} else if (buttonIndex == autoConfigButtons[1]) {
		//TODO: Do something!
		printSavedButton();
	//Exit
	} else if (buttonIndex == autoConfigButtons[2]) {
		//Go to main screen
		drawMainScreen();
	}    
}*/
/*
void GUI::printSensorCalibration() {
  const int xSpacer = 15;
  const int ySpacer = 45;
  
  	const int yFirst = 60;
	const int ySecond = 135;
	_lcd->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
	_lcd->setBackColor(VGA_WHITE);
  
	//Triangles
	_lcd->setFont(various_symbols);
	_lcd->print("T",xSpacer,yFirst);
	_lcd->print("T",xSpacer,ySecond);
	//Buttons
	sensorCalibrationButtons[3] = _buttons.addButton(xSpacer+bigFontSize*2,yFirst,sensorCalibrationButtonsText[0]);
	sensorCalibrationButtons[4] = _buttons.addButton(xSpacer+bigFontSize*2,ySecond,sensorCalibrationButtonsText[1]);
}

//Draws entire screen Sensor Calibration
//_actScreen == 13
void GUI::drawSensorCalibration() {
	_actScreen = 13;
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Calibration -");
	printFlowButtons(true,false,true,sensorCalibrationButtons);
	printSensorCalibration();  
	_buttons.drawButtons();   
}

void GUI::processTouchSensorCalibration(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == sensorCalibrationButtons[0]) { drawSystemSettings(); }
	//Exit
	else if (buttonIndex == sensorCalibrationButtons[2]) { drawMainScreen(); }  
	//Water calibration
	else if (buttonIndex == sensorCalibrationButtons[3]) { drawWaterCalibration(); }
	//Light Calibration
	else if (buttonIndex == sensorCalibrationButtons[4]) { drawLightCalibration(); }
}

void GUI::printWaterCalibration() {
    const int yFirstLine = 50;
    const int ySecondLine = 100;
    const int yThirdLine = 150;
    const int xSpacer = 25;
	
	rawWaterLvl = _sensors->getRawWaterLevel();
	waterLvlMax = _settings->getMaxWaterLvl();
	waterLvlMin = _settings->getMinWaterLvl();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer;
	_lcd->print("Current Reading:",x,yFirstLine);
	x += 16*bigFontSize;
	_lcd->printNumI(rawWaterLvl,x,yFirstLine,3,' ');
	x +=3*bigFontSize;
	_lcd->print("cm",x,yFirstLine);
	
	//Second Line
	x = xSpacer;
	_lcd->print("Current Top:",x,ySecondLine);
	x += 12*bigFontSize;
	_lcd->printNumI(waterLvlMax,x,ySecondLine,3,' ');
	x += 3*bigFontSize;
	_lcd->print("cm",x,ySecondLine);
	x += 3*bigFontSize;
	waterLevelButtons[3] = _buttons.addButton(x,ySecondLine,waterLevelButtonsText[0]);
	
	//Third Line
	x = xSpacer;
	_lcd->print(" and Bottom:",x,yThirdLine);
	x += 12*bigFontSize;
	_lcd->printNumI(waterLvlMin,x,yThirdLine,3,' ');
	x += 3*bigFontSize;
	_lcd->print("cm",x,yThirdLine);
	x += 3*bigFontSize;
	waterLevelButtons[4] = _buttons.addButton(x,yThirdLine,waterLevelButtonsText[1]);
}

void GUI::updateWaterCalibration() {
	const int yFirstLine = 50;
	const int ySecondLine = 100;
	const int yThirdLine = 150;
	const int xSpacer = 25;
	
	rawWaterLvl = _sensors->getRawWaterLevel();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer + 16*bigFontSize;
	_lcd->printNumI(rawWaterLvl,x,yFirstLine,3,' ');
	    
	//Second Line
	x = xSpacer + 12*bigFontSize;
	_lcd->printNumI(waterLvlMax,x,ySecondLine,3,' ');
	    
	//Third Line
	x = xSpacer + 12*bigFontSize;
	_lcd->printNumI(waterLvlMin,x,yThirdLine,3,' ');	
}

//Draws entire screen Water Level Calibration
//_actScreen == 14
void GUI::drawWaterCalibration() {
	_actScreen = 14;
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Nutrient Levels -");
	printFlowButtons(true,true,true,waterLevelButtons);
	printWaterCalibration();  
	_buttons.drawButtons();   
}

void GUI::processTouchWaterCalibration(int x,int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == waterLevelButtons[0]) {
		//Go to calibration menu
		drawSensorCalibration();
	//Save
	} else if (buttonIndex == waterLevelButtons[1]) {
		_settings->setMaxWaterLvl(waterLvlMax);
		_settings->setMinWaterLvl(waterLvlMin);
		printSavedButton();
	//Exit
	} else if (buttonIndex == waterLevelButtons[2]) {
		//Go to main screen
		drawMainScreen();
		
	} else if (buttonIndex == waterLevelButtons[3]) {
		waterLvlMax = rawWaterLvl;
		updateWaterCalibration();
	} else if (buttonIndex == waterLevelButtons[4]) {
		waterLvlMin = rawWaterLvl;
		updateWaterCalibration();
	}
}

void GUI::printLightCalibration() {
    const int yFirstLine = 60;
    const int ySecondLine = 135;
    const int xSpacer = 25;
	
    rawLightLvl = _sensors->getRawLight();
    lightThreshold = _settings->getLightThreshold();
    
    _lcd->setColor(grey[0],grey[1],grey[2]);
    _lcd->setFont(hallfetica_normal);
    
    //First Line
    int x = xSpacer;
    _lcd->print("Current Reading:",x,yFirstLine);
    x += 16*bigFontSize;
    _lcd->printNumI(rawLightLvl,x,yFirstLine,4,' ');
    //x +=4*bigFontSize;
    //_lcd->print("lux",x,yFirstLine);
    
    //Second Line
    x = xSpacer;
    _lcd->print("Threshold:",x,ySecondLine);
    x += 10*bigFontSize;
    _lcd->printNumI(lightThreshold,x,ySecondLine,4,' ');
    //x += 4*bigFontSize;
    //_lcd->print("lux",x,ySecondLine);
    x += 5*bigFontSize;
    lightCalibrationButtons[3] = _buttons.addButton(x,ySecondLine,lightCalibrationButtonsText[0]);
}

void GUI::updateLightCalibration() {
	const int yFirstLine = 60;
	const int ySecondLine = 135;
	const int xSpacer = 25;
	
	rawLightLvl = _sensors->getRawLight();
	
	_lcd->setColor(grey[0],grey[1],grey[2]);
	_lcd->setFont(hallfetica_normal);
	
	//First Line
	int x = xSpacer + 16*bigFontSize;
	_lcd->printNumI(rawLightLvl,x,yFirstLine,4,' ');
	
	//Second Line
	x = xSpacer + 10*bigFontSize;
	_lcd->printNumI(lightThreshold,x,ySecondLine,4,' ');	
}

//Draws entire screen Light Calibration
//_actScreen == 15
void GUI::drawLightCalibration() {
	_actScreen = 15;
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Night Calibration -");
	printFlowButtons(true,true,true,lightCalibrationButtons);
	printLightCalibration();  
	_buttons.drawButtons();    
}

void GUI::processTouchLightCalibration(int x, int y) {
    int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == lightCalibrationButtons[0]) {
		//Go to calibration menu
		drawSensorCalibration();
	//Save
	} else if (buttonIndex == lightCalibrationButtons[1]) {
		_settings->setLightThreshold(lightThreshold);
		printSavedButton();
	//Exit
	} else if (buttonIndex == lightCalibrationButtons[2]) {
		//Go to main screen
		drawMainScreen();
	} else if (buttonIndex == lightCalibrationButtons[3]) {
		lightThreshold = rawLightLvl;
		updateLightCalibration();
	}
}

void GUI::printPumpProtection() {
  const int yFirstLine = 100;
  const int xSpacer = 25;
  const int signSpacer = 22;
  
  pumpProtectionLvl = _settings->getPumpProtectionLvl();
  
  char* wLimitS = "Min Water Lvl:";
  _lcd->setColor(grey[0],grey[1],grey[2]);
  //Text
  _lcd->print(wLimitS,xSpacer,yFirstLine);
  //Numbers
  int x = (4+strlen(wLimitS))*bigFontSize;
  _lcd->printNumI(pumpProtectionLvl,x,yFirstLine,3);
  //Buttons
  x += 1.5*bigFontSize;
  pumpProtectionButtons[3] = _buttons.addButton(x,yFirstLine-signSpacer,pumpProtectionButtonsText[0],BUTTON_SYMBOL);
  pumpProtectionButtons[4] = _buttons.addButton(x,yFirstLine+signSpacer,pumpProtectionButtonsText[1],BUTTON_SYMBOL);
  //percent sign
  x += 2.5*bigFontSize;
  _lcd->print("%",x,yFirstLine);
}

//Draws entire screen Pump Protection
//_actScreen == 16
void GUI::drawPumpProtection() {
	_actScreen = 16;
	_lcd->fillScr(VGA_WHITE);
	_buttons.deleteAllButtons();
	printMenuHeader("- Pump Protection -");
	printFlowButtons(true,true,true,pumpProtectionButtons);
	printPumpProtection();
	_buttons.drawButtons();
}

void GUI::updatePumpProtection() {
	const int yFirstLine = 100;
	char* wLimitS = "Lower Limit:";
	
	_lcd->setFont(hallfetica_normal);
	int x = (4+strlen(wLimitS))*bigFontSize;
	_lcd->printNumI(pumpProtectionLvl,x,yFirstLine,3);
}

void GUI::processTouchPumpProtection(int x, int y) {
	int buttonIndex = _buttons.checkButtons(x,y);
	//Back
	if (buttonIndex == pumpProtectionButtons[0]) {
		//Go to system menu
		drawSystemSettings();
	//Save
	} else if (buttonIndex == pumpProtectionButtons[1]) {
		_settings->setPumpProtectionLvl(pumpProtectionLvl);
		printSavedButton();
	//Exit
	} else if (buttonIndex == pumpProtectionButtons[2]) {
		//Go to main screen
		drawMainScreen();
		
	//Up
	} else if (buttonIndex == pumpProtectionButtons[3]) {
		(pumpProtectionLvl >= 100) ? pumpProtectionLvl=0 : pumpProtectionLvl++;
		updatePumpProtection();
	//Down
	} else if (buttonIndex == pumpProtectionButtons[4]) {
		(pumpProtectionLvl <= 0) ? pumpProtectionLvl=100 : pumpProtectionLvl--;
		updatePumpProtection();
	}
}
*/