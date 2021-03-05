#ifndef MATERIAL
#define MATERIAL
#include <string>

class Material {
protected:
    std::string matName;
	double brightness;
	double reflectivity;
	double transparency;
	double refraction;

public:
    // le cast vers string est-il nécessaire ?
	Material() : matName("Unknown material"), brightness(0), reflectivity(0), transparency(0), refraction(1){}
	Material(std::string name, double bright, double reflect, double transpa, double index) :
        matName(name), brightness(bright), reflectivity(reflect), transparency(transpa), refraction(index){}
	
    std::string getMatName(){
        return matName;
    }
    double getBrightness(){
    	return brightness;
    }
    double getReflectivity(){
    	return reflectivity;
    }
    double getTransparency(){
    	return transparency;
    }
    double getRefraction(){
    	return refraction;
    }
};

#endif