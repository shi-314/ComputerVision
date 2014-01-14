// ========================================================== //
// Filter
// - Namespace für alle möglichen Filter
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 02.03.2011
// ========================================================== //

#ifndef _CONVOLUTION_H
#define _CONVOLUTION_H

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;
	
	namespace Filter {
		// Verfügbare Matrixgrößen
		enum MatrixDimensions {
			MATRIX_3 = 3,
			MATRIX_5 = 5,
			MATRIX_7 = 7
		};

		// Lokaler Operator: Bild mit 3x3 Matrix falten
		void convolve3(Bitmap* bmp, const int* matrix);
		void convolve3f(Bitmap* bmp, const float* matrix);

		// Lokaler Operator: Bild mit A x A Matrix falten
		template <typename T> void convolve(Bitmap* bmp, T* matrix, const MatrixDimensions matrixSize, const unsigned char nullValue = 127) {
			if(bmp->getBPP() != BITMAP_8)
				throw exception("Filter::convolve5f -- This method works only with 8 bit grayscale bitmaps");

			unsigned char* newBuffer = new unsigned char[bmp->getSize()];
			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer = 0, nMatPos = 0;
			
			// Vorberechnung wegen Performance
			vector<unsigned int> nextLinePos;
			nextLinePos.push_back(0);

			vector<unsigned int> matNextLinePos;
			matNextLinePos.push_back(0);

			for(int n=1; n<matrixSize; n++) {
				nextLinePos.push_back(bmp->getWidth()*n);
				matNextLinePos.push_back(matrixSize*n);
			}

			const int matrixSub1 = matrixSize - 1;
			const int matrixCenterAdd = bmp->getWidth() * (int)(matrixSize/2) + (int)(matrixSize/2);

			Vec2 pos, matPos;
			int matrixResult;

			memset(newBuffer, 127, bmp->getSize());

			// Alle Pixel durchlaufen
			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize();) {
				// Unterer Rand
				if(pos.y+matrixSub1 == bmp->getHeight()) {
					break;
				}
				// Rechter (und linker Rand)
				if(pos.x+matrixSub1 == bmp->getWidth()) {
					pos.x = 0;
					pos.y++;
					nPosOnBuffer+=matrixSub1;
					continue;
				}
				pos.x++;

				// Matrix durchlaufen und neuen Grauwert berechnen
				matrixResult = nullValue;
				for(unsigned char n=0; n<matrixSize; n++) {
					for(unsigned char m=0; m<matrixSize; m++) {
						matrixResult += (int)(matrix[matNextLinePos[n]+m] * buffer[nPosOnBuffer+nextLinePos[n]+m]);
					}
				}

				if(matrixResult > 255)
					matrixResult = 255;
				else if(matrixResult < 1)
					matrixResult = 0;
				
				newBuffer[nPosOnBuffer+matrixCenterAdd] = (unsigned char)matrixResult;
				nPosOnBuffer++;
			}
			// Buffer ändern
			bmp->setBuffer(newBuffer);
		}
	}
}

#endif