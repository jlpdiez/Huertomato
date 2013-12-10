#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include "Window.h"

class MainWindow : public Window {
  public:
    Window(UTFT *ptrUTFT, UTouch *ptrUTouch, char *name);
    
    void draw();
    void update();
    int processTouch();
    
  protected:
    char *_name;
    UTFT *_LCD;
    UTouch *_UTouch;
    Borderless_Buttons _buttons;
    
    //void printHeaderBackground();
};

MainWindow::MainWindow() : Window(_LCD,_UTouch) {
  
}

#endif
