#include "float3.hpp"
#include "color.hpp"
#include "SaveBMP.hpp"
#include "fonctionRayTracer.hpp"
#include "sphere.hpp"
#include "Lumiere.hpp"
#include <iostream>
#include <vector>


int main() {
    const int nbPixelLargeur = 500;
    const int nbPixelHauteur = 500;
    double d = 1.0; //distance entre la caméra et l'image
    double largeurImage = 1.0;
    double hauteurImage = 1.0;
    RGBType* listePixels = (RGBType*)malloc(sizeof(RGBType) * nbPixelHauteur * nbPixelLargeur);
    if (!listePixels) {
        std::cout << "echec" << std::endl;
        return -1;
    }
    
    float3 Camera(0, 0, -1); 

    std::vector<Objet*> listeObjets;
    Sphere sphere_1=Sphere();
    Sphere sphere_2(1,float3(2,0,4),Color(0,0,1),500,0.3);
    Sphere sphere_3(1, float3(-2, 0, 4), Color(0, 1, 0),10,0.4);
    Sphere sphere_4(5000,float3(0,-5001,0),Color(1,1,0),1000,0.5);
    listeObjets.push_back(&sphere_1);
    listeObjets.push_back(&sphere_2);
    listeObjets.push_back(&sphere_3);
    listeObjets.push_back(&sphere_4);

    std::vector<Lumiere*> listeLumiere;
    LumiereAmbiante lumiere_1(0.2);
    LumierePonctuelle lumiere_2(0.6,float3(2,1,0));
    LumiereDirectionnelle lumiere_3(0.2,float3(1,4,4));
    listeLumiere.push_back(&lumiere_1);
    listeLumiere.push_back(&lumiere_2);
    listeLumiere.push_back(&lumiere_3);

    Rayon myRay(Camera,float3());
    for (int x =-nbPixelLargeur/2;x<nbPixelLargeur/2;x++) { 
        for(int y=-nbPixelHauteur/2;y<nbPixelHauteur/2;y++) {
            myRay.setDir((float3(x * largeurImage / (double)nbPixelLargeur, y * hauteurImage / (double)nbPixelHauteur, d+Camera.getZ())-Camera).normalize());//vecteur directeur du rayon

            Color couleurPixel = rayTracer(listeObjets,listeLumiere ,myRay,5);
            couleurPixel.check();
            listePixels[(y+ nbPixelHauteur / 2) * nbPixelHauteur + x+ nbPixelLargeur / 2].r = couleurPixel.getColorR();
            listePixels[(y + nbPixelHauteur / 2) * nbPixelHauteur + x+nbPixelLargeur / 2].g = couleurPixel.getColorG();
            listePixels[(y + nbPixelHauteur / 2) * nbPixelHauteur + x+ nbPixelLargeur / 2].b = couleurPixel.getColorB();
        }

    } 
    
    
    savebmp("raytracing_Image.bmp",nbPixelLargeur, nbPixelHauteur,  72, listePixels);
    free(listePixels);
}
