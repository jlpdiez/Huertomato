#include "Buttons.h"

//Constructors
Borderless_Buttons::Borderless_Buttons(UTFT *ptrUTFT, UTouch *ptrUTouch) 
	: UTFT_Buttons(ptrUTFT,ptrUTouch) {}
		
Borderless_Buttons::Borderless_Buttons(const Borderless_Buttons &other)
	: UTFT_Buttons(other) {
		
	for (int i = 0; i < 3; i++) {
		_color_text[i] = other._color_text[i];
		_color_text_inactive[i] = other._color_text_inactive[i];
		_color_background[i] = other._color_background[i];
		_color_border[i] = other._color_border[i];
		_color_hilite[i] = other._color_hilite[i];
	}
}

//Not sure if it works correctly
Borderless_Buttons& Borderless_Buttons::operator=(const Borderless_Buttons &other) {
	//Should call operator= of the superclass but its not available
	_UTFT = other._UTFT;
	_UTouch = other._UTouch;
	for (int i = 0; i < 3; i++) {
		_color_text[i] = other._color_text[i];
		_color_text_inactive[i] = other._color_text_inactive[i];
		_color_background[i] = other._color_background[i];
		_color_border[i] = other._color_border[i];
		_color_hilite[i] = other._color_hilite[i];
	}		
	
	return *this;	
}

//Destructor
Borderless_Buttons::~Borderless_Buttons() {}

//Iterates through button list and draws them into screen
void Borderless_Buttons::drawButtons() {
  for (int i = 0; i < MAX_BUTTONS; i++) {
    if ((buttons[i].flags & BUTTON_UNUSED) == 0)
      drawButton(i);
  }
}

//Add button to list. In this version width and height are auto-calculated
int Borderless_Buttons::addButton(uint16_t x, uint16_t y, char *label, uint16_t flags) {
  int btcnt = 0;
  
  while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt<MAX_BUTTONS))
    btcnt++;
  
  if (btcnt == MAX_BUTTONS)
    return -1;
  else {
    buttons[btcnt].pos_x  = x;
    buttons[btcnt].pos_y  = y;
	//We add a margin of 10 to all buttons in order fr them to be easier to press
    buttons[btcnt].width  = _UTFT->getFontXsize() * strlen(label) + 10;
    buttons[btcnt].height = _UTFT->getFontYsize() + 10;
    buttons[btcnt].flags  = flags;
    buttons[btcnt].label  = label;
    buttons[btcnt].data   = NULL;
    //buttons[btcnt].dataOn   = NULL;
    return btcnt;
  }
}

//Adds button
int Borderless_Buttons::addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags) {
  int btcnt = 0;
  
  while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt<MAX_BUTTONS))
    btcnt++;
  
  if (btcnt == MAX_BUTTONS)
    return -1;
  else {
    buttons[btcnt].pos_x  = x;
    buttons[btcnt].pos_y  = y;
    buttons[btcnt].width  = width;
    buttons[btcnt].height = height;
    buttons[btcnt].flags  = flags | BUTTON_BITMAP;
    buttons[btcnt].label  = NULL;
    buttons[btcnt].data   = data;
    //buttons[btcnt].dataOn   = dataOn;
    
    return btcnt;
  }
}

//Draws a button with its ID. Its called from drawButtons()
void Borderless_Buttons::drawButton(int buttonID) {
  uint8_t *_font_current = _UTFT->getFont();
  word _current_color = _UTFT->getColor();
  word _current_back = _UTFT->getBackColor();
  
  _UTFT->setBackColor(_color_background[0], _color_background[1], _color_background[2]);
  if (buttons[buttonID].flags & BUTTON_BITMAP) {
		if (buttons[buttonID].data != 0)
			_UTFT->drawBitmap(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, buttons[buttonID].data);
		if ((buttons[buttonID].flags & BUTTON_DISABLED))
			_UTFT->setColor(_color_text_inactive[0],_color_text_inactive[1],_color_text_inactive[2]);
  } else {
    if (buttons[buttonID].flags & BUTTON_DISABLED)
      _UTFT->setColor(_color_text_inactive[0],_color_text_inactive[1],_color_text_inactive[2]);
    else
      _UTFT->setColor(_color_text[0],_color_text[1],_color_text[2]);
    if (buttons[buttonID].flags & BUTTON_SYMBOL) {
      _UTFT->setFont(_font_symbol);
    } else {
      _UTFT->setFont(_font_text);
    }
    _UTFT->setBackColor(_color_background[0], _color_background[1], _color_background[2]);
    _UTFT->print(buttons[buttonID].label, buttons[buttonID].pos_x, buttons[buttonID].pos_y);
    if ((buttons[buttonID].flags & BUTTON_SYMBOL) and (buttons[buttonID].flags & BUTTON_SYMBOL_REP_3X)) {
      _UTFT->print(buttons[buttonID].label, buttons[buttonID].pos_x, buttons[buttonID].pos_y);
    }
  }
  _UTFT->setFont(_font_current);
  _UTFT->setColor(_current_color);
  _UTFT->setBackColor(_current_back);
}

