#ifndef BORDERLESS_BUTTONS_H
#define BORDERLESS_BUTTONS_H

#include <Arduino.h>
#include <UTFT_Buttons.h>


//Draws a borderless button where colors of UTFT_Buttons are used as follows:
//text: used for button text and symbols
//inactive: used for button text and symbols on disabled buttons
//border: not used
//highlight: used for button text and symbols when selected
//background: used for button background

class Borderless_Buttons : public UTFT_Buttons {
  public:
    //Constructors
    Borderless_Buttons(UTFT *ptrUTFT, UTouch *ptrUTouch);
	Borderless_Buttons(const Borderless_Buttons &other);
	Borderless_Buttons& operator=(const Borderless_Buttons &other);
	//Destructor
	~Borderless_Buttons();
	    
    int	addButton(uint16_t x, uint16_t y, char *label, uint16_t flags=0);
    int addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags=0);
    void drawButtons();
    void drawButton(int buttonID);
    void enableButton(int buttonID, boolean redraw=false);
    void disableButton(int buttonID, boolean redraw=false);
    void relabelButton(int buttonID, char *label, boolean redraw=false);
    int checkButtons(int touch_x, int touch_y);
    void setButtonColors(const uint8_t atxt[3], const uint8_t iatxt[3], const uint8_t brd[3], const uint8_t brdhi[3], const uint8_t back[3]);
    
   protected:
     uint8_t _color_text[3], _color_text_inactive[3], _color_background[3], _color_border[3], _color_hilite[3];
};

#endif

