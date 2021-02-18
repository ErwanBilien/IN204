#include "texture.hpp"
#ifndef BRILLANT
#define BRILLANT
class Brillant: public Texture{
protected:
	double brillance;
public:
	Brillant():brillance(500){}
	Brillant(double Brillance):brillance(Brillance){}
	 double CalculLuminosite(std::vector<Lumiere*>listeLumiere,float3 M,float3 normale,float3 V)override{ //calcul la luminosité au point M
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
#endif