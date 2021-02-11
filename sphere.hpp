#ifndef SPHERE
#define  SPHERE
#include "objet.hpp"
#include <cmath>
class Sphere : public Objet {
protected:
    double rayon;
    float3 centre;
public:
    Sphere() : rayon(1), centre(float3(0, -1, 3)), Objet(Color(1, 0, 0),500,0.2,0) {}
    Sphere(double Rayon_S, float3 Centre,Color couleur,double brille,double reflech) :rayon(Rayon_S), centre(Centre), Objet(couleur,brille,reflech,0) {}
    Sphere(double Rayon_S, float3 Centre,Color couleur,double brille,double reflech,double transpa,double indiceRefraction) :rayon(Rayon_S), centre(Centre), Objet(couleur,brille,reflech,transpa,indiceRefraction) {}
    Sphere(double Rayon_S, float3 Centre,Color couleur,std::vector<double> materiau) :rayon(Rayon_S), centre(Centre), Objet(couleur,materiau) {}

    double Intersection(Rayon myRay) {
        float3 origine =myRay.getOrigin();
        float3 vectDir=myRay.getDir();
        float3 CO = origine - centre;
        double a = dot(vectDir, vectDir);
        double b = 2 * dot(CO, vectDir);
        double c = dot(CO, CO)  - rayon * rayon;

        double delta = b * b - 4 * a * c;
        if (delta < 0) {
            return -1;
        }
        double d1 = (-b + sqrt(delta)) / (2 * a);
        double d2 = (-b - sqrt(delta)) / (2 * a);
        if (d1 < 0 && d2 < 0) {
            return -1;
        }
        else if (d1 * d2 <= 0.0001) {
            if(std::max(d1, d2)>0.00001)return std::max(d1, d2);
            else return -1;
        }
        else {
            if(std::min(d1, d2)>0.00001)return std::min(d1, d2);
            else return -1;
        }
    }

    float3 getNormal(float3 M){
        return (M-centre).normalize();
    }
};
#endif