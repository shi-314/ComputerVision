// ========================================================== //
// Filter
// - Namespace für alle möglichen Filter
//
// Shivan Taher <shi-89@gmx.net>
// Created: 02.03.2011
// ========================================================== //

#ifndef _FILTER_H
#define _FILTER_H

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;

	namespace Filter {
		// Punktoperator: Grauwertspreizung - Verbessert den Kontrast
		void histogramEqualization(Bitmap* bmp);

		// Rechnet das Bild in ein Grauwertbild um
		// - In channels wird die Kanalgewichtung [0;1] angegeben
		// - Punktoperator
		void grayscale(Bitmap* bmp, Vec3f channels = Vec3f(0.299f, 0.587f, 0.114f));

		// Lokaler Operator: Kantendetektion
		void laplace(Bitmap* bmp);

		// Lokaler Operator: Kantendetektion
		void laplacianOfGaussian(Bitmap* bmp);

		// Lokaler Operator: Kantendetektion
		void sobel(Bitmap* bmp, const int* altXMatrix = 0, const int* altYMatrix = 0);

		// Lokaler Operator: Glättung
		void gaussianBlur(Bitmap* bmp, const MatrixDimensions matDim = MATRIX_3);

		// Lokaler Operator: Glättung (Mittelwertfilter)
		void blur(Bitmap* bmp, const MatrixDimensions matDim = MATRIX_3);

		// Punktoperator: Schwellwert-Filter
		void threshold(Bitmap* bmp, unsigned char min, unsigned char value);

		// Linienerkennung
		void shivanAlgo(Bitmap* bmp);

		// Katentendetection: Nutzt Gauss-und Sobeloperatoren (Abkürzung zur Klasse Canny)
		void canny(Bitmap* src, int lowThresh, int highThresh);
	}
}

#endif