#include "rayon.hpp"
#include "float3.hpp"
#ifndef OBJET_H
#define OBJET_H
//fichier juste pour le test

//sphere
class Objet {
protected:
	Color couleur;
public:
	Objet():couleur(Color(0,0,1)){}
	Objet(Color couleur_o):couleur(couleur_o){}
	Color getObjectColor(){
		return couleur;
	}
	virtual double findIntersection(Rayon Ray){return 0;}
};
#endif
