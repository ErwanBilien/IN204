#ifndef SCENE
#define SCENE
#include "Lumiere.hpp"
#include "SaveBMP.hpp"
#include "color.hpp"
#include "float3.hpp"
#include "fonctionRayTracer.hpp"
#include "sphere.hpp"
#include "tinyxml2.h"

/*
Please note this code is extensively based upon the following tutorial:
https://shilohjames.wordpress.com/2014/04/27/tinyxml2-tutorial/
*/

using namespace tinyxml2;

// the following is a macro to check Xml files are loaded successfully
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult)                                              \
  if (a_eResult != XML_SUCCESS) {                                              \
    printf("Error: %i\n", a_eResult);                                          \
    exit(EXIT_FAILURE);                                                        \
  };
#endif

// -----------------------------------------------------------------------
// This is where we load the scenes from the sceneDesciption.xml file

// Below we extract the objects
std::vector<std::shared_ptr<Objet>> readObjectsFromXML(const char *scene) {
  std::cout << "loading " << scene << std::endl;

  std::vector<std::shared_ptr<Objet>> ObjectList;

  XMLDocument xmlDoc; // loads .xml file into XML document
  XMLError eResult = xmlDoc.LoadFile(scene); // loads the scene
  XMLCheckResult(eResult);                   // checks for errors with macro

  std::cout << "2" << std::endl;
  XMLNode *pRoot =
      xmlDoc.FirstChild(); // pointer to the <root> of the .xml file

  std::cout << "3" << std::endl;

  if (pRoot == nullptr) {
    XMLCheckResult(XML_ERROR_PARSING_ELEMENT); // checks for errors with macro
  }

  std::cout << "3" << std::endl;

  XMLElement *pObject =
      pRoot->FirstChildElement("Objects"); // goes down from <root> to <Objects>
  // if (pObject == nullptr) return XML_ERROR_PARSING_ELEMENT;

  std::cout << "4" << std::endl;

  if (!pObject)
    XMLCheckResult(XML_ERROR_PARSING_ELEMENT);

  // -----S P H E R E S
  // -----------------------------------------------------------------------------------------
  XMLNode *pSpheres = pObject->FirstChildElement(
      "Spheres"); // goes down from <Objects> to <Spheres>

  if (pSpheres == nullptr)
    XMLCheckResult(XML_ERROR_PARSING_ELEMENT);

  std::cout << "4.2" << std::endl;

  XMLElement *pListElement = pSpheres->FirstChildElement("Sphere");

  std::cout << "5" << std::endl;

  if (pListElement == nullptr) {
    exit(EXIT_FAILURE);
    // return XML_ERROR_PARSING_ELEMENT;
  } else {
    std::cout << "6" << std::endl;

    double dOutRayon_s, dOutXCentre, dOutYCentre, dOutZCentre;
    double dOutRedColor, dOutGreenColor, dOutBlueColor;
    double dOutBright, dOutReflect;

    while (pListElement != nullptr) {
      eResult = pListElement->QueryDoubleAttribute("Rayon_S", &dOutRayon_s);
      XMLCheckResult(eResult);
      assert(dOutRayon_s >= 0);
      eResult = pListElement->QueryDoubleAttribute("xCentre", &dOutXCentre);
      XMLCheckResult(eResult);
      eResult = pListElement->QueryDoubleAttribute("yCentre", &dOutYCentre);
      XMLCheckResult(eResult);
      eResult = pListElement->QueryDoubleAttribute("zCentre", &dOutZCentre);
      XMLCheckResult(eResult);
      eResult = pListElement->QueryDoubleAttribute("redColor", &dOutRedColor);
      XMLCheckResult(eResult);
      assert(dOutRedColor >= 0 && dOutRedColor <= 1);
      eResult =
          pListElement->QueryDoubleAttribute("greenColor", &dOutGreenColor);
      XMLCheckResult(eResult);
      assert(dOutGreenColor >= 0 && dOutGreenColor <= 1);
      eResult = pListElement->QueryDoubleAttribute("blueColor", &dOutBlueColor);
      XMLCheckResult(eResult);
      assert(dOutBlueColor >= 0 && dOutBlueColor <= 1);
      eResult = pListElement->QueryDoubleAttribute("brightness", &dOutBright);
      XMLCheckResult(eResult);
      assert(dOutBright >= 0);
      eResult = pListElement->QueryDoubleAttribute("reflection", &dOutReflect);
      XMLCheckResult(eResult);
      assert(dOutReflect >= 0 && dOutReflect <= 1);

      auto sphere = std::shared_ptr<Objet>(
          new Sphere(dOutRayon_s, float3(dOutXCentre, dOutYCentre, dOutZCentre),
                     Color(dOutRedColor, dOutGreenColor, dOutBlueColor),
                     dOutBright, dOutReflect)); // create a sphere
      ObjectList.push_back(sphere);

      pListElement = pListElement->NextSiblingElement("Sphere"); // jump to next
    }
  }

  return ObjectList;
}

#endif