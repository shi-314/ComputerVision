// ========================================================== //
// Vector3
// - Template-Klasse für 3 dimensionale Vektoren mit untersch.
//   Datentypen für die einzelnen Elemente (double, int, float..)
//
// Hinweis: Die Datei darf vom Compiler nicht automatisch mit-
//			kompiliert werden, da sonst unresolved externals
//			auftreten. Diese Datei wird in Vector3.h includet.
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 27.02.2011
// ========================================================== //

#include "Vector3.h"

using namespace std;

namespace ComputerVision {

	// ========================================================== //
	// Vector3
	// ========================================================== //
	template <typename T> inline Vector3<T>::Vector3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	template <typename T> inline Vector3<T>::Vector3(T x, T y, T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template <typename T> Vector3<T>::~Vector3() {
	}

	// ========================================================== //
	// length
	// ========================================================== //
	template <typename T> inline double Vector3<T>::length() {
		return sqrt((double)(this->x*this->x) + (double)(this->y*this->y) + (double)(this->z*this->z));
	}

	// ========================================================== //
	// normalize (Vorsicht: Macht bei Vector3<int> keinen Sinn,
	// da immer 0 raus kommt!)
	// ========================================================== //
	template <typename T> void Vector3<T>::normalize() {
		double len = this->length();
		if(len > 0) {
			this->x = (T)(this->x/len);
			this->y = (T)(this->y/len);
			this->z = (T)(this->z/len);
		} else {
			throw exception("Vector3<T>::normalize() -- division by zero");
		}
	}

	// ========================================================== //
	// distance
	// ========================================================== //
	template <typename T> template <typename P> inline double Vector3<T>::distance(Vector3<P> v) {
		v-=(Vector3<T>)(*this);
		return abs( v.length() );
	}

	// ========================================================== //
	// crossProduct
	// ========================================================== //
	template <typename T> inline Vector3<T> Vector3<T>::crossProduct(Vector3<T> v) {
		return Vector3<T>(this->y*v.z - this->z*v.y, 
						  this->z*v.x - this->x*v.z,
						  this->x*v.y - this->y*v.x);
	}

	// ========================================================== //
	// angle
	// ========================================================== //
	template <typename T> template <typename P> inline double Vector3<T>::angle(Vector3<P> v) {
		// cos a = |m1*m2|/(|m1|*|m2|)
		double cos_a = abs(*this*v)/(this->length()*v.length());
		return acos(cos_a);
	}

	// ========================================================== //
	// O P E R A T O R S
	// ========================================================== //

	// operator =
	template <typename T> template <typename P> inline void Vector3<T>::operator =(const Vector3<P>& v) {
		this->x = (T)v.x;
		this->y = (T)v.y;
		this->z = (T)v.z;
	}

	// operator +
	template <typename T> template <typename P> inline Vector3<T> Vector3<T>::operator +(const Vector3<P>& v) {
		Vector3<T> res;
		res.x = (T)(this->x + v.x);
		res.y = (T)(this->y + v.y);
		res.z = (T)(this->z + v.z);
		return res;
	}

	// operator -
	template <typename T> template <typename P> inline Vector3<T> Vector3<T>::operator -(const Vector3<P>& v) {
		Vector3<T> res;
		res.x = (T)(this->x - v.x);
		res.y = (T)(this->y - v.y);
		res.z = (T)(this->z - v.z);
		return res;
	}

	// operator *
	template <typename T> template <typename P> inline Vector3<T> Vector3<T>::operator *(const P& v) {
		Vector3<T> res;
		res.x = (T)(this->x * v);
		res.y = (T)(this->y * v);
		res.z = (T)(this->z * v);
		return res;
	}

	// operator /
	template <typename T> template <typename P> inline Vector3<T> Vector3<T>::operator /(const P& v) {
		Vector3<T> res;
		res.x = (T)(this->x / v);
		res.y = (T)(this->y / v);
		res.z = (T)(this->z / v);
		return res;
	}

	// operator * Vector3<P> = Skalarprodukt
	template <typename T> template <typename P> inline double Vector3<T>::operator *(const Vector3<P>& v) {
		double res = 0;
		res = (double)(this->x * v.x) + (double)(this->y * v.y) + (double)(this->z * v.z);
		return res;
	}


	// operator +=
	template <typename T> template <typename P> inline void Vector3<T>::operator +=(const Vector3<P>& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}

	// operator -=
	template <typename T> template <typename P> inline void Vector3<T>::operator -=(const Vector3<P>& v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
	}

	// operator *= Reelle Zahl
	template <typename T> template <typename P> inline void Vector3<T>::operator *=(const P& v) {
		this->x *= v;
		this->y *= v;
		this->z *= v;
	}

	// operator /= Reelle Zahl
	template <typename T> template <typename P> inline void Vector3<T>::operator /=(const P& v) {
		this->x /= v;
		this->y /= v;
		this->z /= v;
	}

	// operator ==
	template <typename T> template <typename P> inline bool Vector3<T>::operator ==(const Vector3<P>& v) {
		if(this->x == (T)v.x && this->y == (T)v.y && this->z == (T)v.z)
			return true;
		return false;
	}

	// operator !=
	template <typename T> template <typename P> inline bool Vector3<T>::operator !=(const Vector3<P>& v) {
		if(this->x != (T)v.x || this->y != (T)v.y || this->z != (T)v.z)
			return true;
		return false;
	}
}