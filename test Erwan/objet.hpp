#include "rayon.hpp"
#include "float3.hpp"
//fichier juste pour le test

//sphere
class Objet {
public:
	Objet(){}
	Color object_color=Color(1, 0, 0);
	float3 centre=float3(5, 5, -5);
	double rayon_c = 5;
	double findIntersection(Rayon ray) {
		double delta = pow(dot(ray.getDir(), ray.getOrigin() - centre), 2) - (ray.getOrigin() - centre).length_squared() + rayon_c * rayon_c;
		if (delta < 0) {
			return -1;
		}
		else {
			return 1;
		}
	}
	Color getObjectColor() {
		return object_color;
	}
	
};
