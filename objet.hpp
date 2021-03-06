#ifndef OBJET
#define OBJET
#include "color.hpp"
#include "rayon.hpp"
#include "material.hpp"
#include <memory>
class Objet {//classe parente de tout les objets
protected:
	Color couleurObjet;
	double brillance;
	double reflechissance; //0: ne reflechie pas; 1: l'objet est un mirroir parfait
	double transparence; //0: opaque, 1: transparent attention : transparence+brillance<=1
	double indiceRefraction;//indice optique

public:
	Objet() :couleurObjet(Color()),brillance(0),reflechissance(0),transparence(0),indiceRefraction(1.5) {}
	Objet(Color aColor) :couleurObjet(aColor),brillance(0),reflechissance(0),transparence(0),indiceRefraction(1.5) {}
	Objet(Color aColor,double brille,double reflech,double transpa) :
        couleurObjet(aColor),brillance(brille),reflechissance(reflech),transparence(transpa),indiceRefraction(1.5) {}
	Objet(Color aColor,double brille,double reflech,double transpa,double indice) :
        couleurObjet(aColor),brillance(brille),reflechissance(reflech),transparence(transpa),indiceRefraction(indice) {}

    virtual double Intersection(Rayon MyRay) {//retourne la distance de la plus proche intersection entre l'origine de MyRay et l'objet, -1 sinon
		return 0;
	}
	Color getColor() {
		return couleurObjet;
	}

	virtual float3 getNormal(float3 M){
        return float3();
    }
    double getShine(){
    	return brillance;
    }
    double getReflechissance(){
    	return reflechissance;
    }
    double getTransparence(){
    	return transparence;
    }
    double getIndiceRefraction(){
    	return indiceRefraction;
    }
};

    //calcule l'index et la distance du plus proche objet intersectant le rayon
void CalculIntersection(std::vector<std::shared_ptr<Objet>> listeObjets, Rayon myRay, int* indexPlusProche, double* dPlusProche)
{
    *indexPlusProche = -1;
    double d;
    for (int i = 0; i < (int)listeObjets.size(); i++) {
        d = listeObjets[i]->Intersection(myRay);
        if (d > 0 && (*indexPlusProche == -1 || d < *dPlusProche)) {
            *indexPlusProche = i;
            *dPlusProche = d;
        }
    }
}


#endif