#ifndef TRIANGLE
#define TRIANGLE
#include "objet.hpp"
class Triangle{
protected:
	float3 sommet1;//sommets du triangle
	float3 sommet2;
	float3 sommet3;
	float3 Normale;
public:
	Triangle(float3 s1,float3 s2,float3 s3):sommet1(s1),sommet2(s2),sommet3(s3),Normale(cross(sommet2-sommet1,sommet3-sommet1).normalize()){
		if(cross(sommet2-sommet1,sommet3-sommet1).length()<0.0001)std::cout<<" A "<<sommet1<<" B "<< sommet2<<" C "<<sommet3<<std::endl<<std::flush;

	}
	float3 getNormal(){
		return Normale;
    }
	double IntersectionTriangle(Rayon myRay){
		//*****//algorithme de Moller-Trumbore (source wikipedia)//******//
		float3 origine=myRay.getOrigin();
		float3 vectDir=myRay.getDir();
		float3 arrete1=sommet2-sommet1;
		float3 arrete2=sommet3-sommet1;
		float3 h=cross(vectDir,arrete2);
		double a=dot(arrete1,h);
		if(abs(a)<0.0001)return -1;//rayon parallele au triangle
		double f=1.0/a;
		float3 s=origine-sommet1;
		double u=f*dot(s,h);
		if(u<0.0||u>1.0)return -1;
		float3 q=cross(s,arrete1);
		double v=f*dot(vectDir,q);
		if(v<0.0||u+v>1.0)return-1;
		double t=f*dot(arrete2,q);
		if (t>0.0001)return t;
		return -1;
		//******//
	}
};

class  Solide:public Objet{//solide defini par des triangles
protected:
	float3 normale=float3();
	std::vector<Triangle> listeTriangle;//liste des triangles composant le solide
public:
	Solide():listeTriangle(std::vector<Triangle>()),Objet(){}
	Solide(Color couleur,std::vector<double> materiau):listeTriangle(std::vector<Triangle>()),Objet(couleur,materiau){}
	Solide(std::vector<Triangle> liste,Color couleur,std::vector<double> materiau):listeTriangle(liste),Objet(couleur,materiau){}//
		
    double Intersection(Rayon myRay){//calcul l'index et la distance du plus proche objet intersectant le rayon
    	int indexPlusProche = -1;
    	double dPlusProche;
    	double d;
    	for (int i = 0; i < (int)listeTriangle.size(); i++) {	
        	d = listeTriangle[i].IntersectionTriangle(myRay);
        	if (d > 0 && (indexPlusProche == -1 || d < dPlusProche)) {
           	 	indexPlusProche = i;
            	dPlusProche = d;
        	}
    	}
    	
    	if(indexPlusProche==-1)return -1;
    	normale=listeTriangle[indexPlusProche].getNormal();
    	
    	return dPlusProche;
	}
	float3 getNormal(float3 M){
        return normale;
    }
    std::vector<Triangle> getListeTriangle(){
    	return listeTriangle;
    }
};

//quelques solides standards

Solide tetraedre(float3 s1, float3 s2,float3 s3,float3 s4,Color couleur,std::vector<double> materiau){
	std::vector<Triangle> listeTriangle=std::vector<Triangle>();
	listeTriangle.push_back(Triangle(s1,s2,s3));
	listeTriangle.push_back(Triangle(s1,s2,s4));
	listeTriangle.push_back(Triangle(s1,s3,s4));
	listeTriangle.push_back(Triangle(s2,s3,s4));
	Solide tetra=Solide(listeTriangle,couleur,materiau);
	return tetra;
}

Solide parallelepipede(float3 s1, float3 arrete1,float3 arrete2,float3 arrete3,Color couleur,std::vector<double> materiau){
	std::vector<Triangle> listeTriangle=std::vector<Triangle>();
	float3 s2=s1+arrete1;
	float3 s3=s1+arrete2;
	float3 s4=s1+arrete2+arrete1;
	float3 s5=s1+arrete1+arrete2+arrete3;
	float3 s6=s1+arrete3+arrete2;
	float3 s7=s1+arrete3;
	float3 s8=s1+arrete3+arrete1;

	listeTriangle.push_back(Triangle(s1,s2,s8));
	listeTriangle.push_back(Triangle(s1,s7,s8));

	listeTriangle.push_back(Triangle(s1,s2,s4));
	listeTriangle.push_back(Triangle(s1,s2,s3));

	listeTriangle.push_back(Triangle(s3,s4,s5));
	listeTriangle.push_back(Triangle(s3,s6,s5));

	listeTriangle.push_back(Triangle(s2,s4,s5));
	listeTriangle.push_back(Triangle(s2,s8,s5));

	listeTriangle.push_back(Triangle(s1,s3,s6));
	listeTriangle.push_back(Triangle(s1,s7,s6));
	
	listeTriangle.push_back(Triangle(s7,s6,s5));
	listeTriangle.push_back(Triangle(s7,s8,s5));
	


	return Solide(listeTriangle,couleur,materiau);
}

#endif