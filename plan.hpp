#ifndef PLAN
#define PLAN
#include "objet.hpp"
class Plan:public Objet{
protected:
	float3 point;// point par lequel passe le plan
	float3 normale;//normale du plan
public:
	Plan():point(float3()),normale(float3(0,1,0)),Objet(){}
	//definition avec un point et une normale
	Plan(float3 A,float3 N,Color couleur,double brille,double reflech,double transpa,double indiceRefraction):point(A),normale(N) ,Objet(couleur,brille,reflech,transpa ,indiceRefraction){
		assert(N.length()>0.00001);//normale non nulle
	}
	Plan(float3 A,float3 N,Color couleur,std::vector<double> materiau):point(A),normale(N.normalize()) ,Objet(couleur,materiau){
		assert(N.length()>0.00001);//normale non nulle
	}
	//definition avec 3 points
	Plan(float3 A,float3 B,float3 C,Color couleur,double brille,double reflech,double transpa,double indiceRefraction):point(A),normale(cross(B-A,C-A).normalize()),Objet(couleur,brille,reflech,transpa ,indiceRefraction){
		assert(cross(B-A,C-A).length()>0.0001);//vecteurs directeurs non colineaires
	}
	Plan(float3 A,float3 B,float3 C,Color couleur,std::vector<double> materiau):point(A),normale((cross(B-A,C-A)).normalize()),Objet(couleur,materiau){
		assert(cross(B-A,C-A).length()>0.0001);//vecteurs directeurs non colineaires
	}
	virtual double Intersection(Rayon myRay) {
		float3 vectDir=myRay.getDir().normalize();
		double DirDotN=dot(vectDir,normale);
		if (abs(DirDotN)<0.0001){//pas d'intersection
			return -1;
		}
		double d= dot(point-myRay.getOrigin(),normale)/DirDotN;
		if(d<0.00001)return -1;//le plan est derriere l'origine du rayon
		return d;
	}
	float3 getNormal(float3 M){
		
        return normale;
    }
};
#endif