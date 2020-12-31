#include "color.hpp"
#include "objet.hpp"
#include"rayon.hpp"
#include "pixels.hpp"
#include <iostream>
//#include "moteur.hpp"
#include"SaveBMP.hpp"
int main() {

	float3 Camera = float3(5, 5, 5);
	//Objet liste_objet[1];
	Objet sphere_1 = Objet();
	//liste_objet[0] = sphere_1;
	int N_objet = 1;


	const int Hauteur = 10;
	const int Largeur = 10;

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
				//findIntersection renvoi la distance entre la camera et le point dintersection si le rayon intersecte l'objet, -1 sinon
				/*Distance = liste_objet[k].findIntersection(MyRay);
				if ((Distance < 0) && ((ClosestInstersect == -1) || (D_ClosestIntersect > Distance))) {
					ClosestInstersect = k;
					D_ClosestIntersect = Distance;*/
				Distance = sphere_1.findIntersection(MyRay);
				if ((Distance < 0) && ((ClosestInstersect == -1) || (D_ClosestIntersect > Distance))) {
					ClosestInstersect = k;
					D_ClosestIntersect = Distance;
				}
			}
			//liste_pixels[i * Largeur + j].setColor(liste_objet[ClosestInstersect].object_color);
			if (ClosestInstersect == -1) {
				liste_pixels[i * Largeur + j].setColor(Color(0,0,0));
			}
			else {
				liste_pixels[i * Largeur + j].setColor(sphere_1.object_color);
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
	savebmp("/mnt/c/Users/Erwan/Documents/perso/ensta/2A/IN204/Projet_IN204/raytracing_Image.bmp", Largeur, Hauteur, 72, pixels);
	return 0;
}