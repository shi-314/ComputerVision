// ========================================================== //
// Matrix2
// - Repräsentiert eine 2 dimensionale Matrix (Array)
//
// Shivan Taher <shi-89@gmx.net>
// Created: 17.05.2011
// ========================================================== //

#include "Matrix2.h"

using namespace std;

namespace ComputerVision {
	// ========================================================== //
	// Matrix2
	// ========================================================== //
	template <typename T> inline Matrix2<T>::Matrix2(unsigned int w, unsigned int h) {
		this->width = w;
		this->height = h;

		this->data = new T*[w];
		for(unsigned int i=0; i<width; i++) {
			this->data[i] = new T[h];
			memset(this->data[i], 0, h*sizeof(T));
		}
	}

	template <typename T> Matrix2<T>::~Matrix2() {
		for(unsigned int i=0; i<width; i++) {
			delete[] this->data[i];
		}
		delete[] this->data;
	}

	// ========================================================== //
	// index operator
	// ========================================================== //
	template <typename T> inline T* Matrix2<T>::operator[](unsigned int i) {
		return this->data[i];
	}
}