#ifndef _FLOAT3_H
#define _FLOAT3_H
#include<math.h>
#include <assert.h>

class float3 {
protected:
	double x;
	double y;
	double z;
public:
	//Constructeur/Destructeur
	float3() :x(0), y(0), z(0) {}
	float3(double xValue, double yValue, double zValue) :x(xValue), y(yValue), z(zValue) {}
	~float3() {}

	double getX() {
		return x;
	}
	double getY() {
		return y;
	}
	double getZ() {
		return z;
	}
	double length() {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	double length_squared() {
		return pow(x, 2) + pow(y, 2) + pow(z, 2);
	}
	float3 normalize() {
		double norm = this->length();
		assert(norm != 0);
		return float3(x / norm, y / norm, z / norm);
	}

	friend float3 operator * (double, const float3);
	friend double distance(const float3, const float3);
	friend float3 cross(const float3, const float3);
	friend double dot(const float3, const float3);

	float3 operator+(const float3 anotherVect) const {
		return float3(x + anotherVect.x, y + anotherVect.y, z + anotherVect.z);
	}
	//produit terme a terme de deux vecteurs
	float3 operator*(const float3 anotherVect) const {
		return float3(x * anotherVect.x, y * anotherVect.y, z * anotherVect.z);
	}
	//Soustraction de 2 vecteurs
	float3 operator-(const float3 anotherVect) const {
		return float3(x - anotherVect.x, y - anotherVect.y, z - anotherVect.z);
	}
	//Opposé du vecteur
	float3 operator-() {
		return float3(-x, -y, -z);
	}
	//multiplication par un scalaire
	float3 operator* (double aValue) {
		return float3(aValue * x, aValue * y, aValue * z);
	}
	//division par un scalaire
	float3 operator/ (double aValue) {
		assert(aValue != 0);
		return float3(x / aValue, y / aValue, z / aValue);
	}
	bool operator ==(const float3 anotherVect) {
		return (x == anotherVect.x) && (y == anotherVect.y) && (z == anotherVect.z);
	}
	bool operator !=(const float3 anotherVect) {
		return (x != anotherVect.x) || (y != anotherVect.y) || (z != anotherVect.z);
	}
};
inline float3 operator* (double aValue, const float3 aVect) {
	return float3(aValue * aVect.x, aValue * aVect.y, aValue * aVect.z);
}
//distance euclidienne
inline double distance(const float3 aVect, const float3 anotherVect) {
	return sqrt(pow(aVect.x - anotherVect.x, 2) + pow(aVect.y - anotherVect.y, 2) + pow(aVect.z - anotherVect.z, 2));
}
//produit vectoriel
inline float3 cross(const float3 aVect, const float3 anotherVect) {
	return float3(aVect.y * anotherVect.z - aVect.z * anotherVect.y, aVect.z * anotherVect.x - aVect.x * anotherVect.z, aVect.x * anotherVect.y - aVect.y * anotherVect.x);
}
inline double dot(const float3 aVect, const float3 anotherVect) {
	return (aVect.x * anotherVect.x + aVect.y * anotherVect.y + aVect.z * anotherVect.z);
}
#endif