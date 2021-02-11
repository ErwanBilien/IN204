#include"float3.hpp"
#include <iostream>
#include <cmath>
class Camera{
protected:
	float3 position;
	float3 rotation;//rotation autours des axes x,y et z
public:
	Camera():position(float3()),rotation(float3()){}
	Camera(float3 coord,float3 rot):position(coord),rotation(rot){}
	float3 coordPixel(int i,int j,int nbPixelLargeur,int nbPixelHauteur,double largeurImage,double hauteurImage,double distance){//vecteur directeur du rayon passant par le pixel(i,j)
		float3 vectDir=float3(i * largeurImage / (double)nbPixelLargeur, j * hauteurImage / (double)nbPixelHauteur, distance);
		double x=vectDir.getX();
		double y=vectDir.getY();
		double z=vectDir.getZ();
		double thetaX=rotation.getX();
		double thetaY=rotation.getY();
		double thetaZ=rotation.getZ();
		
		//rotation autours de x
		vectDir=float3(x,cos(thetaX)*y-sin(thetaX)*z,sin(thetaX)*y+cos(thetaX)*z);
		//rotation autours de y
		x=vectDir.getX();
		y=vectDir.getY();
		z=vectDir.getZ();
		vectDir=float3(cos(thetaY)*x+sin(thetaY)*z,y,-sin(thetaY)*x+cos(thetaY)*z);
		//rotation autours de z
		x=vectDir.getX();
		y=vectDir.getY();
		z=vectDir.getZ();
		vectDir=float3(cos(thetaZ)*x-sin(thetaZ)*y,sin(thetaZ)*x+cos(thetaZ)*y,z);
		return vectDir;
	}
	float3 getPosition(){
		return position;
	}
};