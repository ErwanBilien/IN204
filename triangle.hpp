#ifndef TRIANGLE
#define TRIANGLE
#include "objet.hpp"

class Triangle{
protected:
	float3 sommet1;//sommets du triangle
	float3 sommet2;
	float3 sommet3;
public:
	Triangle(float3 s1,float3 s2,float3 s3):sommet1(s1),sommet2(s2),sommet3(s3){}
	float3 getNormal(){
		return cross(sommet2-sommet1,sommet3-sommet1).normalize();
    }
	double IntersectionTriangle(Rayon myRay){
		//*****//algorithme de Moller-Trumbore (source wikipedia)//******//
		float3 origine=myRay.getOrigin();
		float3 vectDir=myRay.getDir();
		float3 arrete1=sommet2-sommet1;
		float3 arrete2=sommet3-sommet1;
		float3 h=cross(vectDir,arrete2);
		double a=dot(arrete1,h);
		if(a<0.000001&&a>-0.000001)return -1;//rayon parallele au triangle
		double f=1.0/a;
		float3 s=origine-sommet1;
		double u=f*dot(s,h);
		if(u<0.0||u>1.0)return -1;
		float3 q=cross(s,arrete1);
		double v=f*dot(vectDir,q);
		if(v<0.0||u+v>1.0)return-1;
		double t=f*dot(arrete2,q);
		if (t>0.000001)return t;
		return -1;
		//******//
	}
};

class  Solide:public Objet{//solide defini par des triangles
protected:
	float3 normale=float3();
	std::vector<std::shared_ptr<Triangle>> listeTriangle;//liste des triangles composant le solide

	//parametre pour une precalculation de l'intersection : si cette option est utilisée, on verifie si le rayn incident intersecte une sphere contenant le solide. Si oui, on determine precisement l'intersection
	bool preCalcul=false;
	double rayon=0.0;
	float3 centre=float3();

public:
	Solide() : listeTriangle(std::vector<std::shared_ptr<Triangle>>()), Objet() {}
	//Solide(Color couleur,std::vector<double> materiau) : listeTriangle(std::vector<Triangle>()),Objet(couleur,materiau){}
	//Solide(std::vector<Triangle> liste,Color couleur,std::vector<double> materiau) : listeTriangle(liste),Objet(couleur,materiau){}
	Solide(std::vector<std::shared_ptr<Triangle>> liste, Color couleur, double brille, double reflech, double transpa, double indiceRefraction) :
		listeTriangle(std::vector<std::shared_ptr<Triangle>>()), Objet(couleur,brille,reflech,transpa,indiceRefraction) {}
		
    double Intersection(Rayon myRay){//calcul l'index et la distance du plus proche objet intersectant le rayon
    	if(preCalcul){
    		float3 origine =myRay.getOrigin();
        	float3 vectDir=myRay.getDir();
        	float3 CO = origine - centre;
        	double a = dot(vectDir, vectDir);
        	double b = 2 * dot(CO, vectDir);
        	double c = dot(CO, CO)  - rayon * rayon;

	        double delta = b * b - 4 * a * c;
	        if (delta < 0) {
	            return -1;
	        }
	        double d1 = (-b + sqrt(delta)) / (2 * a);
	        double d2 = (-b - sqrt(delta)) / (2 * a);
	        if (d1 < 0 && d2 < 0) {
	            return -1;
	        }
	        if (d1 * d2 <= 0.0001) {
	            if(std::max(d1, d2)<0.00001)return -1;
	        }
	        else {
	            if(std::min(d1, d2)<0.00001)return -1;
	        }
    	}

    	int indexPlusProche = -1;
    	double dPlusProche;
    	double d;
    	for (int i = 0; i < (int)listeTriangle.size(); i++) {	
        	d = listeTriangle[i]->IntersectionTriangle(myRay);
        	if (d > 0.0 && (indexPlusProche == -1 || d < dPlusProche)) {
           	 	indexPlusProche = i;
            	dPlusProche = d;
        	}
    	}
    	
    	if(indexPlusProche==-1)return -1;
    	normale=listeTriangle[indexPlusProche]->getNormal();

    	
    	return dPlusProche;
	}
	float3 getNormal(float3 M){
        return normale;
    }
    std::vector<std::shared_ptr<Triangle>> getListeTriangle(){
    	return listeTriangle;
    }
    void setListTriangle(std::vector<std::shared_ptr<Triangle>> nouvelleListe){
    	listeTriangle=nouvelleListe;
    }
};

//quelques solides standards
class Tetraedre:public Solide{
public:
	/*Tetraedre(float3 s1,float3 s2,float3 s3,float3 s4, Color couleur, std::vector<double> materiau):
		Solide(std::vector<Triangle>(), couleur, materiau) {
		
		this->listeTriangle.push_back(Triangle(s1,s2,s3));
		this->listeTriangle.push_back(Triangle(s1,s2,s4));
		this->listeTriangle.push_back(Triangle(s1,s3,s4));
		this->listeTriangle.push_back(Triangle(s2,s3,s4));

		this->preCalcul=true;
		this->centre=(s1+s2+s3+s4)/4;
		this->rayon=(centre-s1).length();
		this->rayon=std::max(rayon,(centre-s2).length());
		this->rayon=std::max(rayon,(centre-s3).length());
		this->rayon=std::max(rayon,(centre-s4).length());
		this->rayon*=1.1;
	}*/

