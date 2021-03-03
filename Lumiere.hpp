#ifndef LUMIERE
#define LUMIERE
#include "objet.hpp"
#include <cmath>
class Lumiere{
protected:
	double 	intensite;
	Color couleurLumiere;
public:
	Lumiere(double I): intensite(I),couleurLumiere(Color(1,1,1)){}
	Lumiere(double I,Color couleur): intensite(I),couleurLumiere(couleur){}

	double getI(){
		return intensite;
	}
	virtual bool Ombre(std::vector<Objet*> listeObjets,float3 M){
		return false;
	}
	virtual double LuminositeReflexDiffuse(float3 M,float3 normale){ 
		return 0;
	}
	virtual double LuminositeReflexSpeculaire(float3 M,float3 normale,float3 V,double brillance){
		return 0;
	}
	Color getColor(){
		return couleurLumiere;
	}
};

class LumiereAmbiante:public Lumiere{
public:
	LumiereAmbiante(double I): Lumiere(I){}
	LumiereAmbiante(double I,Color couleur): Lumiere(I,couleur){}

	double getI(){
		return intensite;
	}
	bool Ombre(std::vector<Objet*> listeObjets,float3 M){
		return false;
	}
	double LuminositeReflexDiffuse(float3 M,float3 normale){ //reflexion difuse
		return intensite;
	}
	double LuminositeReflexSpeculaire(float3 M,float3 normale,float3 V,double brillance){//reflexion speculaire
		return 0;
	}
};

class LumierePonctuelle:public Lumiere{
protected:
	float3 position;
public:
	LumierePonctuelle(double I,float3 pos):position(pos),Lumiere(I){}
	LumierePonctuelle(double I,float3 pos,Color couleur):position(pos),Lumiere(I,couleur){}

	float3 getPosition(){
		return position;
	}

	bool Ombre(std::vector<Objet*> listeObjets,float3 M){//renvoie true si la lumiere projete une ombre sur le point M
		float3 PM=position-M;
		int indexPlusProche=-1;
    	double dPlusProche;
		CalculIntersection(listeObjets,Rayon(M,PM.normalize()),&indexPlusProche,&dPlusProche);
		if (indexPlusProche==-1)return false;
		/*double transparence=listeObjets[indexPlusProche]->getTransparence();
		if (listeObjets[indexPlusProche]->getTransparence())*/
		return true;
	}

	double LuminositeReflexDiffuse(float3 M,float3 normale){//luminosité pour la reflexion diffuse
		float3 PM=position-M;
		double NormaleDotPM=dot(normale,PM);
		return std::max(intensite*NormaleDotPM/(normale.length()*PM.length()),0.0);
	}
	double LuminositeReflexSpeculaire(float3 M,float3 normale,float3 V,double brillance){//luminosité pour la reflexion spéculaire, V=rayon vers le point d'observation
		float3 PM=position-M;
		float3 rayonReflechi=2*normale*dot(PM,normale)-PM;
		if (dot(rayonReflechi,V)>0) return intensite*pow(dot(rayonReflechi,V)/(rayonReflechi.length()*V.length()),brillance);
		return 0;
	}

};

class Spot:public Lumiere{//lumiere ponctuelle emmettant dans un cone d angle 2*alpha
protected:
	float3 position;
	float3 direction;
	double alpha;
public:
	Spot(double I,float3 pos,float3 dir,double angle):position(pos),direction(dir),alpha(angle) ,Lumiere(I){}
	Spot(double I,float3 pos,float3 dir,double angle,Color couleur):position(pos),direction(dir),alpha(angle) ,Lumiere(I,couleur){}

	float3 getPosition(){
		return position;
	}

	bool Ombre(std::vector<Objet*> listeObjets,float3 M){//renvoie true si la lumiere projete une ombre sur le point M
		float3 PM=position-M;
		int indexPlusProche=-1;
    	double dPlusProche;
		CalculIntersection(listeObjets,Rayon(M,PM.normalize()),&indexPlusProche,&dPlusProche);
		if (indexPlusProche!=-1)return true;
		return false;
	}

