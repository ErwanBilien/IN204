#ifndef RAYTRACER
#define RAYTRACER
#include "Lumiere.hpp"


Color rayTracer(
    std::vector<std::shared_ptr<Objet>> listeObjets,
    std::vector<std::shared_ptr<Lumiere>> listeLumiere,
    Rayon myRay,
    int profondeurMax)
{
    int indexPlusProche = -1;//index du plus proche objet
    double dPlusProche;//distance du plus proche objet

    //determination de la plus proche intersection
    CalculIntersection(listeObjets,myRay,&indexPlusProche,&dPlusProche);

    if (indexPlusProche != -1){
        float3 vectDir=myRay.getDir();
        float3 PointIntersect=myRay.getOrigin()+dPlusProche*vectDir;
        float3 normale=listeObjets[indexPlusProche]->getNormal(PointIntersect);
        Color couleurObjet=listeObjets[indexPlusProche]->getColor();
        double intensiteRayon=myRay.getI();   

        //fixe la normale dans le "bon sens" (pointant vers l'observateur)     
        if (dot(normale,-vectDir.normalize())<0.00001)normale=-normale;
        
        //Calcul de la couleur locale
        Color couleurLocale= couleurObjet*CalculLuminosite(listeObjets,listeLumiere,PointIntersect,
            normale,-vectDir,listeObjets[indexPlusProche]->getShine()); 

        double reflechissance=listeObjets[indexPlusProche]->getReflechissance();
        double transparence=listeObjets[indexPlusProche]->getTransparence();

        //Est ce que le rayon peut encore contribuer à la couleur finale ?
        if (intensiteRayon<=0.01||profondeurMax==0||(reflechissance<=0&&transparence<=0)){
            return couleurLocale;
        }
        else{
            //Calcul de la contribution du rayon réfléchi
            Color couleurReflechie=Color();
                if(reflechissance>0.0){
                    float3 reflecDir=2 * normale * dot(normale,-vectDir) + vectDir;
                    Rayon rayonReflechi(PointIntersect,reflecDir,myRay.getIndex(),reflechissance*intensiteRayon);
                    couleurReflechie=rayTracer(listeObjets,listeLumiere,rayonReflechi,profondeurMax-1);
            }
            //Calcul de la contribuion du rayon réfracté
            Color couleurRefractee=Color();
            if(transparence>0.0){
                //indices refraction
                double n1,n2;
                int prochainIndex;// index de l'objet  duquel partira le rayon réfracté

                //détermination des indices optiques du milieu actuel et du milieu suivant 
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
                double cosThetaI=dot(normale,-vectDir);
                double sinThetaR2=n*n*(1.0-cosThetaI*cosThetaI );

                //Est ce qu il y a reflexion totale ?
                if (sinThetaR2<=1.0){
                    double cosThetaR=sqrt(1.0-sinThetaR2);
                    Rayon rayonRefracte(PointIntersect,n*vectDir+(n*cosThetaI-cosThetaR)*normale,prochainIndex,transparence*intensiteRayon);
                    couleurRefractee=rayTracer(listeObjets,listeLumiere,rayonRefracte,profondeurMax-1);
                }
                else couleurRefractee=Color();
            }
            return couleurLocale*(1-reflechissance-transparence)+couleurObjet*couleurReflechie*reflechissance+couleurObjet*couleurRefractee*transparence;
        }
        

    }
    else {
        return Color();
    }
   
}


#endif

