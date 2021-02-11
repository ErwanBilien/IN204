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
        if (dot(normale,-vectDir)<0)normale=-normale;
        Color couleurLocale= listeObjets[indexPlusProche]->getColor()*CalculLuminosite(listeObjets,listeLumiere,PointIntersect,
            normale,-vectDir,listeObjets[indexPlusProche]->getShine()); //couleur avant la reflection
        
        double reflechissance=listeObjets[indexPlusProche]->getReflechissance();
        double transparence=listeObjets[indexPlusProche]->getTransparence();
        if (profondeurMax==0||(reflechissance<=0&&transparence<=0)){
            return couleurLocale;
        }
        else{
            Color couleurReflechie=Color();
            if(reflechissance>0){
                Rayon rayonReflechi(PointIntersect,2 * normale * dot(normale,-vectDir) + vectDir,myRay.getIndex());
                couleurReflechie=rayTracer(listeObjets,listeLumiere,rayonReflechi,profondeurMax-1);
            }
            Color couleurRefractee=Color();
            if(transparence>0){
                //indices refraction
                double n1,n2;
                int prochainIndex;
                if (myRay.getIndex()==-1)n1=1;  //le mileu actuel est l'air
                else n1=listeObjets[myRay.getIndex()]->getIndiceRefraction();

                if(myRay.getIndex()==indexPlusProche){//le rayon sort de l'objet
                    n2=1;
                    prochainIndex=-1;
                } 
                else{
                   n2=listeObjets[indexPlusProche]->getIndiceRefraction(); 
                   prochainIndex=indexPlusProche;
                } 

                double n=n1/n2;//indice du milieu ambiant/indice du mileu suivant
                vectDir=vectDir.normalize();
                //std::cout<<n<< " ";
                double cosThetaI=dot(normale,-vectDir);
                double sinThetaR2=n*n*(1.0-cosThetaI*cosThetaI );
                if (sinThetaR2<=1.0){
                    double cosThetaR=sqrt(1.0-sinThetaR2);
                    Rayon rayonRefracte(PointIntersect,n*vectDir+(n*cosThetaI-cosThetaR)*normale,prochainIndex);
                    //std::cout<<vectDir-n*vectDir+(n*cosThetaI-cosThetaR)*normale<<std::endl;
                    couleurRefractee=rayTracer(listeObjets,listeLumiere,rayonRefracte,profondeurMax-1);
                }
                else couleurRefractee=Color();
            }
            return couleurLocale*(1-reflechissance-transparence)+couleurReflechie*reflechissance+couleurRefractee*transparence;
        }
        

    }
    else {
        return Color();
    }
   
}


#endif

