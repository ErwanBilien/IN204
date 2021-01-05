#include "rayon.hpp"
#include "float3.hpp"
#include "objet.hpp"
#include <iostream>

//sphere
class Sphere: public Objet{
private:
	float3 centre;
	double rayon;
public:
	Sphere():centre(float3(5, 5, -5)),rayon(5),Objet(Color(1, 0, 1)){}
	Sphere(float3 centre_s,double rayon_s,Color couleur_s):centre(centre_s),rayon(rayon_s),Objet(couleur_s){}
	
	virtual double findIntersection(Rayon ray) {
		//std::cout<<"ouui";
		double delta = pow(dot(ray.getDir(), ray.getOrigin() - centre), 2) - (ray.getOrigin() - centre).length_squared() + rayon * rayon;
		if (delta < 0) {
			return -1;
		}
		else {
			return 1;
		}
	}
	
	
};