	double LuminositeReflexDiffuse(float3 M,float3 normale){//luminosité pour la reflexion diffuse
		float3 PM=position-M;
		double NormaleDotPM=dot(normale,PM);
		double cosTheta=-dot(PM.normalize(),direction.normalize());
		if (cosTheta<cos(alpha))return 0;
		return std::max(intensite*NormaleDotPM/(normale.length()*PM.length()),0.0);
	}
	double LuminositeReflexSpeculaire(float3 M,float3 normale,float3 V,double brillance){//luminosité pour la reflexion spéculaire, V=rayon vers le point d'observation
		float3 PM=position-M;
		float3 rayonReflechi=2*normale*dot(PM,normale)-PM;
		double cosTheta=-dot(PM.normalize(),direction.normalize());
		if (cosTheta<cos(alpha))return 0;
		if (dot(rayonReflechi,V)>0) return intensite*pow(dot(rayonReflechi,V)/(rayonReflechi.length()*V.length()),brillance);
		return 0;
	}

};


class LumiereDirectionnelle:public Lumiere{
protected:
	float3 direction;
public:
	LumiereDirectionnelle(double I,float3 dir):direction(dir),Lumiere(I){}
	LumiereDirectionnelle(double I,float3 dir,Color couleur):direction(dir),Lumiere(I,couleur){}

	float3 getDirection(){
		return direction;
	}

	bool Ombre(std::vector<Objet*> listeObjets,float3 M){//renvoie true si la lumiere projete une ombre sur le point M
		int indexPlusProche=-1;
    	double dPlusProche;
		CalculIntersection(listeObjets,Rayon(M,direction.normalize()),&indexPlusProche,&dPlusProche);
		if (indexPlusProche==-1)return false;
		return true;
	}
	double LuminositeReflexDiffuse(float3 M,float3 normale){
		double NormaleDotDir=dot(normale,direction);
		return std::max(intensite*NormaleDotDir/(normale.length()*direction.length()),0.0);
	}
	double LuminositeReflexSpeculaire(float3 M,float3 normale,float3 V,double brillance){//luminosité pour la reflexion spéculaire, V=rayon vers le point d'observation
		float3 rayonReflechi=2*normale*dot(direction,normale)-direction;
		if (dot(rayonReflechi,V)>0) return intensite*pow(dot(rayonReflechi,V)/(rayonReflechi.length()*V.length()),brillance);
		return 0;
	}
};

Color CalculLuminosite(std::vector<Objet*> listeObjets,std::vector<Lumiere*>listeLumiere,float3 M,float3 normale,float3 V,double brillance){
    std::vector<double> Luminosite={0.0,0.0,0.0};
    for(int i=0;i<listeLumiere.size();i++){
        if (!listeLumiere[i]->Ombre(listeObjets,M)){
            //reflexion diffuse
            Luminosite[0]+=listeLumiere[i]->LuminositeReflexDiffuse(M,normale)*listeLumiere[i]->getColor().getColorR();
            Luminosite[1]+=listeLumiere[i]->LuminositeReflexDiffuse(M,normale)*listeLumiere[i]->getColor().getColorG();
            Luminosite[2]+=listeLumiere[i]->LuminositeReflexDiffuse(M,normale)*listeLumiere[i]->getColor().getColorB();
            //relfexion speculaire
            if(brillance>0){
            	Luminosite[0]+=listeLumiere[i]->LuminositeReflexSpeculaire(M,normale,V,brillance)*listeLumiere[i]->getColor().getColorR();
            	Luminosite[1]+=listeLumiere[i]->LuminositeReflexSpeculaire(M,normale,V,brillance)*listeLumiere[i]->getColor().getColorG();
            	Luminosite[2]+=listeLumiere[i]->LuminositeReflexSpeculaire(M,normale,V,brillance)*listeLumiere[i]->getColor().getColorB();
            }
        }
    }
    return Color(Luminosite);
}

#endif