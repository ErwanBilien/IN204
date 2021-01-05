#include "color.hpp"
#include "objet.hpp"
#include"rayon.hpp"
#include "pixels.hpp"
#include "sphere.hpp"
#include <iostream>
#include <vector>
#include "objet.hpp"
//#include "moteur.hpp"
#include"SaveBMP.hpp"
int main() {
	
	float3 Camera = float3(5, 5, 2);
	std::vector<Objet*> liste_objet;
	Sphere sphere_1 = Sphere();
	Sphere sphere_2=Sphere(float3(12, 0, -5),2,Color(1, 0, 0));
	liste_objet.push_back(&sphere_1);
	liste_objet.push_back(&sphere_2);
	int N_objet = liste_objet.size();
	


	const int Hauteur = 500;
	const int Largeur = 500;

	Rayon MyRay(float3(), Camera);
	Pixel* liste_pixels=new Pixel[Hauteur * Largeur];

	int ClosestInstersect = -1; //indice de l'objet le plus proche intersecté par le rayon, -1 si il n y a pas d intersection
	double D_ClosestIntersect = -1;// distance de l'intersection la plus proche
	double Distance;
	for(int i = 0; i < Largeur; i++) {
		for (int j = 0; j < Hauteur; j++) {
			liste_pixels[i * Largeur + j] = Pixel(i, j);
			ClosestInstersect = -1;
			MyRay.setDir(Camera, liste_pixels[i * Largeur + j].getCoord(Largeur, Hauteur));
			for (int k = 0; k < N_objet; k++) {
				Distance = liste_objet[k]->findIntersection(MyRay);
				//if (Distance>0)std::cout<<Distance<<std::endl;
				if ((Distance >= 0) && ((ClosestInstersect == -1) || (D_ClosestIntersect > Distance))) {
					ClosestInstersect = k;
					D_ClosestIntersect = Distance;
				}
			}
			if (ClosestInstersect == -1) {
				liste_pixels[i * Largeur + j].setColor(Color(0,0,0));
			}
			else {
				liste_pixels[i * Largeur + j].setColor(liste_objet[ClosestInstersect]->getObjectColor());
			}
			
		}
	}

	
	//Pixel* liste_pixels = raytracer(/*width, height,*/  liste_objet, 1, float3(5,5,5));
	RGBType pixels[Largeur*Hauteur];
	for (int i = 0; i < Largeur * Hauteur; i++) {
		pixels[i].r = liste_pixels[i].getPixelColor().getColorR();
		pixels[i].g = liste_pixels[i].getPixelColor().getColorG();
		pixels[i].b = liste_pixels[i].getPixelColor().getColorB();
	}
	savebmp("raytracing_Image.bmp", Largeur, Hauteur, 72, pixels);
	return 0;
}