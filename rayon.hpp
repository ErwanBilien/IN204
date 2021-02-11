#ifndef RAYON
#define RAYON
#include "float3.hpp"
class Rayon{
protected: 
	float3 origine;
	float3 direction;
	int indexObjet;//index de l'objet dans lequle est le rayon, -1 si dans l'air
public:
	Rayon(float3 O,float3 D):origine(O),direction(D),indexObjet(-1){};
	Rayon(float3 O,float3 D,int index):origine(O),direction(D),indexObjet(index){};
	float3 getDir(){
		return direction;
	}
	float3 getOrigin(){
		return origine;
	}
	int getIndex(){
		return indexObjet;
	}
	void setDir(float3 vect){
		direction=vect;
	}
};
#endif