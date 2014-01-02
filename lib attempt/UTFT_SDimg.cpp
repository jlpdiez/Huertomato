/* 
* UTFT_SDimg.cpp
*
* Created: 02/01/2014 12:19:35
* Author: eNDeR
*/


#include "UTFT_SDimg.h"
//#include <UTFT.h>

//Constructors
UTFT_SDimg::UTFT_SDimg() : UTFT() {}
	
UTFT_SDimg::UTFT_SDimg(byte model, int RS, int WR,int CS, int RST, int SER=0) :
	UTFT(model,RS,WR,CS,RST,SER) {}

/*UTFT_SDimg::UTFT_SDimg(byte model, int RS, int WR,int CS, int RST, int SER=0) {
		switch (model) {
			#ifndef DISABLE_HX8347A
				case HX8347A:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_ILI9327
				case ILI9327:
				disp_x_size=239;
				disp_y_size=399;
				display_transfer_mode=16;
				break;
			#endif
				#ifndef DISABLE_SSD1289
				case SSD1289:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=16;
				break;
				#endif
			#ifndef DISABLE_ILI9325C
				case ILI9325C:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=8;
				break;
			#endif
			#ifndef DISABLE_ILI9325D
				case ILI9325D_8:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=8;
				break;
			#endif
			#ifndef DISABLE_ILI9325D
				case ILI9325D_16:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_HX8340B
				case HX8340B_8:
				disp_x_size=175;
				disp_y_size=219;
				display_transfer_mode=8;
				break;
			#endif
			#ifndef DISABLE_HX8352A
				case HX8352A:
				disp_x_size=175;
				disp_y_size=219;
				display_transfer_mode=1;
				display_serial_mode=SERIAL_4PIN;
				break;
			#endif
			#ifndef DISABLE_HX8352A
				case HX8352A:
				disp_x_size=239;
				disp_y_size=399;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_
				case ST7735:
				disp_x_size=127;
				disp_y_size=159;
				display_transfer_mode=1;
				display_serial_mode=SERIAL_5PIN;
				break;
			#endif
			#ifndef DISABLE_PCF8833
				case PCF8833:
				disp_x_size=127;
				disp_y_size=127;
				display_transfer_mode=1;
				display_serial_mode=SERIAL_5PIN;
				break;
			#endif
			#ifndef DISABLE_S1D19122
				case S1D19122:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_SSD1963_480
				case SSD1963_480:
				disp_x_size=271;
				disp_y_size=479;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_SSD1963_800
				case SSD1963_800:
				disp_x_size=479;
				disp_y_size=799;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_S6D1121
				case S6D1121_8:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=8;
				break;
			#endif
			#ifndef DISABLE_S6D1121
				case S6D1121_16:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_SSD1289
				case SSD1289LATCHED:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=LATCHED_16;
				break;
			#endif
			#ifndef DISABLE_ILI9320
				case ILI9320_8:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=8;
				break;
			#endif
			#ifndef DISABLE_ILI9320
				case ILI9320_16:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=16;
				break;
			#endif
			#ifndef DISABLE_SSD1289_8
				case SSD1289_8:
				disp_x_size=239;
				disp_y_size=319;
				display_transfer_mode=8;
				break;
			#endif
		}
		display_model=model;

		if (display_transfer_mode!=1)
		{
			_set_direction_registers(display_transfer_mode);
			P_RS	= portOutputRegister(digitalPinToPort(RS));
			B_RS	= digitalPinToBitMask(RS);
			P_WR	= portOutputRegister(digitalPinToPort(WR));
			B_WR	= digitalPinToBitMask(WR);
			P_CS	= portOutputRegister(digitalPinToPort(CS));
			B_CS	= digitalPinToBitMask(CS);
			P_RST	= portOutputRegister(digitalPinToPort(RST));
			B_RST	= digitalPinToBitMask(RST);
			if (display_transfer_mode==LATCHED_16)
			{
				P_ALE	= portOutputRegister(digitalPinToPort(SER));
				B_ALE	= digitalPinToBitMask(SER);
				pinMode(SER,OUTPUT);
				cbi(P_ALE, B_ALE);
				pinMode(8,OUTPUT);
				digitalWrite(8, LOW);
			}
			pinMode(RS,OUTPUT);
			pinMode(WR,OUTPUT);
			pinMode(CS,OUTPUT);
			pinMode(RST,OUTPUT);
		}
		else
		{
			P_SDA	= portOutputRegister(digitalPinToPort(RS));
			B_SDA	= digitalPinToBitMask(RS);
			P_SCL	= portOutputRegister(digitalPinToPort(WR));
			B_SCL	= digitalPinToBitMask(WR);
			P_CS	= portOutputRegister(digitalPinToPort(CS));
			B_CS	= digitalPinToBitMask(CS);
			P_RST	= portOutputRegister(digitalPinToPort(RST));
			B_RST	= digitalPinToBitMask(RST);
			if (display_serial_mode!=SERIAL_4PIN)
			{
				P_RS	= portOutputRegister(digitalPinToPort(SER));
				B_RS	= digitalPinToBitMask(SER);
				pinMode(SER,OUTPUT);
			}
			pinMode(RS,OUTPUT);
			pinMode(WR,OUTPUT);
			pinMode(CS,OUTPUT);
			pinMode(RST,OUTPUT);
		}
}*/
		
UTFT_SDimg::UTFT_SDimg(const UTFT_SDimg &other) : UTFT(other) {}
	
//TODO: test if this works
UTFT_SDimg& UTFT_SDimg::operator=(const UTFT_SDimg &other) {
	*this = other;
	return *this;
}

//Draws a pixel line. Ideal from reading data from SD and displaying
void UTFT_SDimg::drawPixelLine(int x, int y, int sx, uint16_t* data) {
	unsigned int col;
	cbi(P_CS, B_CS);
	setXY(x, y, x+sx-1, y);
	for (int tc=0; tc<sx; tc++) {
		char* p = (char*)&data[tc];
		LCD_Write_DATA(*(p + 1), *p);
	}
	sbi(P_CS, B_CS);
}