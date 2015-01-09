// #############################################################################
//
// # Name       : Borderless_Buttons
// # Version    : 1.1
//
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 07.01.2015
//
// # Description: Makes buttons with no border. There's also a function added to pass Strings as text to them
//
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef BORDERLESS_BUTTONS_H
#define BORDERLESS_BUTTONS_H

#include <Arduino.h>
#include <UTFT_Buttons.h>
#include <string.h>


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
	//int addButton(uint16_t x, uint16_t y, String label, uint16_t flags=0);
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

