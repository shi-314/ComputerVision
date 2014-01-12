// ========================================================== //
// ColorPalette
// - Zum Verwalten von Bitmap Farbpaletten
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 17.02.2011
// ========================================================== //

#include <cv/ComputerVision.h>

using namespace std;

namespace ComputerVision {

	// ========================================================== //
	// ColorPalette
	// ========================================================== //
	ColorPalette::ColorPalette() {
		this->colors = 0;
		this->size = 0;
	}

	ColorPalette::ColorPalette(unsigned int size) {
		this->colors = new Color[size];
		this->size = size;
	}

	ColorPalette::~ColorPalette() {
		if(this->colors != 0)
			delete[] this->colors;
	}

	// ========================================================== //
	// setColor
	// ========================================================== //
	void ColorPalette::setColor(unsigned int index, Color color) {
		if(index < this->size) {
			this->colors[index] = color;
		}else{
			throw exception("ColorPalette::setColor -- index out of range");
		}
	}

	// ========================================================== //
	// getColor
	// ========================================================== //
	Color ColorPalette::getColor(unsigned int index) {
		if(index < this->size) {
			return this->colors[index];
		}else{
			throw exception("ColorPalette::getColor -- index out of range");
		}
	}
}