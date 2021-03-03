#ifndef _Color_H
#define _Color_H

class Color {
protected:

    double red, green, blue;

public:

    Color() : red(0), green(0), blue(0) {} // default color is black

    Color(double r, double g, double b) : red(r), green(g), blue(b) {}

    Color(std::vector<double> colorVect):red(colorVect[0]),green(colorVect[1]),blue(colorVect[2]){}


    double getColorR() { return red; };
    double getColorG() { return green; };
    double getColorB() { return blue; };

    void setColorR(double redValue) { red = redValue; };
    void setColorG(double greenValue) { green = greenValue; };
    void setColorB(double blueValue) { blue = blueValue; };

    Color check() {
        // ensures color values are correct
        double lightSum = red + green + blue;
        double lightExcess = lightSum - 3;
        if (lightExcess > 0) {
            red += lightExcess * (red / lightSum);
            green += lightExcess * (green / lightSum);
            blue += lightExcess * (blue / lightSum);
        }
        if (red > 1) red = 1;
        if (red < 0) red = 0;
        if (green > 1) green = 1;
        if (green < 0) green = 0;
        if (blue > 1) blue = 1;
        if (blue < 0) blue = 0;

        return Color(red, green, blue);
    }
    void operator=(Color anotherColor)  {
        red=anotherColor.getColorR();
        green=anotherColor.getColorG();
        blue=anotherColor.getColorB();
    }
    Color operator+(Color anotherColor) const {
        return Color(red + anotherColor.getColorR(), green + anotherColor.getColorG(), blue + anotherColor.getColorB());
    }
    Color operator/(double aScalar) const {
        return Color(red /aScalar, green / aScalar, blue / aScalar);
    }
    Color operator*(double aScalar) const {
        return Color(red *aScalar, green * aScalar, blue * aScalar);
    }
    Color operator*(Color aColor) const {
        return Color(red *aColor.getColorR(), green * aColor.getColorG(), blue * aColor.getColorB());
    }
    friend Color operator * (double, const Color);
    
};

inline Color operator* (double aValue, const Color aColor) {
    return Color(aValue * aColor.red, aValue * aColor.green, aValue * aColor.blue);
}
#endif
