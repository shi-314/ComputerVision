// ========================================================== //
// Vector2
// - Template-Klasse für 2 dimensionale Vektoren mit untersch.
//   Datentypen für die einzelnen Elemente (double, int, float..)
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 06.02.2011
// ========================================================== //

#ifndef _VECTOR_2_H
#define _VECTOR_2_H

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;

	template <typename T=double> class Vector2 {
		public:
			T x;
			T y;

			inline Vector2();
			inline Vector2(T x, T y);
			~Vector2();

			// Gibt die Länge des Vektors als double zurück
			inline double length();

			// Normalisiert den Vektor
			void normalize();

			// Absolute Distanz zum anderen Vektor
			template <typename P> inline double distance(Vector2<P> v);

			// Kreuzprodukt des Vektors (Orthogonale)
			inline Vector2<T> crossProduct();

			// Winkel zwischen diesem Vektor und v in Radiant/Bogenmaß
			template <typename P> inline double angle(Vector2<P> v);

			// Operatoren überladen (das meißte inline)
			// Rechenoperatoren ----------------------------------------------

			// Wertzuweisung
			template <typename P> inline void operator =(const Vector2<P>& v);

			// Gibt als Datentyp A zurück wenn A+B oder B bei B+A !
			template <typename P> inline Vector2<T> operator +(const Vector2<P>& v);
			// Gibt als Datentyp A zurück wenn A-B oder B bei B-A !
			template <typename P> inline Vector2<T> operator -(const Vector2<P>& v);
			
			// Multiplizieren mit einer reellen Zahl
			template <typename P> inline Vector2<T> operator *(const P& v);
			// Dividieren durch eine reelle Zahl
			template <typename P> inline Vector2<T> operator /(const P& v);
			
			// Skalarprodukt (gibt double zurück)
			template <typename P> inline double operator *(const Vector2<P>& v);

			// Zwei Vektoren addieren
			template <typename P> inline void operator +=(const Vector2<P>& v);
			// Zwei Vektoren subtrahieren
			template <typename P> inline void operator -=(const Vector2<P>& v);
			
			// Multiplizieren mit einer reellen Zahl
			template <typename P> inline void operator *=(const P& v);
			// Dividieren durch eine reellen Zahl
			template <typename P> inline void operator /=(const P& v);

			// Vergleichsopertoren -------------------------------------------
			// Werte von Vektor A == Werte von Vektor B
			template <typename P> inline bool operator ==(const Vector2<P>& v);
			template <typename P> inline bool operator !=(const Vector2<P>& v);
	};
}


#include "Vector2.cpp"

// Standard Datentypen, die man immer braucht...
typedef ComputerVision::Vector2<int> Vec2;
typedef ComputerVision::Vector2<double> Vec2d;
typedef ComputerVision::Vector2<float> Vec2f;

#endif