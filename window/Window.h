#ifndef __WINDOW_H
#define __WINDOW_H

#include <UTFT.h>
#include <UTouch.h>

class Window {
  public:
    Window(UTFT *ptrUTFT, UTouch *ptrUTouch, char *name);
    
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual int processTouch() = 0;
    
  protected:
    char *_name;
    UTFT *_LCD;
    UTouch *_UTouch;
    Borderless_Buttons _buttons;
    
    //ADD flowButtons
    void printHeaderBackground();
//    void printMainHeader();
//    void printMenuHeader(char* c);
//    void updateMainHeader();  
};

Window::Window(UTFT *ptrUTFT, UTouch *ptrUTouch, char *name) : 
_LCD(ptrUTFT), _UTouch(ptrUTouch), _buttons(ptrUTFT,ptrUTouch), _name(name) {}

void Window::printHeaderBackground() {
  const int headerHeight = 20; 
  //Header background
  _LCD->setColor(lightGreen[0],lightGreen[1],lightGreen[2]);
  _LCD->fillRect(0,0,xSize,headerHeight);
  //Separtator line
  _LCD->setColor(darkGreen[0], darkGreen[1], darkGreen[2]);
  _LCD->drawLine(0, headerHeight, xSize, headerHeight);  
}

//void Window::printMainHeader() {
//  printHeader(); 
//  //Left
//  //_LCD->print("20:55:33",2,2);
//  //Center
//  //_LCD->print("20:55:33",xSize/2-bigLetterSize*4,2);
//  //RIGHT
//  //max X - 16px per letter X 8 letters - 2px spacing
//  //_LCD->print("20:57:23",xSize-(bigLetterSize*8)-2, 2);
//  
//  //Spacing is 4px between imgs (so its orig x + 16 + 4)
//  //if (alarm)
////  _LCD->drawBitmap(2, 2, 16, 16, alarm16);
////  //if (manual control)
////  _LCD->drawBitmap(22, 2, 16, 16, water16);
////  //if (watering plants)
////  _LCD->drawBitmap(42, 2, 16, 16, mano16); 
//  //Small TextS
//  _LCD->setFont(Sinclair_S);
//  _LCD->setColor(grey[0], grey[1], grey[2]);
//  _LCD->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
//  _LCD->print("Huertomato 1.2",10,6); 
//  //Change below too
//  //max X - 8px per letter X 5 letters - 6px spacing
//  _LCD->print("16:57:05",xSize-(8*smallLetterSize)-2, 6); 
//}
//
//void Window::updateMainHeader() {
//  _LCD->setFont(Sinclair_S);
//  _LCD->setColor(grey[0], grey[1], grey[2]);
//  _LCD->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]);
//  //max X - 8px per letter X 5 letters - 6px spacing
//  //Change above too
//  _LCD->print("16:57:05",xSize-(8*smallLetterSize)-2, 6); 
//}
//
//void Window::printMenuHeader(char* c) {
//  printHeader();
//  _LCD->setFont(hallfetica_normal);
//  _LCD->setColor(grey[0], grey[1], grey[2]);
//  _LCD->setBackColor(lightGreen[0],lightGreen[1],lightGreen[2]); 
//  _LCD->print(c,xSize/2-(bigLetterSize*(strlen(c)/2)),2);
//}

#endif
