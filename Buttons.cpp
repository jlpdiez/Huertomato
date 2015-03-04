#include "Buttons.h"

//Constructors
WinButtons::WinButtons(UTFT *lcd, UTouch *touch) : _lcd(lcd), _touch(touch) {
	deleteAllButtons();
	for (uint8_t i = 0; i < 3; i++) {
		_color_text[i] = 255;
		_color_text_inactive[i] = 150;
		_color_background[i] = 0;
		_color_hilite[i] = 150;
	}
	_font_text = NULL;
	_font_symbol = NULL;
}
		
WinButtons::WinButtons(const WinButtons &other) {
	_lcd = other._lcd;
	_touch = other._touch;
	for (uint8_t i = 0; i < 3; i++) {
		_color_text[i] = other._color_text[i];
		_color_text_inactive[i] = other._color_text_inactive[i];
		_color_background[i] = other._color_background[i];
		_color_hilite[i] = other._color_hilite[i];
	}
}
 
WinButtons& WinButtons::operator=(const WinButtons &other) {
	//Should call operator= of the superclass but its not available
	_lcd = other._lcd;
	_touch = other._touch;
	for (uint8_t i = 0; i < 3; i++) {
		_color_text[i] = other._color_text[i];
		_color_text_inactive[i] = other._color_text_inactive[i];
		_color_background[i] = other._color_background[i];
		_color_hilite[i] = other._color_hilite[i];
	}		
	
	return *this;	
}

//Destructor
WinButtons::~WinButtons() {}

//Iterates through button list and draws them into screen
void WinButtons::drawButtons() {
  for (uint8_t i = 0; i < _maxButtons; i++) {
    if ((buttons[i].flags & BUTTON_UNUSED) == 0)
      drawButton(i);
  }
}

//Add button to list. In this version width and height are auto-calculated
int8_t WinButtons::addButton(uint16_t x, uint16_t y, const char *label, uint16_t flags) {
  int8_t btcnt = 0;
  
  while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt<_maxButtons))
    btcnt++;
  
  if (btcnt == _maxButtons)
    return -1;
  else {
    buttons[btcnt].pos_x  = x;
    buttons[btcnt].pos_y  = y;
	//We add a margin of 10 to all buttons in order fr them to be easier to press
    buttons[btcnt].width  = _lcd->getFontXsize() * strlen_P(label) + 10;
    buttons[btcnt].height = _lcd->getFontYsize() + 10;
    buttons[btcnt].flags  = flags;
    buttons[btcnt].label  = label;
    buttons[btcnt].data   = NULL;

    return btcnt;
  }
}

//Adds button
int8_t WinButtons::addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags) {
  int8_t btcnt = 0;
  
  while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt<_maxButtons))
    btcnt++;
  
  if (btcnt == _maxButtons)
    return -1;
  else {
    buttons[btcnt].pos_x  = x;
    buttons[btcnt].pos_y  = y;
    buttons[btcnt].width  = width;
    buttons[btcnt].height = height;
    buttons[btcnt].flags  = flags | BUTTON_BITMAP;
    buttons[btcnt].label  = NULL;
    buttons[btcnt].data   = data;
    
    return btcnt;
  }
}

//Draws a button with its ID. Its called from drawButtons()
void WinButtons::drawButton(int buttonID) {
  uint8_t *_font_current = _lcd->getFont();
  word _current_color = _lcd->getColor();
  word _current_back = _lcd->getBackColor();
  
  _lcd->setBackColor(_color_background[0], _color_background[1], _color_background[2]);
  if (buttons[buttonID].flags & BUTTON_BITMAP) {
		if (buttons[buttonID].data != 0)
			_lcd->drawBitmap(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, buttons[buttonID].data);
		if ((buttons[buttonID].flags & BUTTON_DISABLED))
			_lcd->setColor(_color_text_inactive[0],_color_text_inactive[1],_color_text_inactive[2]);
  } else {
    if (buttons[buttonID].flags & BUTTON_DISABLED)
      _lcd->setColor(_color_text_inactive[0],_color_text_inactive[1],_color_text_inactive[2]);
    else
      _lcd->setColor(_color_text[0],_color_text[1],_color_text[2]);
    if (buttons[buttonID].flags & BUTTON_SYMBOL) {
      _lcd->setFont(_font_symbol);
    } else {
      _lcd->setFont(_font_text);
    }
    _lcd->setBackColor(_color_background[0], _color_background[1], _color_background[2]);
    _lcd->print(pmChar(buttons[buttonID].label), buttons[buttonID].pos_x, buttons[buttonID].pos_y);
    if ((buttons[buttonID].flags & BUTTON_SYMBOL) and (buttons[buttonID].flags & BUTTON_SYMBOL_REP_3X)) {
      _lcd->print(pmChar(buttons[buttonID].label), buttons[buttonID].pos_x, buttons[buttonID].pos_y);
    }
  }
  _lcd->setFont(_font_current);
  _lcd->setColor(_current_color);
  _lcd->setBackColor(_current_back);
}

