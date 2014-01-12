// ========================================================== //
// Histogram
// - f�r Bitmaps
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

			// Histogram f�r ein Bitmap berechnen
			void calculate(Bitmap* bmp);

			// Gibt die absolute H�ufigkeit der Farbe w zur�ck
			inline unsigned int getAbsoluteFrequency(unsigned char w) {return this->data[w];}

			// Gibt die relative H�ufigkeit der Farbe w zur�ck
			inline float getRelativeFrequency(unsigned char w) {return (float)this->data[w]/(float)amount;}

			// Gibt den Grauwert mit der h�chsten H�ufigkeit zur�ck
			unsigned char getHighestFrequency();

			// Gibt das Maximum (den hellsten Grauwert) zur�ck [0; 255]
			unsigned char getMaximum();

			// Gibt das Minimum (den dunkelsten Grauwert) zur�ck [0; 255]
			unsigned char getMinimum();

			// Gibt das gezeichnete Histogram als Bitmap zur�ck
			Bitmap getGraph();

		private:
			unsigned int data[256]; // absolute H�ufigkeiten
			unsigned int amount;	// Umfang / Anzahl Pixel
	};
}

#endif