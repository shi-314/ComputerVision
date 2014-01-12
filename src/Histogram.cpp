// ========================================================== //
// Histogram
//
// Shivan Taher <shi-89@gmx.net>
// Created: 02.03.2011
// ========================================================== //

#include "Histogram.h"

using namespace std;

namespace ComputerVision {
	
	// ========================================================== //
	// Histogram
	// ========================================================== //
	Histogram::Histogram() {
	}

	Histogram::Histogram(Bitmap* bmp) {
		this->calculate(bmp);
	}

	Histogram::~Histogram() {
	}

	// ========================================================== //
	// calculate
	// ========================================================== //
	void Histogram::calculate(Bitmap* bmp) {
		memset(this->data, 0, 256*sizeof(unsigned int));
		this->amount = bmp->getWidth()*bmp->getHeight();
		
		unsigned char* buffer = bmp->getBuffer();
		unsigned int nPosOnBuffer = 0;

		if(bmp->getBPP() == BITMAP_24) {
			unsigned char r,g,b;

			int pixelBrightness;

			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize(); nPosOnBuffer+=3) {
				b = buffer[nPosOnBuffer];
				g = buffer[nPosOnBuffer+1];
				r = buffer[nPosOnBuffer+2];
				
				pixelBrightness = (int)(r + g + b)/3;
				this->data[pixelBrightness]++;
			}
		} else {
			unsigned char v;

			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize(); nPosOnBuffer++) {
				v = buffer[nPosOnBuffer];
				this->data[v]++;
			}
		}
	}

	// ========================================================== //
	// getHighestFrequency
	// ========================================================== //
	unsigned char Histogram::getHighestFrequency() {
		unsigned int highestValue = 0;
		unsigned char highestN = 0;

		for(int n=0; n<256; n++) {
			if(this->data[n] > highestValue) {
				highestValue = this->data[n];
				highestN = n;
			}
		}

		return highestN;
	}

	// ========================================================== //
	// getGraph
	// ========================================================== //
	Bitmap Histogram::getGraph() {
		Bitmap* graph = new Bitmap(Vec2(260, 190));
		graph->fill(Color(235, 235, 235));

		unsigned char hn = this->getHighestFrequency();
		float hf = this->getRelativeFrequency(hn);
		const float hightConst = 178/hf;

		for(int n=0; n<256; n++) {
			int x = 2+n;
			graph->setPixel(Vec2(x, 181), Color(n,n,n));
			graph->setPixel(Vec2(x, 182), Color(n,n,n));
			graph->setPixel(Vec2(x, 183), Color(n,n,n));
			graph->setPixel(Vec2(x, 184), Color(n,n,n));
			graph->setPixel(Vec2(x, 185), Color(n,n,n));
			graph->setPixel(Vec2(x, 186), Color(n,n,n));
			graph->setPixel(Vec2(x, 187), Color(n,n,n));

			graph->drawLine(Vec2(x, 179), Vec2(x, 179-(int)(this->getRelativeFrequency(n)*hightConst)), Color(0,0,0));
		}

		return *graph;
	}

	// ========================================================== //
	// getMaximum
	// ========================================================== //
	unsigned char Histogram::getMaximum() {
		for(int n=255; n>=0; n--) {
			if(this->data[n] > 0)
				return n;
		}
		return 0;
	}

	// ========================================================== //
	// getMinimum
	// ========================================================== //
	unsigned char Histogram::getMinimum() {
		for(int n=0; n<256; n++) {
			if(this->data[n] > 0)
				return n;
		}
		return 255;
	}
}