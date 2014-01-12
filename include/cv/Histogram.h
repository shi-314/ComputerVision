// ========================================================== //
// Histogram
// - für Bitmaps
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 02.03.2011
// ========================================================== //

#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;
	
	class Histogram {
		public:
			Histogram();			
			Histogram(Bitmap* bmp);
			~Histogram();

			// Histogram für ein Bitmap berechnen
			void calculate(Bitmap* bmp);

			// Gibt die absolute Häufigkeit der Farbe w zurück
			inline unsigned int getAbsoluteFrequency(unsigned char w) {return this->data[w];}

			// Gibt die relative Häufigkeit der Farbe w zurück
			inline float getRelativeFrequency(unsigned char w) {return (float)this->data[w]/(float)amount;}

			// Gibt den Grauwert mit der höchsten Häufigkeit zurück
			unsigned char getHighestFrequency();

			// Gibt das Maximum (den hellsten Grauwert) zurück [0; 255]
			unsigned char getMaximum();

			// Gibt das Minimum (den dunkelsten Grauwert) zurück [0; 255]
			unsigned char getMinimum();

			// Gibt das gezeichnete Histogram als Bitmap zurück
			Bitmap getGraph();

		private:
			unsigned int data[256]; // absolute Häufigkeiten
			unsigned int amount;	// Umfang / Anzahl Pixel
	};
}

#endif