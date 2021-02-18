#ifndef TEXTURE
#define TEXTURE
#include "Lumiere.hpp"
class Texture{

public:
	Texture(){}
	virtual double CalculLuminosite(std::vector<Lumiere*>listeLumiere,float3 M,float3 normale,float3 V){
		return 0;
	}
};

class Brillant{
protected:
	double brillance;
public:
	Brillant():brillance(500){}
	Brillant(double brille):brillance(brille){}
	double CalculLuminosite(std::vector<Lumiere*>listeLumiere,float3 M,float3 normale,float3 V){ //calcul la luminosité au point M
    	double Luminosite=0.0;
    	std::cout<<"ok"<<" ";
    	for(int i=0;i<listeLumiere.size();i++){
    		//reflexion diffuse
        	Luminosite+=listeLumiere[i]->LuminositeReflexDiffuse(M,normale);
        	std::cout<<"ok"<<" ";
        	//relfexion speculaire
        	//Luminosite+=listeLumiere[i]->LuminositeReflexSpeculaire(M,normale,V,brillance);

    	}
    	return Luminosite;
	}
};

class Mat:public Texture{
public:
	Mat(){}
	double CalculLuminosite(std::vector<Lumiere*>listeLumiere,float3 M,float3 normale,float3 V){
    	double Luminosite=0.0;
    	for(int i=0;i<listeLumiere.size();i++){
        	Luminosite+=listeLumiere[i]->LuminositeReflexDiffuse(M,normale);
    	}
    	return Luminosite;
	}
};
#endif