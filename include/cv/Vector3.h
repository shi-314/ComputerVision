// ========================================================== //
// Vector3
// - Template-Klasse für 3 dimensionale Vektoren mit untersch.
//   Datentypen für die einzelnen Elemente (double, int, float..)
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 27.02.2011
// ========================================================== //

#ifndef _VECTOR_3_H
#define _VECTOR_3_H

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;

	template <typename T=double> class Vector3 {
		public:
			T x;
			T y;
			T z;

			inline Vector3();
			inline Vector3(T x, T y, T z);
			~Vector3();

			// Gibt die Länge des Vektors als double zurück
			inline double length();

			// Normalisiert den Vektor
			void normalize();

			// Absolute Distanz zum anderen Vektor
			template <typename P> inline double distance(Vector3<P> v);

			// Kreuzprodukt des Vektors (Orthogonale)
			inline Vector3<T> crossProduct(Vector3<T> v);

			// Winkel zwischen diesem Vektor und v in Radiant/Bogenmaß
			template <typename P> inline double angle(Vector3<P> v);

			// Operatoren überladen (das meißte inline)
			// Rechenoperatoren ----------------------------------------------

			// Wertzuweisung
			template <typename P> inline void operator =(const Vector3<P>& v);

			// Gibt als Datentyp A zurück wenn A+B oder B bei B+A !
			template <typename P> inline Vector3<T> operator +(const Vector3<P>& v);
			// Gibt als Datentyp A zurück wenn A-B oder B bei B-A !
			template <typename P> inline Vector3<T> operator -(const Vector3<P>& v);
			
			// Multiplizieren mit einer reellen Zahl
			template <typename P> inline Vector3<T> operator *(const P& v);
			// Dividieren durch eine reelle Zahl
			template <typename P> inline Vector3<T> operator /(const P& v);
			
			// Skalarprodukt (gibt double zurück)
			template <typename P> inline double operator *(const Vector3<P>& v);

			// Zwei Vektoren addieren
			template <typename P> inline void operator +=(const Vector3<P>& v);
			// Zwei Vektoren subtrahieren
			template <typename P> inline void operator -=(const Vector3<P>& v);
			
			// Multiplizieren mit einer reellen Zahl
			template <typename P> inline void operator *=(const P& v);
			// Dividieren durch eine reellen Zahl
			template <typename P> inline void operator /=(const P& v);

			// Vergleichsopertoren -------------------------------------------
			// Werte von Vektor A == Werte von Vektor B
			template <typename P> inline bool operator ==(const Vector3<P>& v);
			template <typename P> inline bool operator !=(const Vector3<P>& v);
	};
}


#include "Vector3.cpp"

typedef ComputerVision::Vector3<int> Vec3;
typedef ComputerVision::Vector3<double> Vec3d;
typedef ComputerVision::Vector3<float> Vec3f;

#endif