#ifndef _Color_H
#define _Color_H

class Color {
    double red, green, blue;
    
public:
    Color ();
    
    Color(double, double, double);
    
    double getColorR(double redValue) {return red};
    double getColorG(double greenValue) {return green};
    double getColorB(double rblueValue) {return blue};
    
    double setColorR(double redValue) {red = redValue};
    double setColorG(double greenValue) {green = greenValue};
    double setColorB(double rblueValue) {blue = blueValue};
    
    Color check() {
        // ensures color values are correct
        double lightSum = red + green + blue;
        double lightExcess = lightSum - 3;
        if (excesslight > 0) {
            red += lightExcess*(red/lightSum);
            green += lightExcess*(green/lightSum);
            blue += lightExcess*(blue/lightSum);
        }
        if (red > 1) red = 1;
        if (red < 0) red = 0;
        if (green > 1) green = 1;
        if (green < 0) green = 0;
        if (blue > 1) blue = 1;
        if (blue < 0) blue = 0;

        return Color (red, green, blue);
    }
}

//Grey as default color
Color::Color () {
    red = 0.5;
    green = 0.5;
    blue = 0.5;
}

Color::Color (double r, double g, double b) {
    red = r;
    green = g;
    blue = b;
}
#endif
