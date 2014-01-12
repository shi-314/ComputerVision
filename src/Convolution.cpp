// ========================================================== //
// Filter
// - Namespace für alle möglichen Filter
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 02.03.2011
// ========================================================== //

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;

	namespace Filter {
		// ========================================================== //
		// convolve3
		// ========================================================== //
		void convolve3(Bitmap* bmp, const int* matrix) {
			if(bmp->getBPP() != BITMAP_8)
				throw exception("Filter::convolve3 -- This method works only with 8 bit grayscale bitmaps");

			unsigned char* newBuffer = new unsigned char[bmp->getSize()];
			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer = 0, nMatPos = 0;

			Vec2 pos, matPos;
			int matrixResult = 0;

			// Unteren Rand zeichnen (schwarz)
			newBuffer[0] = 0;
			for(nPosOnBuffer=bmp->getWidth(); nPosOnBuffer>0; nPosOnBuffer--) {
				newBuffer[nPosOnBuffer] = 0;
			}
			
			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize();) {
				// Unterer Rand
				if(pos.y+2 == bmp->getHeight()) {
					newBuffer[nPosOnBuffer+bmp->getWidth()] = 0;		// Rand
					nPosOnBuffer++;
					pos.x++;
					if(pos.x+2 == bmp->getWidth()) {
						newBuffer[nPosOnBuffer+1] = 0;
						nPosOnBuffer+=bmp->getWidth();
						newBuffer[nPosOnBuffer] = 0;	// Rand
						newBuffer[nPosOnBuffer+1] = 0;	// Rand
						break;
					}
					continue;
				}
				// Rechter (und linker Rand)
				if(pos.x+2 == bmp->getWidth()) {
					pos.x = 0;
					pos.y++;
					newBuffer[nPosOnBuffer+1] = 0;	// Rand
					nPosOnBuffer+=2;
					newBuffer[nPosOnBuffer] = 0;	// linker Rand
					continue;
				}
				pos.x++;

				// Matrix durchlaufen und neuen Grauwert berechnen
				matrixResult = 127;

				matrixResult += matrix[0] * buffer[nPosOnBuffer];
				matrixResult += matrix[1] * buffer[nPosOnBuffer+1];
				matrixResult += matrix[2] * buffer[nPosOnBuffer+2];

				nMatPos = nPosOnBuffer+bmp->getWidth();
				matrixResult += matrix[3] * buffer[nMatPos];
				matrixResult += matrix[4] * buffer[nMatPos+1];
				matrixResult += matrix[5] * buffer[nMatPos+2];

				nMatPos += bmp->getWidth();
				matrixResult += matrix[6] * buffer[nMatPos];
				matrixResult += matrix[7] * buffer[nMatPos+1];
				matrixResult += matrix[8] * buffer[nMatPos+2];

				if(matrixResult > 255)
					matrixResult = 255;
				else if(matrixResult < 1)
					matrixResult = 0;
				
				nPosOnBuffer++;
				newBuffer[nPosOnBuffer+bmp->getWidth()] = (unsigned char)matrixResult;
			}
			// Buffer ändern
			bmp->setBuffer(newBuffer);
		}

		void convolve3f(Bitmap* bmp, const float* matrix) {
			if(bmp->getBPP() != BITMAP_8)
				throw exception("Filter::convolve3f -- This method works only with 8 bit grayscale bitmaps");

			unsigned char* newBuffer = new unsigned char[bmp->getSize()];
			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer = 0, nMatPos = 0;

			Vec2 pos, matPos;
			int matrixResult = 0;

			// Unteren Rand zeichnen (schwarz)
			newBuffer[0] = 0;
			for(nPosOnBuffer=bmp->getWidth(); nPosOnBuffer>0; nPosOnBuffer--) {
				newBuffer[nPosOnBuffer] = 0;
			}

			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize();) {
				// Unterer Rand
				if(pos.y+2 == bmp->getHeight()) {
					newBuffer[nPosOnBuffer+bmp->getWidth()] = 0;		// Rand
					nPosOnBuffer++;
					pos.x++;
					if(pos.x+2 == bmp->getWidth()) {
						newBuffer[nPosOnBuffer+1] = 0;
						nPosOnBuffer+=bmp->getWidth();
						newBuffer[nPosOnBuffer] = 0;	// Rand
						newBuffer[nPosOnBuffer+1] = 0;	// Rand
						break;
					}
					continue;
				}
				// Rechter (und linker Rand)
				if(pos.x+2 == bmp->getWidth()) {
					pos.x = 0;
					pos.y++;
					newBuffer[nPosOnBuffer+1] = 0;	// Rand
					nPosOnBuffer+=2;
					newBuffer[nPosOnBuffer] = 0;	// linker Rand
					continue;
				}
				pos.x++;

				// Matrix durchlaufen und neuen Grauwert berechnen
				matrixResult = 0;

				matrixResult += (int)(matrix[0] * buffer[nPosOnBuffer]);
				matrixResult += (int)(matrix[1] * buffer[nPosOnBuffer+1]);
				matrixResult += (int)(matrix[2] * buffer[nPosOnBuffer+2]);

				nMatPos = nPosOnBuffer+bmp->getWidth();
				matrixResult += (int)(matrix[3] * buffer[nMatPos]);
				matrixResult += (int)(matrix[4] * buffer[nMatPos+1]);
				matrixResult += (int)(matrix[5] * buffer[nMatPos+2]);

				nMatPos += bmp->getWidth();
				matrixResult += (int)(matrix[6] * buffer[nMatPos]);
				matrixResult += (int)(matrix[7] * buffer[nMatPos+1]);
				matrixResult += (int)(matrix[8] * buffer[nMatPos+2]);

				if(matrixResult > 255)
					matrixResult = 255;
				else if(matrixResult < 1)
					matrixResult = 0;
				
				nPosOnBuffer++;
				newBuffer[nPosOnBuffer+bmp->getWidth()] = (unsigned char)matrixResult;
			}
			// Buffer ändern
			bmp->setBuffer(newBuffer);
		}

	} // namespace Filter
} // namespace ComputerVision