//Enables button
void WinButtons::enableButton(int buttonID, boolean redraw) {
  if (!(buttons[buttonID].flags & BUTTON_UNUSED)) {
    buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_DISABLED;
    if (redraw)
      drawButton(buttonID);
  }
}

//Disables button
void WinButtons::disableButton(int buttonID, boolean redraw) {
  if (!(buttons[buttonID].flags & BUTTON_UNUSED)) {
    buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_DISABLED;
    if (redraw)
      drawButton(buttonID);
  }
}

//Relabels a button
void WinButtons::relabelButton(int buttonID, char *label, boolean redraw) {
  if (!(buttons[buttonID].flags & BUTTON_UNUSED)) {
	buttons[buttonID].label = label;
    if (redraw)
      drawButton(buttonID);
  }
}

boolean WinButtons::buttonEnabled(int buttonID) {
	return !(buttons[buttonID].flags & BUTTON_DISABLED);
}

void WinButtons::deleteButton(int buttonID) {
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
		buttons[buttonID].flags = BUTTON_UNUSED;
}

void WinButtons::deleteAllButtons() {
	for (uint8_t i=0;i<_maxButtons;i++)	{
		buttons[i].pos_x = 0;
		buttons[i].pos_y = 0;
		buttons[i].width = 0;
		buttons[i].height = 0;
		buttons[i].flags = BUTTON_UNUSED;
		buttons[i].label = NULL;
	}
}

//Given x,y coords of a touch, returns ID of which button is pressed
int8_t WinButtons::checkButtons(int touch_x, int touch_y) {
    int8_t	result = -1;
    word _current_color = _lcd->getColor();
    
    //We check what buttonID is pressed
    for (uint8_t i=0;i<_maxButtons;i++) {
      if (((buttons[i].flags & BUTTON_UNUSED) == 0) && ((buttons[i].flags & BUTTON_DISABLED) == 0) && (result == -1)) {
        if ((touch_x >= buttons[i].pos_x) && (touch_x <= (buttons[i].pos_x + buttons[i].width)) && (touch_y >= buttons[i].pos_y) && (touch_y <= (buttons[i].pos_y + buttons[i].height)))
          result = i;
      }
    }
	//While pressed
    if (result != -1) {
      //if (!(buttons[result].flags & BUTTON_NO_BORDER)) {
        _lcd->setColor(_color_hilite[0], _color_hilite[1], _color_hilite[2]);
        if (buttons[result].flags & BUTTON_SYMBOL)
          _lcd->setFont(_font_symbol);
        else
          _lcd->setFont(_font_text);
		if (!(buttons[result].flags & BUTTON_BITMAP))
			_lcd->print(pmChar(buttons[result].label), buttons[result].pos_x, buttons[result].pos_y);
    }
    
	//The following line prevents multi-presses
    while (_touch->dataAvailable() == true) {};
		
	//On release
    if (result != -1) {
		_lcd->setColor(_color_text[0],_color_text[1],_color_text[2]);
		if (buttons[result].flags & BUTTON_SYMBOL)
          _lcd->setFont(_font_symbol);
        else
          _lcd->setFont(_font_text);
		if (!(buttons[result].flags & BUTTON_BITMAP))
			_lcd->print(pmChar(buttons[result].label), buttons[result].pos_x, buttons[result].pos_y);
    }

    _lcd->setColor(_current_color);
    return result;
}

void WinButtons::setTextFont(uint8_t* font) {
	_font_text = font;
}

void WinButtons::setSymbolFont(uint8_t* font) {
	_font_symbol = font;
}

//Sets colours for buttons
void WinButtons::setButtonColors(const uint8_t atxt[3], const uint8_t iatxt[3], const uint8_t brdhi[3], const uint8_t back[3]) {
  for (uint8_t i = 0; i < 3; i++) {	
    _color_text[i] = atxt[i];
    _color_text_inactive[i] = iatxt[i];
    _color_hilite[i] = brdhi[i];
    _color_background[i] = back[i];
  }
}

//Converts a char array from PROGMEM to variable in SRAM
char* WinButtons::pmChar(const char *pmArray) {
	strcpy_P(_stringBuffer, (char*)pmArray);
	return _stringBuffer;
}