//Enables button
void Borderless_Buttons::enableButton(int buttonID, boolean redraw) {
  if (!(buttons[buttonID].flags & BUTTON_UNUSED)) {
    buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_DISABLED;
    if (redraw)
      drawButton(buttonID);
  }
}

//Disables button
void Borderless_Buttons::disableButton(int buttonID, boolean redraw) {
  if (!(buttons[buttonID].flags & BUTTON_UNUSED)) {
    buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_DISABLED;
    if (redraw)
      drawButton(buttonID);
  }
}

//Relabels a button
void Borderless_Buttons::relabelButton(int buttonID, char *label, boolean redraw) {
  if (!(buttons[buttonID].flags & BUTTON_UNUSED)) {
    buttons[buttonID].label = label;
    if (redraw)
      drawButton(buttonID);
  }
}

//Given x,y coords of a touch, returns ID of which button is pressed
int Borderless_Buttons::checkButtons(int touch_x, int touch_y) {
//  if (_UTouch->dataAvailable() == true) {
//    _UTouch->read();
    int	result = -1;
//    int	touch_x = _UTouch->getX();
//    int	touch_y = _UTouch->getY();
    word _current_color = _UTFT->getColor();
    
    //We check what buttonID is pressed
    for (int i=0;i<MAX_BUTTONS;i++) {
      if (((buttons[i].flags & BUTTON_UNUSED) == 0) && ((buttons[i].flags & BUTTON_DISABLED) == 0) && (result == -1)) {
        if ((touch_x >= buttons[i].pos_x) && (touch_x <= (buttons[i].pos_x + buttons[i].width)) && (touch_y >= buttons[i].pos_y) && (touch_y <= (buttons[i].pos_y + buttons[i].height)))
          result = i;
      }
    }
	//While pressed
    if (result != -1) {
      //if (!(buttons[result].flags & BUTTON_NO_BORDER)) {
        _UTFT->setColor(_color_hilite[0], _color_hilite[1], _color_hilite[2]);
        if (buttons[result].flags & BUTTON_SYMBOL)
          _UTFT->setFont(_font_symbol);
        else
          _UTFT->setFont(_font_text);
		if (!(buttons[result].flags & BUTTON_BITMAP))
			_UTFT->print(buttons[result].label, buttons[result].pos_x, buttons[result].pos_y);
    }
    
	//The following line prevents multi-presses
    while (_UTouch->dataAvailable() == true) {};
		
	//On release
    if (result != -1) {
		_UTFT->setColor(_color_text[0],_color_text[1],_color_text[2]);
		if (buttons[result].flags & BUTTON_SYMBOL)
          _UTFT->setFont(_font_symbol);
        else
          _UTFT->setFont(_font_text);
		if (!(buttons[result].flags & BUTTON_BITMAP))
			_UTFT->print(buttons[result].label, buttons[result].pos_x, buttons[result].pos_y);
    }

    _UTFT->setColor(_current_color);
    return result;
//  }
//  else
//    return -1;
}

//Sets colours for buttons
void Borderless_Buttons::setButtonColors(const uint8_t atxt[3], const uint8_t iatxt[3], const uint8_t brd[3], const uint8_t brdhi[3], const uint8_t back[3]) {
  for (int i = 0; i < 3; i++) {	
    _color_text[i] = atxt[i];
    _color_text_inactive[i] = iatxt[i];
    _color_border[i] = brd[i];
    _color_hilite[i] = brdhi[i];
    _color_background[i] = back[i];
  }
}

