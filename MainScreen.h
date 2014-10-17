/*
 * mainScreen.h
 *
 * Created: 10/17/2014 9:12:39 PM
 *  Author: eNDeR
 */ 


#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

class MainScreen: public Window {
	public:
		MainScreen();
		MainScreen(const MainScreen &other);
		MainScreen& operator=(const MainScreen &other);
		~MainScreen();
		
		void printWindow();
		void drawWindow();
		void updateWindow();
		void processTouchWindow();
		
	private:
		uint8_t _actScreen;
		
		static UTFT *_lcd;
		static UTouch *_touch;
		static Sensors *_sensors;
		static Settings *_settings;
		static Borderless_Buttons _buttons;
		
		void printMainHeader();
		void updateMainHeader();
};

MainScreen::MainScreen() : _actScreen(1) {
	
}



#endif 