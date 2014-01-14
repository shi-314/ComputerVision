// ========================================================== //
// ColorPalette
// - Zum Verwalten von Bitmap Farbpaletten
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 17.02.2011
// ========================================================== //

#ifndef _COLOR_PALETTE_H
#define _COLOR_PALETTE_H

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;

	class ColorPalette {
		friend class Bitmap;
		public:
			ColorPalette();
			// 256 => 8 Bit, ...
			ColorPalette(unsigned int size);
			~ColorPalette();

			// Gibt die Größe der Farbpalette zurück = Anzahl der Farben = Farbtiefe
			inline unsigned int getSize() {return this->size;};
			// Setzt eine Farbe an der Stelle index auf color
			void setColor(unsigned int index, Color color);
			// Gibt die Farbe an der Stelle index zurück
			Color getColor(unsigned int index);

		private:
			unsigned int size;
			Color* colors;
	};
}

#endif