#ifndef RAYON
#define RAYON
#include "float3.hpp"
class Rayon{
protected: 
	float3 origine;
	float3 direction;
public:
	Rayon(float3 O,float3 D):origine(O),direction(D){};
	float3 getDir(){
		return direction;
	}
	float3 getOrigin(){
		return origine;
	}
	void setDir(float3 vect){
		direction=vect;
	}
};
#endif