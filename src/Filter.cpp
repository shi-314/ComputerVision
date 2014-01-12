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
		// Faltungsmatrizen
		// ========================================================== //
		// 3x3 Matrix: Laplace-Filter
		const int matLaplacian3[] =	{0,	-1,	 0,
									-1,	 4,	-1,
									 0,	-1,	 0};

		const int matLaplacian3b[] =	{1,  1,	1,
										 1, -8,	1,
										 1,  1, 1};
		// 5x5 Matrix: Laplacian of Gauss
		const int matLog5[] =	{0,	 0,	-1,	 0,	 0,
								 0,	-1,	-2,	-1,	 0,
								-1,	-2,	16,	-2,	-1,
								 0,	-1,	-2,	-1,	 0,
								 0,	 0,	-1,	 0,	 0};

		// 5x5 Matrix: Gauß-Filter
		const float matGaussian5f[] = {	1.0f/273,  4.0f/273,  7.0f/273,  4.0f/273,  1.0f/273,
										4.0f/273,  16.0f/273, 26.0f/273, 16.0f/273, 4.0f/273,
										7.0f/273,  26.0f/273, 41.0f/273, 26.0f/273, 7.0f/273,
										4.0f/273,  16.0f/273, 26.0f/273, 16.0f/273, 4.0f/273,
										1.0f/273,  4.0f/273,  7.0f/273,  4.0f/273,  1.0f/273};

		// 3x3 Matrix: Gauß-Filter
		const float matGaussian3f[] = {	1.0f/16.0f,  2.0f/16.0f,  1.0f/16.0f,
										2.0f/16.0f,  4.0f/16.0f,  2.0f/16.0f,
										1.0f/16.0f,  2.0f/16.0f,  1.0f/16.0f};

		// 3x3 Matrix: Sobel und Scharr Operator
		const int matSobelX3[] = {   1, 0, -1,
									 2,	0, -2,
									 1,	0, -1};

		const int matSobelY3[] = {  1,  2,  1,
									0,	0,  0,
								   -1, -2, -1};

		const int matScharrX3[] = { 3,  0, -3,
									10,	0, -10,
									3,	0, -3};

		const int matScharrY3[] = { 3,  10,  3,
									0,	0,   0,
								   -3, -10, -3};

		// Mittelwertfilter
		const float matBlur3f[] = {  1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f,
									 1.0f/9.0f,	1.0f/9.0f, 1.0f/9.0f,
									 1.0f/9.0f,	1.0f/9.0f, 1.0f/9.0f};

		const float matBlur5f[] = {  1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f,
									 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f,
									 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f,
									 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f,
									 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f};


		// ========================================================== //
		// histogramEqualization
		// ========================================================== //
		void histogramEqualization(Bitmap* bmp) {
			if(bmp->getBPP() == BITMAP_24)
				throw exception("Filter::histogramEqualization -- This method works only with 8 bit grayscale bitmaps");
			
			Histogram hist(bmp);
			unsigned char gmax = hist.getMaximum();
			unsigned char gmin = hist.getMinimum();
			unsigned char gdiff = gmax-gmin;
			
			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer;
			unsigned char w;
			
			if(gdiff == 0) gdiff = 1;
			const float c = 255.0f / gdiff;

			// Grauwertbild mit 8 Bit Farbtiefe
			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize(); nPosOnBuffer++) {
				w = buffer[nPosOnBuffer];
				w = (unsigned char)((w-gmin) * c);
				buffer[nPosOnBuffer] = w;
			}
		}

		// ========================================================== //
		// grayscale
		// ========================================================== //
		void grayscale(Bitmap* bmp, Vec3f channels /*= Vec3f(0.299f, 0.587f, 0.114f)*/) {
			if(bmp->getBPP() == BITMAP_8)
				throw exception("Filter::grayscale -- Bitmap is already a grayscale image with 8 bpp");

			int pixelBrightness;

			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer = 0;
			unsigned char r,g,b;

			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize();) {
				b = buffer[nPosOnBuffer];
				g = buffer[nPosOnBuffer+1];
				r = buffer[nPosOnBuffer+2];
				
				pixelBrightness = (int)(channels.x*r + channels.y*g + channels.z*b);

				buffer[nPosOnBuffer++] = (unsigned char)pixelBrightness;
				buffer[nPosOnBuffer++] = (unsigned char)pixelBrightness;
				buffer[nPosOnBuffer++] = (unsigned char)pixelBrightness;
			}			
		}

		// ========================================================== //
		// laplace
		// ========================================================== //
		void laplace(Bitmap* bmp) {
			convolve(bmp, Filter::matLaplacian3, Filter::MATRIX_3);
		}

		// ========================================================== //
		// laplacianOfGaussian
		// ========================================================== //
		void laplacianOfGaussian(Bitmap* bmp) {
			convolve(bmp, Filter::matLog5, Filter::MATRIX_3);
		}

		// ========================================================== //
		// sobel
		// ========================================================== //
		void sobel(Bitmap* bmp, const int* altXMatrix /*= 0*/, const int* altYMatrix /*= 0*/) {
			if(bmp->getBPP() != BITMAP_8)
				throw exception("Filter::sobel -- This method works only with 8 bit grayscale bitmaps");

			unsigned char* newBuffer = new unsigned char[bmp->getSize()];
			memset(newBuffer, 0, bmp->getSize());
			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer = 0, nMatPos = 0;

			Vec2 pos, matPos;
			int sum = 0, sumX = 0, sumY = 0;

			if(altXMatrix==0)
				altXMatrix = matSobelX3;
			if(altYMatrix==0)
				altYMatrix = matSobelY3;
			
			for(nPosOnBuffer=0; nPosOnBuffer<bmp->getSize();) {
				// Unterer Rand
				if(pos.y+2 == bmp->getHeight()) {
					break;
				}
				// Rechter (und linker Rand)
				if(pos.x+2 == bmp->getWidth()) {
					pos.x = 0;
					pos.y++;
					nPosOnBuffer+=2;
					continue;
				}
				pos.x++;

				// Sobel Faltungsmatrix in X Richtung
				sumX =  altXMatrix[0] * buffer[nPosOnBuffer];
				sumX += altXMatrix[1] * buffer[nPosOnBuffer+1];
				sumX += altXMatrix[2] * buffer[nPosOnBuffer+2];
				sumY =  altYMatrix[0] * buffer[nPosOnBuffer];
				sumY += altYMatrix[1] * buffer[nPosOnBuffer+1];
				sumY += altYMatrix[2] * buffer[nPosOnBuffer+2];

				nMatPos = nPosOnBuffer+bmp->getWidth();
				sumX += altXMatrix[3] * buffer[nMatPos];
				sumX += altXMatrix[4] * buffer[nMatPos+1];
				sumX += altXMatrix[5] * buffer[nMatPos+2];
				sumY += altYMatrix[3] * buffer[nMatPos];
				sumY += altYMatrix[4] * buffer[nMatPos+1];
				sumY += altYMatrix[5] * buffer[nMatPos+2];

				nMatPos += bmp->getWidth();
				sumX += altXMatrix[6] * buffer[nMatPos];
				sumX += altXMatrix[7] * buffer[nMatPos+1];
				sumX += altXMatrix[8] * buffer[nMatPos+2];
				sumY += altYMatrix[6] * buffer[nMatPos];
				sumY += altYMatrix[7] * buffer[nMatPos+1];
				sumY += altYMatrix[8] * buffer[nMatPos+2];

				sum = (long)sqrt((float)(sumX*sumX + sumY*sumY));
				//sum = abs(sumX)+abs(sumY);
				if(sum > 255)
					sum = 255;
				else if(sum < 1)
					sum = 0;
				
				nPosOnBuffer++;
				newBuffer[nPosOnBuffer+bmp->getWidth()] = (unsigned char)sum;
			}
			// Buffer ändern
			bmp->setBuffer(newBuffer);
		}

		// ========================================================== //
		// gaussianBlur
		// ========================================================== //
		void gaussianBlur(Bitmap* bmp, const MatrixDimensions matDim/*=MATRIX_3*/) {
			if(matDim == MATRIX_5)
				convolve(bmp, Filter::matGaussian5f, Filter::MATRIX_5, 0);
			else
				convolve(bmp, Filter::matGaussian3f, Filter::MATRIX_3, 0);
		}

		// ========================================================== //
		// blur
		// ========================================================== //
		void blur(Bitmap* bmp, const MatrixDimensions matDim/*=MATRIX_3*/) {
			if(matDim == MATRIX_5)
				convolve(bmp, Filter::matBlur5f, Filter::MATRIX_5, 0);
			else
				convolve(bmp, Filter::matBlur3f, Filter::MATRIX_3, 0);
		}

		// ========================================================== //
		// threshold
		// ========================================================== //
		void threshold(Bitmap* bmp, unsigned char min, unsigned char value) {
			if(bmp->getBPP() != BITMAP_8)
				throw exception("Filter::threshold -- This method works only with 8 bit grayscale bitmaps");

			unsigned char* buffer = bmp->getBuffer();
			unsigned int nPosOnBuffer = bmp->getSize()-1;
			unsigned char w;
			
			for(nPosOnBuffer; nPosOnBuffer > 0; nPosOnBuffer--) {
				w = buffer[nPosOnBuffer];
				
				if(w > min) {
					buffer[nPosOnBuffer] = value;
				}  else {
					buffer[nPosOnBuffer] = (unsigned char)255-value;
				}
			}
		}

		// ========================================================== //
		// canny
		// ========================================================== //
		void canny(Bitmap* src, int lowThresh, int highThresh) {
			Canny c(src);
			c.apply(lowThresh, highThresh);
		}
	} // namespace Filter
} // namespace ComputerVision