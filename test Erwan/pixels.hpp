#include "float3.hpp"
#include "color.hpp"
class Pixel {
protected:
	//Coordonnées dans le plan de l'image
	int i;
	int j;
	Color pixel_color;
public:
	Pixel() :i(0),j(0), pixel_color(Color()){}
	Pixel(int iValue,int jValue) :i(iValue), j(jValue),pixel_color(Color()) {}
	Pixel(int iValue, int jValue, double redValue,double greenValue,double blueValue) :i(iValue), j(jValue), pixel_color(Color(redValue,greenValue, blueValue)) {}
	
	//renvoie les coordonnées dans l'espace
	float3 getCoord(const int Largeur,const int Hauteur) {
		return float3((double)i / (double)Largeur * 10.0, (double)j / (double)Hauteur * 10.0, 0);
	}
	Color getPixelColor() {
		return pixel_color;
	}
	void setCoord(int iValue, int jValue) {
		i = iValue;
		j = jValue;
	}
	void setColor(Color MyColor) {
		pixel_color = MyColor;
	}
};