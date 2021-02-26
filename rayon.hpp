#ifndef RAYON
#define RAYON
#include "float3.hpp"
class Rayon{
protected: 
	float3 origine;
	float3 direction;
	int indexObjet;//index de l'objet dans lequle est le rayon, -1 si dans l'air
	double intensite;// utilisé pour limiter le nombre de recursions, 0<=intensité<=1
public:
	Rayon(float3 O,float3 D):origine(O),direction(D),indexObjet(-1),intensite(1){};
	Rayon(float3 O,float3 D,int index,double I):origine(O),direction(D),indexObjet(index),intensite(I){};
	float3 getDir(){
		return direction;
	}
	float3 getOrigin(){
		return origine;
	}
	int getIndex(){
		return indexObjet;
	}
	double getI(){
		return intensite;
	}
	void setDir(float3 vect){
		direction=vect;
	}
};
#endif