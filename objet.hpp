#ifndef OBJET
#define OBJET
#include "color.hpp"
#include "rayon.hpp"
#include <vector>

class Objet {
protected:
  Color couleurObjet;
  double brillance;
  double reflechissance; // 0 ne reflechie pas; 1 l'objet est un mirroir parfait

public:
  Objet() : couleurObjet(Color()), brillance(-1), reflechissance(-1) {}
  Objet(Color aColor)
      : couleurObjet(aColor), brillance(-1), reflechissance(-1) {}
  Objet(Color aColor, double brille, double reflech)
      : couleurObjet(aColor), brillance(brille), reflechissance(reflech) {}
  virtual double Intersection(Rayon MyRay) { return 0; }
  Color getColor() { return couleurObjet; }

  virtual float3 getNormal(float3 M) { return float3(); }
  double getShine() { return brillance; }
  double getReflechissance() { return reflechissance; }
};

void CalculIntersection(
    std::vector<Objet *> listeObjets, Rayon myRay, int *indexPlusProche,
    double *dPlusProche) { // calcul l'index et la distance du plus proche objet
                           // intersectant le rayon
  *indexPlusProche = -1;
  double d;
  for (int i = 0; i < (int)listeObjets.size(); i++) {
    d = listeObjets[i]->Intersection(myRay);
    if (d > 0 && (*indexPlusProche == -1 || d < *dPlusProche)) {
      *indexPlusProche = i;
      *dPlusProche = d;
    }
  }
}

#endif