	Tetraedre(float3 s1,float3 s2,float3 s3,float3 s4, Color couleur, double brille,double reflech,double transpa,double indiceRefraction):
		Solide(std::vector<std::shared_ptr<Triangle>>(),couleur, brille, reflech, transpa, indiceRefraction) {
		
		auto triangle1 = std::shared_ptr<Triangle> (new Triangle(s1,s2,s3));
		auto triangle2 = std::shared_ptr<Triangle> (new Triangle(s1,s2,s3));
		auto triang1e3 = std::shared_ptr<Triangle> (new Triangle(s1,s2,s3));
		auto triangle4 = std::shared_ptr<Triangle> (new Triangle(s1,s2,s3));

		this->listeTriangle.push_back( triangle1 ); // convertir ça en pointeurs
		this->listeTriangle.push_back( triangle2 );
		this->listeTriangle.push_back( triang1e3 );
		this->listeTriangle.push_back( triangle4 );

		this->preCalcul=true;
		this->centre=(s1+s2+s3+s4)/4;
		this->rayon=(centre-s1).length();
		this->rayon=std::max(rayon,(centre-s2).length());
		this->rayon=std::max(rayon,(centre-s3).length());
		this->rayon=std::max(rayon,(centre-s4).length());
		this->rayon*=1.1;
	}

};

class Parallelepipede:public Solide{
public:
	/*Parallelepipede(float3 s1, float3 arrete1,float3 arrete2,float3 arrete3,Color couleur,std::vector<double> materiau) :
		Solide(std::vector<Triangle>(), couleur, materiau){
	
		float3 s2=s1+arrete1;
		float3 s3=s1+arrete2;
		float3 s4=s1+arrete2+arrete1;
		float3 s5=s1+arrete1+arrete2+arrete3;
		float3 s6=s1+arrete3+arrete2;
		float3 s7=s1+arrete3;
		float3 s8=s1+arrete3+arrete1;

		this->listeTriangle.push_back(Triangle(s1,s2,s8));
		this->listeTriangle.push_back(Triangle(s1,s7,s8));

		this->listeTriangle.push_back(Triangle(s1,s2,s4));
		this->listeTriangle.push_back(Triangle(s1,s2,s3));

		this->listeTriangle.push_back(Triangle(s3,s4,s5));
		this->listeTriangle.push_back(Triangle(s3,s6,s5));

		this->listeTriangle.push_back(Triangle(s2,s4,s5));
		this->listeTriangle.push_back(Triangle(s2,s8,s5));

		this->listeTriangle.push_back(Triangle(s1,s3,s6));
		this->listeTriangle.push_back(Triangle(s1,s7,s6));
		
		this->listeTriangle.push_back(Triangle(s7,s6,s5));
		this->listeTriangle.push_back(Triangle(s7,s8,s5));

		this->preCalcul=true;
		this->centre=s1+(arrete3+arrete2+arrete1)/3;
		this->rayon=0.6*(arrete3+arrete2+arrete1).length();
	}*/

	Parallelepipede(float3 s1,float3 arrete1,float3 arrete2,float3 arrete3, Color couleur, double brille,double reflech,double transpa,double indiceRefraction) :
		Solide(std::vector<std::shared_ptr<Triangle>>(), couleur, brille, reflech, transpa, indiceRefraction) {
	
		float3 s2=s1+arrete1;
		float3 s3=s1+arrete2;
		float3 s4=s1+arrete2+arrete1;
		float3 s5=s1+arrete1+arrete2+arrete3;
		float3 s6=s1+arrete3+arrete2;
		float3 s7=s1+arrete3;
		float3 s8=s1+arrete3+arrete1;

		auto triangle1  = std::shared_ptr<Triangle> (new Triangle(s1,s2,s8));
		auto triangle2  = std::shared_ptr<Triangle> (new Triangle(s1,s7,s8));

		auto triang1e3  = std::shared_ptr<Triangle> (new Triangle(s1,s2,s4));
		auto triangle4  = std::shared_ptr<Triangle> (new Triangle(s1,s2,s3));

		auto triangle5  = std::shared_ptr<Triangle> (new Triangle(s3,s4,s5));
		auto triangle6  = std::shared_ptr<Triangle> (new Triangle(s3,s6,s5));

		auto triangle7  = std::shared_ptr<Triangle> (new Triangle(s2,s4,s5));
		auto triangle8  = std::shared_ptr<Triangle> (new Triangle(s2,s8,s5));

		auto triangle9  = std::shared_ptr<Triangle> (new Triangle(s1,s3,s6));
		auto triangle10 = std::shared_ptr<Triangle> (new Triangle(s1,s7,s6));

		auto triangle11 = std::shared_ptr<Triangle> (new Triangle(s7,s6,s5));
		auto triangle12 = std::shared_ptr<Triangle> (new Triangle(s7,s8,s5));

		this->listeTriangle.push_back(triangle1);
		this->listeTriangle.push_back(triangle2);

		this->listeTriangle.push_back(triang1e3);
		this->listeTriangle.push_back(triangle4);

		this->listeTriangle.push_back(triangle5);
		this->listeTriangle.push_back(triangle6);

		this->listeTriangle.push_back(triangle7);
		this->listeTriangle.push_back(triangle8);

		this->listeTriangle.push_back(triangle9);
		this->listeTriangle.push_back(triangle10);
		
		this->listeTriangle.push_back(triangle11);
		this->listeTriangle.push_back(triangle12);

		this->preCalcul=true;
		this->centre=s1+(arrete3+arrete2+arrete1)/3;
		this->rayon=0.6*(arrete3+arrete2+arrete1).length();
	}
};

#endif