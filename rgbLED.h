// #############################################################################
// #
// # Scriptname : rgbLed.h
// # Author     : Juan L. Pérez Diez <ender.vs.melkor at gmail>
// # Date       : 30.07.2012
// 
// # Description: Library for controlling an RGB LED
// # Constructor needs red, green and blue pins as argument
// 
// # Based on http://amkimian.blogspot.com.es/2009/07/trivial-led-class.html
//
// # Version    : 1.2
// #############################################################################

#ifndef __RGBLED_H
#define __RGBLED_H

enum color { RED, GREEN, BLUE, WHITE };
class rgbLED {
  public:
     rgbLED(uint8_t, uint8_t, uint8_t);
     void setColor(color);
     void setOn();
     void setOff();
     // Turn the led on for a given amount of time (relies on a call to check() in the main loop())
     void setOnForTime(int millis, color);
     void check();
  private:
     uint8_t _redPin;
     uint8_t _greenPin;
     uint8_t _bluePin;
     color _actualColor;
     long _turnOffTime;
};

//Constructor
rgbLED::rgbLED(uint8_t r, uint8_t g, uint8_t b) : 
      _redPin(r), _greenPin(g), _bluePin(b), _turnOffTime(0) { 
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    _actualColor = WHITE;
}

//Drives led coloring
void rgbLED::setColor(color c) {
   switch(c) {
      case RED:    
        analogWrite(_redPin,255);
        analogWrite(_greenPin,0);
        analogWrite(_bluePin,0);
        break;
      case GREEN:   
        analogWrite(_redPin,0);
        analogWrite(_greenPin,255);
        analogWrite(_bluePin,0);
        break;
      case BLUE:  
        analogWrite(_redPin,0);
        analogWrite(_greenPin,0);
        analogWrite(_bluePin,255);
        break;
      case WHITE:  
        analogWrite(_bluePin,255);
        analogWrite(_redPin,255);
        analogWrite(_greenPin,255);
        break;
      default: break;
   } 
   _actualColor = c;
}


void rgbLED::setOn() {
   rgbLED::setColor(_actualColor);
}

void rgbLED::setOff() {
   digitalWrite(_redPin, LOW);
   digitalWrite(_greenPin, LOW);
   digitalWrite(_bluePin, LOW);
}

void rgbLED::setOnForTime(int p_millis, color c) {
   _turnOffTime = millis() + p_millis;
   setColor(c);
}

void rgbLED::check() {
   if (_turnOffTime != 0 && (millis() > _turnOffTime)) {
     _turnOffTime = 0;
     setOff();
   }
}
#endif
