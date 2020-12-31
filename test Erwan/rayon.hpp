#include "float3.hpp"
#ifndef RAYON_H
#define RAYON_H
class Rayon {
protected:
	float3 Direction;
	float3 Origine;
public:
	Rayon() :Direction(float3()),Origine(float3()) {}
	Rayon(float3 DirVect,float3 OriVect) :Direction(DirVect),Origine(OriVect) {}
	//Rayon(double xValue, double yValue, double zValue) :Direction(float3(xValue, yValue, zValue)){}
	~Rayon() {}
	float3 getDir() {
		return Direction;
	}
	float3 getOrigin() {
		return Origine;
	}

	void setDir(float3 A,float3 B) { //calcule la direction du rayon passant par le point A puis par le point B
		Direction = (B - A).normalize();
	}
};
#endif