#include "camera.hpp"
#include "SaveBMP.hpp"
#include "fonctionRayTracer.hpp"
#include "sphere.hpp"
#include "plan.hpp"
#include "scene.hpp"
#include "triangle.hpp"
#include "image.hpp"
#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>
#include <chrono>

char pathToScene[] = "sceneDescription2.xml";

int main( int argc, char *argv[] )
{
    std::cout << "loading" << std::endl; 

    Picture picture = readPictureFromXML(pathToScene);
    const int nbPixelLargeur = picture.getWidth();
    const int nbPixelHauteur = picture.getHeight();

    double d = readCameraToPictureDistanceFromXML(pathToScene); //distance entre la caméra et l'image, d=1 correspond à un angle de vue d'environ 53°
    double largeurImage;
    double hauteurImage ;
    if (nbPixelLargeur>nbPixelHauteur){
        largeurImage = 1.0;
        hauteurImage = (double)nbPixelHauteur/ (double)nbPixelLargeur;
    }
    else{
        largeurImage = (double)nbPixelLargeur/(double)nbPixelHauteur;
        hauteurImage = 1.0;
    }
    
    RGBType* listePixels = (RGBType*)malloc(sizeof(RGBType) * nbPixelHauteur * nbPixelLargeur);
    if (!listePixels) {
        std::cout << "echec" << std::endl;
        return -1;
    }

    Camera pointDeVue = readCameraFromXML(pathToScene); 

    std::vector<std::shared_ptr<Objet>> listeObjets = readObjectsFromXML(pathToScene);

    std::vector<std::shared_ptr<Lumiere>> listeLumiere = readLightsFromXML(pathToScene);

    std::cout << "rendering" << std::endl;    

    std::chrono::time_point<std::chrono::system_clock> start, end1, end2;
    start = std::chrono::system_clock::now();
    Rayon myRay(pointDeVue.getPosition(),float3());
#pragma omp parallel for schedule(dynamic)
    for (int x =-nbPixelLargeur/2;x<nbPixelLargeur/2;x++) { 
        for(int y=-nbPixelHauteur/2;y<nbPixelHauteur/2;y++) {

            myRay.setDir((pointDeVue.coordPixel(x,y,nbPixelLargeur,nbPixelHauteur,largeurImage,hauteurImage,d)).normalize());//vecteur directeur du rayon

            Color couleurPixel = rayTracer(listeObjets,listeLumiere ,myRay,30);
            couleurPixel.check();
            listePixels[(y+ nbPixelHauteur / 2) * nbPixelLargeur + x+ nbPixelLargeur / 2].r = couleurPixel.getColorR();
            listePixels[(y + nbPixelHauteur / 2) * nbPixelLargeur + x+nbPixelLargeur / 2].g = couleurPixel.getColorG();
            listePixels[(y + nbPixelHauteur / 2) * nbPixelLargeur + x+ nbPixelLargeur / 2].b = couleurPixel.getColorB();
        }
    } 
    end1 = std::chrono::system_clock::now();
    std::chrono::duration<double> elaps1 = end1 - start;
    double temps_calcul=elaps1.count();
    std::cout<<"temps de calcul : "<<temps_calcul<<std::endl;

    
    savebmp("raytracing_Image.bmp",nbPixelLargeur, nbPixelHauteur,  72, listePixels);
    free(listePixels);
}
