// #############################################################################
// 
// # Name       : rgbLed
// # Version    : 1.5

// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 11.03.2013
// 
// # Description: Library for controlling an RGB LED, 
// # Allows to set colour, change brightness and do some effects.
// # Constructor needs red, green and blue pins as argument.
// # Keep in mind delays are used in effects functions.
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

#ifndef RGBLED_H
#define RGBLED_H

#include <Arduino.h>

#include <Arduino.h>

enum color { RED, GREEN, BLUE, WHITE };

<<<<<<< HEAD
class rgbLED {
  public:
     rgbLED(uint8_t, uint8_t, uint8_t);
=======
class RGBled {
  public:
	 //Constructors
     RGBled(uint8_t, uint8_t, uint8_t);
	 RGBled(const RGBled &other);
	 RGBled& operator=(const RGBled &other);
	 //Destructor
	 ~RGBled();
	 
>>>>>>> origin/newUI
     void setOn();
     void setOff();
     void setColour(color);
     void setColour(uint8_t, uint8_t, uint8_t);
     void moreBrightness();
     void lessBrightness();
     //Effects - They use delays!
     void strobe();
     void rgbStrobe();
     void smooth();
     //void fade();

  private:
     uint8_t _redPin;
     uint8_t _greenPin;
     uint8_t _bluePin;
     uint8_t _red;
     uint8_t _green;
     uint8_t _blue;
     //long _fadeTime;
};

#endif
