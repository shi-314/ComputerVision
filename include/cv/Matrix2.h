// ========================================================== //
// Matrix2
// - Repräsentiert eine 2 dimensionale Matrix (Array)
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 17.05.2011
// ========================================================== //

#ifndef _MATRIX_2_H
#define _MATRIX_2_H

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;
	
	template <typename T=int> class Matrix2 {
		public:
			T** data;

			inline Matrix2(unsigned int w, unsigned int h);
			~Matrix2();

			inline T* operator [](unsigned int i);

			inline unsigned int getWidth() {return this->width;}
			inline unsigned int getHeight() {return this->height;}

		private:
			unsigned int width;
			unsigned int height;
	};
}

#include "Matrix2.cpp"

#endif