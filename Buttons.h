// #############################################################################
//
// # Name       : Borderless_Buttons
// # Version    : 1.2
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 13.01.2015
//
// # Description: Makes buttons with no border. 
//   The label field is treated as a char pointer to PROGMEM
//	 Based on the UTFT_Buttons library by Henning Karlsen: http://electronics.henningkarlsen.com/
//
// # This library is free software; you can redistribute it and/or
//   modify it under the terms of the CC BY-NC-SA 3.0 license.
//   Please see the included documents for further information.
//
// #############################################################################

#ifndef BORDERLESS_BUTTONS_H
#define BORDERLESS_BUTTONS_H

#include <Arduino.h>
#include <UTFT.h>
#include <UTouch.h>

// Define presets for button status
#define BUTTON_DISABLED			0x0001
#define BUTTON_SYMBOL			0x0002
#define BUTTON_SYMBOL_REP_3X	0x0004
#define BUTTON_BITMAP			0x0008
#define BUTTON_NO_BORDER		0x0010
#define BUTTON_UNUSED			0x8000

typedef struct {
	uint16_t pos_x, pos_y, width, height;
	uint16_t flags;
	//Stores a pointer to PROGMEM
	const char* label;
	bitmapdatatype	data;
} button_struct;

//Draws a borderless button where colors are used as follows:
//text: used for button text and symbols
//inactive: used for button text and symbols on disabled buttons
//highlight: used for button text and symbols when selected
//background: used for button background
class Borderless_Buttons {
	public:
		//Constructors
		Borderless_Buttons(UTFT *ptrUTFT, UTouch *ptrUTouch);
		Borderless_Buttons(const Borderless_Buttons &other);
		Borderless_Buttons& operator=(const Borderless_Buttons &other);
		//Destructor
		~Borderless_Buttons();
	    
		int8_t	addButton(uint16_t x, uint16_t y, const char* label, uint16_t flags=0);
		int8_t addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags=0);
		void drawButtons();
		void drawButton(int buttonID);
		void enableButton(int buttonID, boolean redraw=false);
		void disableButton(int buttonID, boolean redraw=false);
		void relabelButton(int buttonID, char *label, boolean redraw=false);
		boolean	buttonEnabled(int buttonID); 
		void deleteButton(int buttonID); 
		void deleteAllButtons(); 
		int8_t checkButtons(int touch_x, int touch_y);
		void setTextFont(uint8_t* font);
		void setSymbolFont(uint8_t* font);
		void setButtonColors(const uint8_t atxt[3], const uint8_t iatxt[3], const uint8_t brdhi[3], const uint8_t back[3]);
    
	protected:
		UTFT *_lcd;
		UTouch *_touch;
		static const uint8_t _maxButtons = 15;
		button_struct buttons[_maxButtons];
		uint8_t _color_text[3], _color_text_inactive[3], _color_background[3], _color_hilite[3];
		uint8_t	*_font_text, *_font_symbol;
		//These are used to read data from PROGMEM and store them into SRAM
		char _stringBuffer[30];
		char* pmChar(const char* pmArray);
};

#endif

