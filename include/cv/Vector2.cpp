// ========================================================== //
// Vector2
// - Template-Klasse für 2 dimensionale Vektoren mit untersch.
//   Datentypen für die einzelnen Elemente (double, int, float..)
//
// Hinweis: Die Datei darf vom Compiler nicht automatisch mit-
//			kompiliert werden, da sonst unresolved externals
//			auftreten. Diese Datei wird in Vector2.h includet.
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 06.02.2011
// ========================================================== //

#include "Vector2.h"

using namespace std;

namespace ComputerVision {

	// ========================================================== //
	// Vector2
	// ========================================================== //
	template <typename T> inline Vector2<T>::Vector2() {
		this->x = 0;
		this->y = 0;
	}

	template <typename T> inline Vector2<T>::Vector2(T x, T y) {
		this->x = x;
		this->y = y;
	}

	template <typename T> Vector2<T>::~Vector2() {
	}

	// ========================================================== //
	// length
	// ========================================================== //
	template <typename T> inline double Vector2<T>::length() {
		return sqrt((double)(this->x*this->x) + (double)(this->y*this->y));
	}

	// ========================================================== //
	// normalize (Vorsicht: Macht bei Vector2<int> keinen Sinn,
	// da immer 0 raus kommt!)
	// ========================================================== //
	template <typename T> void Vector2<T>::normalize() {
		double len = this->length();
		if(len > 0) {
			this->x = (T)(this->x/len);
			this->y = (T)(this->y/len);
		} else {
			throw exception("Vector2<T>::normalize() -- division by zero");
		}
	}

	// ========================================================== //
	// distance
	// ========================================================== //
	template <typename T> template <typename P> inline double Vector2<T>::distance(Vector2<P> v) {
		v-=(Vector2<T>)(*this);
		return abs( v.length() );
	}

	// ========================================================== //
	// crossProduct
	// ========================================================== //
	template <typename T> inline Vector2<T> Vector2<T>::crossProduct() {
		return Vector2<T>(this->y, -this->x);
	}

	// ========================================================== //
	// angle
	// ========================================================== //
	template <typename T> template <typename P> inline double Vector2<T>::angle(Vector2<P> v) {
		// cos a = |m1*m2|/(|m1|*|m2|)
		double cos_a = abs(*this*v)/(this->length()*v.length());
		return acos(cos_a);
	}

	// ========================================================== //
	// O P E R A T O R S
	// ========================================================== //

	// operator =
	template <typename T> template <typename P> inline void Vector2<T>::operator =(const Vector2<P>& v) {
		this->x = (T)v.x;
		this->y = (T)v.y;
	}

	// operator +
	template <typename T> template <typename P> inline Vector2<T> Vector2<T>::operator +(const Vector2<P>& v) {
		Vector2<T> res;
		res.x = (T)(this->x + v.x);
		res.y = (T)(this->y + v.y);
		return res;
	}

	// operator -
	template <typename T> template <typename P> inline Vector2<T> Vector2<T>::operator -(const Vector2<P>& v) {
		Vector2<T> res;
		res.x = (T)(this->x - v.x);
		res.y = (T)(this->y - v.y);
		return res;
	}

	// operator *
	template <typename T> template <typename P> inline Vector2<T> Vector2<T>::operator *(const P& v) {
		Vector2<T> res;
		res.x = (T)(this->x * v);
		res.y = (T)(this->y * v);
		return res;
	}

	// operator /
	template <typename T> template <typename P> inline Vector2<T> Vector2<T>::operator /(const P& v) {
		Vector2<T> res;
		res.x = (T)(this->x / v);
		res.y = (T)(this->y / v);
		return res;
	}

	// operator * Vector2<P>
	template <typename T> template <typename P> inline double Vector2<T>::operator *(const Vector2<P>& v) {
		double res = 0;
		res = (double)(this->x * v.x) + (double)(this->y * v.y);
		return res;
	}


	// operator +=
	template <typename T> template <typename P> inline void Vector2<T>::operator +=(const Vector2<P>& v) {
		this->x += v.x;
		this->y += v.y;
	}

	// operator -=
	template <typename T> template <typename P> inline void Vector2<T>::operator -=(const Vector2<P>& v) {
		this->x -= v.x;
		this->y -= v.y;
	}

	// operator *= Reelle Zahl
	template <typename T> template <typename P> inline void Vector2<T>::operator *=(const P& v) {
		this->x *= v;
		this->y *= v;
	}

	// operator /= Reelle Zahl
	template <typename T> template <typename P> inline void Vector2<T>::operator /=(const P& v) {
		this->x /= v;
		this->y /= v;
	}

	// operator ==
	template <typename T> template <typename P> inline bool Vector2<T>::operator ==(const Vector2<P>& v) {
		if(this->x == (T)v.x && this->y == (T)v.y)
			return true;
		return false;
	}

	// operator !=
	template <typename T> template <typename P> inline bool Vector2<T>::operator !=(const Vector2<P>& v) {
		if(this->x != (T)v.x || this->y != (T)v.y)
			return true;
		return false;
	}
}