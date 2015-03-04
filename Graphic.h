// Graphic.h

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <UTFT.h>

template <class TElem>
class Graphic {
	public:
		//Constructors, destructor
		Graphic(UTFT *lcd);
		Graphic(const Graphic<TElem>&);
		Graphic& operator=(const Graphic<TElem>&);
		~Graphic();
		
		void setAxis(int x, TElem valMax = 100, TElem valMin = 0);
		void addReading(const TElem&);
		//Draws _xSize * _ySize starting at xSpacer,ySpacer
		void draw(const int xSpacer, const int ySpacer);
		
	private:
		UTFT *_lcd;
		int _xSize;
		TElem _yMax;
		TElem _yMin;
		//Array of length x that holds the position of the value in y
		TElem _dot[];
};

template <class TElem>
inline Graphic<TElem>::Graphic(UTFT *lcd) : _lcd(lcd) {
	/*for (int i = 0; i < _xSize; i++) {
		_dot[i] = NULL;
	}*/
}

template <class TElem>
inline Graphic<TElem>::Graphic(const Graphic<TElem> &other) {
	_lcd = other._lcd;
	_xSize = other._xSize;
	_yMax = other._yMax;
	_yMin = other._yMin;
	for (int i = 0; i < _xSize; i++) {
		_dot[i] = other._dot[i];
	}
}

template <class TElem>
inline Graphic<TElem>& Graphic<TElem>::operator=(const Graphic<TElem> &other) {
	_lcd = other._lcd;
	_xSize = other._xSize;
	_yMax = other._yMax;
	_yMin = other._yMin;
	for (int i = 0; i < _xSize; i++) {
		_dot[i] = other._dot[i];
	}
	return *this;
}

template <class TElem>
Graphic<TElem>::~Graphic() {}

template <class TElem>
//Input: Size of data array, max value of data contained and min value of data contained
void Graphic<TElem>::setAxis(int x, TElem valMax, TElem valMin) {
	_xSize = x;
	_yMax = valMax;
	_yMin = valMin;
	for (int i = 0; i < _xSize; i++) {
		_dot[i] = NULL;
	}
}

template <class TElem>
void Graphic<TElem>::addReading(const TElem &reading) {
	//If acceptable
	if ((reading <= _yMax) && (reading >= _yMin)) {
		//Moves all array one place to the left
		for (int i = 0; i < _xSize-1 ; i++) {
			//if (_dot[i+1] != NULL)
				_dot[i] = _dot[i+1];
		}
		//Inserts reading to last position of array
		_dot[_xSize-1] = reading;
	}
}

template <class TElem>
//Draws graphic starting at xSpacer, ySpacer which should be top left corner
void Graphic<TElem>::draw(const int xSpacer, const int ySpacer) {
	const int dotSize = 2;
	_lcd->setBackColor(VGA_WHITE);
	_lcd->setColor(VGA_WHITE);
	_lcd->fillRect(xSpacer,ySpacer,xSpacer+_xSize*dotSize,ySpacer+_yMax);
	_lcd->setColor(100,100,100);
	//Draws a marquee for the graph
	//_lcd->drawRect(xSpacer,ySpacer,xSpacer+_xSize*dotSize,ySpacer-_yMax);
	//Draws a 2x2 point
	for (int i = 0; i < _xSize; i++) {
		if (_dot[i] != NULL) {
			_lcd->drawPixel(xSpacer+i*dotSize, ySpacer+_yMax-_dot[i]);
			_lcd->drawPixel(xSpacer+i*dotSize+1, ySpacer+_yMax-_dot[i]);
			_lcd->drawPixel(xSpacer+i*dotSize, ySpacer+_yMax-_dot[i]-1);
			_lcd->drawPixel(xSpacer+i*dotSize+1, ySpacer+_yMax-_dot[i]-1);
		}
	}
}

#endif

