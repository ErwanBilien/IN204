#ifndef RAYTRACER
#define RAYTRACER
#include "float3.hpp"
#include "color.hpp"
#include "objet.hpp"
#include "Lumiere.hpp"




Color rayTracer(std::vector<Objet*> listeObjets,std::vector<Lumiere*>listeLumiere,Rayon myRay,int profondeurMax) {
    int indexPlusProche = -1;//index du plus proche objet
    double dPlusProche;//distance du plus proche objet
    CalculIntersection(listeObjets,myRay,&indexPlusProche,&dPlusProche);
    if (indexPlusProche != -1){
        float3 vectDir=myRay.getDir();
        float3 PointIntersect=myRay.getOrigin()+dPlusProche*vectDir;
        float3 normale=listeObjets[indexPlusProche]->getNormal(PointIntersect);
        Color couleurLocale= listeObjets[indexPlusProche]->getColor()*CalculLuminosite(listeObjets,listeLumiere,PointIntersect,
            normale,-vectDir,listeObjets[indexPlusProche]->getShine()); //couleur avant la reflection
        
        double reflechissance=listeObjets[indexPlusProche]->getReflechissance();
        if (profondeurMax==0||reflechissance<=0){
            return couleurLocale;
        }
        else{
            Rayon rayonReflechi(PointIntersect,2 * normale * dot(normale,-vectDir) + vectDir);
            Color couleurReflechie=rayTracer(listeObjets,listeLumiere,rayonReflechi,profondeurMax-1);
            return couleurLocale*(1-reflechissance)+couleurReflechie*reflechissance;
        }
    }
    else {
        return Color();
    }
   
}


#endif

