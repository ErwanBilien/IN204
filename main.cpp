#include "camera.hpp"
#include "SaveBMP.hpp"
#include "fonctionRayTracer.hpp"
#include "sphere.hpp"
#include "plan.hpp"
#include "triangle.hpp"
#include <iostream>
#include <vector>
#include <omp.h>


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
    
    //creation de quelques matériaux par défaut (la couleur n'est pas incluse dans le materiau)
    //std::vector<double> materiau={brillance,reflechissance,transparance,indice de refraction}, reflechissance+transparence<=1.0
    std::vector<double> verre_1={50,0.1,0.7,1.03};
    std::vector<double> metal_1={10,0.4,0,1};
    std::vector<double> plastique_1={1000,0.05,0,1};
    std::vector<double> mirroir={10,0.9,0,1};

    //quelques couleurs standardes au format RGB
    Color bleu(0,0,1);
    Color rouge(1,0,0);
    Color jaune(1,1,0);
    Color rose(1,0,0.5);
    Color blanc(1,1,1);


    //Camera pointDeVue=Camera(float3(7,1,2),float3(0,-1.57,0)); 
    Camera pointDeVue=Camera(float3(-10,1,-8),float3(0,1.57/2,0)); 
    //Camera pointDeVue=Camera(float3(0,1,10),float3(0,3.14,0)); 
    //Camera pointDeVue=Camera(float3(0,10,2),float3(3.14/2,0,0)); 
    //Camera pointDeVue=Camera(float3(0,0,-4),float3(0,0,0)); 
    //Camera pointDeVue=Camera(float3(7,7,2),float3(0,-1.57,3.14/4.0)); 
    //Camera pointDeVue=Camera(float3(-7,7,2),float3(0,1.57,-3.14/4.0)); 
    //Camera pointDeVue=Camera(); 

    std::vector<Objet*> listeObjets;
    Sphere sphere_1=Sphere();
    Sphere sphere_2(1,float3(2,0,4),bleu,verre_1);
    Sphere sphere_3(1, float3(-2, 0, 4), Color(0, 1, 0),metal_1);
    Sphere sphere_4(5000,float3(0,-5001,0),jaune,1000,0.5);
    Sphere sphere_5(0.5,float3(-3.5,4,2),blanc,plastique_1);
    Plan plan_1(float3(1,-1,0),float3(0,-1,0),float3(0,-1,1),blanc,mirroir);
    Plan plan_2(float3(-4,0,0),float3(-4,-1,0),float3(-4.5,0,1),blanc,mirroir);
    Plan plan_3(float3(8,0,0),float3(8,-1,0),float3(7.5,0,1),blanc,mirroir);
    Solide tetra_1=tetraedre(float3(1,0,-5),float3(1,0,1),float3(0,1,-1),float3(-1,0,0),jaune,verre_1);
    Solide cube_1=parallelepipede(float3(),float3(3,0,0),float3(0,1,0),float3(1,0,1),rouge,metal_1);

    std::vector<Triangle> listeTri=std::vector<Triangle>();
    listeTri.push_back(Triangle(float3(0,0,1),float3(1,0,1),float3(0,1,1)));
    listeTri.push_back(Triangle(float3(1,0,1),float3(0,1,1),float3(-1,0,0)));
    listeTri.push_back(Triangle(float3(0,0,1),float3(0,1,1),float3(-1,0,0)));
    listeTri.push_back(Triangle(float3(0,0,1),float3(1,0,1),float3(-1,0,0)));
    Solide triangle=Solide(listeTri,jaune,plastique_1);

    /*std::vector<Triangle> liste=tetra_1.getListeTriangle();
    std::cout<<liste.size();*/

    listeObjets.push_back(&sphere_1);
    listeObjets.push_back(&sphere_2);
    listeObjets.push_back(&sphere_3);
    //listeObjets.push_back(&sphere_4);
    listeObjets.push_back(&sphere_5);
    listeObjets.push_back(&plan_1);
    //listeObjets.push_back(&plan_2);
    //listeObjets.push_back(&tetra_1);
    //listeObjets.push_back(&triangle);
    //listeObjets.push_back(&plan_3);
    listeObjets.push_back(&cube_1);

    std::vector<Lumiere*> listeLumiere;
    LumiereAmbiante lumiere_1(0.2);
    LumierePonctuelle lumiere_2(0.8,float3(0,5,2));
    LumierePonctuelle lumiere_4(0.6,float3(2,1,0));
    LumiereDirectionnelle lumiere_3(0.2,float3(1,4,4));
    listeLumiere.push_back(&lumiere_1);
    listeLumiere.push_back(&lumiere_2);
    listeLumiere.push_back(&lumiere_3);
    //listeLumiere.push_back(&lumiere_4);

    Rayon myRay(pointDeVue.getPosition(),float3());
    #pragma omp parallel for schedule(dynamic )
    for (int x =-nbPixelLargeur/2;x<nbPixelLargeur/2;x++) { 
        for(int y=-nbPixelHauteur/2;y<nbPixelHauteur/2;y++) {
            myRay.setDir((pointDeVue.coordPixel(x,y,nbPixelLargeur,nbPixelHauteur,largeurImage,hauteurImage,d)).normalize());//vecteur directeur du rayon

            Color couleurPixel = rayTracer(listeObjets,listeLumiere ,myRay,30);
            couleurPixel.check();
            listePixels[(y+ nbPixelHauteur / 2) * nbPixelHauteur + x+ nbPixelLargeur / 2].r = couleurPixel.getColorR();
            listePixels[(y + nbPixelHauteur / 2) * nbPixelHauteur + x+nbPixelLargeur / 2].g = couleurPixel.getColorG();
            listePixels[(y + nbPixelHauteur / 2) * nbPixelHauteur + x+ nbPixelLargeur / 2].b = couleurPixel.getColorB();
        }
    } 
    
    
    savebmp("raytracing_Image.bmp",nbPixelLargeur, nbPixelHauteur,  72, listePixels);
    free(listePixels);
}
