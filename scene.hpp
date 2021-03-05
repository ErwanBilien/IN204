#ifndef SCENE
#define SCENE
#include <memory>
#include <string>
#include <map>
#include <string>
#include "Lumiere.hpp"
#include "SaveBMP.hpp"
#include "color.hpp"
#include "float3.hpp"
#include "fonctionRayTracer.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "triangle.hpp"
#include "image.hpp"
#include "tinyxml2.h"

// -----------------------------------------------------------------------
// This is where we load the scenes from the sceneDesciption.xml file
// -----------------------------------------------------------------------

/*
Please note this code is inspired by the following tutorial:
https://shilohjames.wordpress.com/2014/04/27/tinyxml2-tutorial/
*/

using namespace tinyxml2;

// the following is a macro to check that XML files are loaded successfully
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult)                                              \
    if (a_eResult != XML_SUCCESS) {                                            \
        printf("Error: %i\n", a_eResult);                                      \
        exit(EXIT_FAILURE);                                                    \
  };
#endif

//----------------------------------------------------------------------------
//----- P I C T U R E   ------------------------------------------------------
//----------------------------------------------------------------------------

double readCameraToPictureDistanceFromXML(const char *scene)
{
    XMLDocument xmlDoc; // loads .xml file into XML document
    XMLError eResult = xmlDoc.LoadFile(scene); // loads the scene
    XMLCheckResult(eResult);                   // checks for errors with macro    
    XMLNode *pRoot = xmlDoc.FirstChild(); // points to the <root> of the .xml file
    if (pRoot == nullptr) {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT); // checks for errors with macro
    }

    XMLElement *pPicCamDistance = pRoot->FirstChildElement("PictureToCameraDistance");
    if (pPicCamDistance == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    double dOutDistance;
    eResult = pPicCamDistance->QueryDoubleText(&dOutDistance);
    XMLCheckResult(eResult);

    return dOutDistance;
}

Picture readPictureFromXML(const char *scene)
{
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(scene);
    XMLCheckResult(eResult);
    XMLNode *pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    }

    XMLElement *pPixels = pRoot->FirstChildElement("Width");
    if (pPixels == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    int width;
    eResult = pPixels->QueryIntText(&width);
    XMLCheckResult(eResult);

    pPixels = pRoot->FirstChildElement("Height");
    if (pPixels == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    int height;
    eResult = pPixels->QueryIntText(&height);
    XMLCheckResult(eResult);

    return Picture(width, height);
}

//----------------------------------------------------------------------------
//----- C A M E R A   --------------------------------------------------------
//----------------------------------------------------------------------------

Camera readCameraFromXML(const char *scene)
{
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(scene);
    XMLCheckResult(eResult); 
    XMLNode *pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    }
    XMLElement *pCamera = pRoot->FirstChildElement("Camera");
    if (pCamera == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);

    double dOutXCentre, dOutYCentre, dOutZCentre, dOutTheta, dOutPhi, dOutPsi;

    eResult = pCamera->QueryDoubleAttribute("xCentre", &dOutXCentre);
    XMLCheckResult(eResult);
    eResult = pCamera->QueryDoubleAttribute("yCentre", &dOutYCentre);
    XMLCheckResult(eResult);
    eResult = pCamera->QueryDoubleAttribute("zCentre", &dOutZCentre);
    XMLCheckResult(eResult);
    eResult = pCamera->QueryDoubleAttribute("theta", &dOutTheta);
    XMLCheckResult(eResult);
    eResult = pCamera->QueryDoubleAttribute("phi", &dOutPhi);
    XMLCheckResult(eResult);
    eResult = pCamera->QueryDoubleAttribute("psi", &dOutPsi);
    XMLCheckResult(eResult);

    return Camera( float3(dOutXCentre,dOutYCentre,dOutZCentre), float3(dOutTheta,dOutPhi,dOutPsi) );

}

//----------------------------------------------------------------------------
//----- L I G H T S ----------------------------------------------------------
//----------------------------------------------------------------------------

std::vector<std::shared_ptr<Lumiere>> readLightsFromXML(const char *scene)
{
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(scene);
    XMLCheckResult(eResult);
    XMLNode *pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    }

    // ----- C O L O R S -----------------------------------------------------------------------
    
    std::map<std::string, int> mapOfColors;        
    std::vector<std::shared_ptr<Color>> ColorList;

    XMLElement *pColor = pRoot->FirstChildElement("Colors");
    if (pColor == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);    

    XMLElement *pListElement = pColor->FirstChildElement("Color");
    if (!pListElement)
    {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
        exit(EXIT_FAILURE);
    }
    else
    {
        double dOutRedColor, dOutGreenColor, dOutBlueColor;
        int count=0;
        const char * szAttributeText = nullptr;
        std::string strOutColorName;

        while (pListElement != nullptr)
        {
            szAttributeText = pListElement->Attribute("colorName");
            strOutColorName = szAttributeText;
            mapOfColors[strOutColorName] = count;
            count+=1;

            eResult = pListElement->QueryDoubleAttribute("redColor", &dOutRedColor);
            XMLCheckResult(eResult);
            assert(dOutRedColor >= 0 && dOutRedColor <= 1);

            eResult = pListElement->QueryDoubleAttribute("greenColor", &dOutGreenColor);
            XMLCheckResult(eResult);
            assert(dOutGreenColor >= 0 && dOutGreenColor <= 1);

            eResult = pListElement->QueryDoubleAttribute("blueColor", &dOutBlueColor);
            XMLCheckResult(eResult);
            assert(dOutBlueColor >= 0 && dOutBlueColor <= 1);        

            auto color = std::shared_ptr<Color>(new Color(dOutRedColor, dOutGreenColor, dOutBlueColor ));
            ColorList.push_back(color);
            pListElement = pListElement->NextSiblingElement("Color");
        }
    }   

    // --- L I G H T S -------------------------------------------------------------------------

    std::vector<std::shared_ptr<Lumiere>> LightsList;

    XMLElement *pLights = pRoot->FirstChildElement("Lights");
    if (pLights == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT); // we don't authorize a scene without lighting

    double dOutIntensity, dOutXCentre, dOutYCentre, dOutZCentre, dOutXDir, dOutYDir, dOutZDir;
    const char * charOutColor;
    std::string strOutColor;

    XMLNode *pLightAmbiant = pLights->FirstChildElement("Ambiant");
    pListElement = pLightAmbiant->FirstChildElement("AmbiantLight");
    while (pListElement != nullptr)
    {
        eResult = pListElement->QueryDoubleAttribute("intensity", &dOutIntensity);
        XMLCheckResult(eResult);
        assert(dOutIntensity >= 0);
        charOutColor = pListElement->Attribute("color");
        XMLCheckResult(eResult);
        strOutColor = charOutColor;

        auto light = std::shared_ptr<Lumiere> (new LumiereAmbiante(
            dOutIntensity,
            Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                  ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                  ColorList[ mapOfColors[ strOutColor ] ]->getColorB() ))); // creates a light

        LightsList.push_back(light);

        pListElement = pListElement->NextSiblingElement("AmbiantLight"); // jump to next AmbiantLight
    }
    
    XMLNode *pLightPunctual = pLights->FirstChildElement("Punctual");
    pListElement = pLightPunctual->FirstChildElement("PunctualLight");
    while (pListElement != nullptr)
    {
        eResult = pListElement->QueryDoubleAttribute("intensity", &dOutIntensity);
        XMLCheckResult(eResult);
        assert(dOutIntensity >= 0);
        eResult = pListElement->QueryDoubleAttribute("xCentre", &dOutXCentre);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("yCentre", &dOutYCentre);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("zCentre", &dOutZCentre);
        XMLCheckResult(eResult);
        charOutColor = pListElement->Attribute("color");
        XMLCheckResult(eResult);
        strOutColor = charOutColor;

        auto light = std::shared_ptr<Lumiere> (new LumierePonctuelle(
            dOutIntensity,
            float3(dOutXCentre, dOutYCentre, dOutZCentre),
            Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                  ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                  ColorList[ mapOfColors[ strOutColor ] ]->getColorB() )));

        LightsList.push_back(light);

        pListElement = pListElement->NextSiblingElement("PunctualLight");
    }

    XMLNode *pLightDirectional = pLights->FirstChildElement("Directional");
    pListElement = pLightDirectional->FirstChildElement("DirectionalLight");
    while (pListElement != nullptr)
    {
        eResult = pListElement->QueryDoubleAttribute("intensity", &dOutIntensity);
        XMLCheckResult(eResult);
        assert(dOutIntensity >= 0);
        eResult = pListElement->QueryDoubleAttribute("xDirection", &dOutXDir);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("yDirection", &dOutYDir);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("zDirection", &dOutZDir);
        XMLCheckResult(eResult);
        charOutColor = pListElement->Attribute("color");
        XMLCheckResult(eResult);
        strOutColor = charOutColor;

        auto light = std::shared_ptr<Lumiere> (new LumiereDirectionnelle(dOutIntensity,float3(dOutXDir, dOutYDir, dOutZDir),Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),ColorList[ mapOfColors[ strOutColor ] ]->getColorB() )));

        LightsList.push_back(light);

        pListElement = pListElement->NextSiblingElement("DirectionalLight");
    }

    XMLNode *pLightSpot = pLights->FirstChildElement("SpotLights");
    pListElement = pLightSpot->FirstChildElement("SpotLight");
    double dOutSpotAngle;
    while (pListElement != nullptr)
    {
        eResult = pListElement->QueryDoubleAttribute("intensity", &dOutIntensity);
        XMLCheckResult(eResult);
        assert(dOutIntensity >= 0);

        eResult = pListElement->QueryDoubleAttribute("xCentre", &dOutXCentre);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("yCentre", &dOutYCentre);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("zCentre", &dOutZCentre);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("xDirection", &dOutXDir);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("yDirection", &dOutYDir);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("zDirection", &dOutZDir);
        XMLCheckResult(eResult);
        eResult = pListElement->QueryDoubleAttribute("angle", &dOutSpotAngle);
        XMLCheckResult(eResult);
        charOutColor = pListElement->Attribute("color");
        XMLCheckResult(eResult);
        strOutColor = charOutColor;

        auto light = std::shared_ptr<Lumiere> (new Spot(
            dOutIntensity,
            float3(dOutXCentre, dOutYCentre, dOutZCentre),
            float3(dOutXDir, dOutYDir, dOutZDir),
            dOutSpotAngle,
            Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                  ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                  ColorList[ mapOfColors[ strOutColor ] ]->getColorB() )));

        LightsList.push_back(light);

        pListElement = pListElement->NextSiblingElement("SpotLight");
    }

    return LightsList;
}

//----------------------------------------------------------------------------
//----- O B J E C T S --------------------------------------------------------
//----------------------------------------------------------------------------

std::vector<std::shared_ptr<Objet>> readObjectsFromXML(const char *scene)
{
    std::vector<std::shared_ptr<Objet>> ObjectList;

    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(scene);
    XMLCheckResult(eResult);

    XMLNode *pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
    }

    // ----- M A T E R I A L S -----------------------------------------------------------------------
    
    std::map<std::string, int> mapOfMaterials; // index of materials (a map serves the same purpose as an ENUM but it's dynamic)        
    std::vector<std::shared_ptr<Material>> MaterialList;

    XMLElement *pMaterial = pRoot->FirstChildElement("Materials"); // goes down from <root> to <Materials>
    if (pMaterial == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);    

    XMLElement *pListElement = pMaterial->FirstChildElement("Material");
    if (!pListElement)
    {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
        exit(EXIT_FAILURE);
    }
    else
    {
        double dOutBright, dOutReflect, dOutTranspa, dOutRefract;
        int count=0;
        const char * szAttributeText = nullptr;
        std::string strOutMatName;

        while (pListElement != nullptr)
        {
            szAttributeText = pListElement->Attribute("materialName");

            strOutMatName = szAttributeText;
            mapOfMaterials[strOutMatName] = count++;
            //count+=1;
            eResult = pListElement->QueryDoubleAttribute("brightness", &dOutBright);
            XMLCheckResult(eResult);
            assert(dOutBright >= 0);

            eResult = pListElement->QueryDoubleAttribute("reflectivity", &dOutReflect);
            XMLCheckResult(eResult);
            assert(dOutReflect >= 0 && dOutReflect <= 1);
            
            eResult = pListElement->QueryDoubleAttribute("transparency", &dOutTranspa);
            XMLCheckResult(eResult);
            assert(dOutReflect+dOutTranspa <= 1.0);
            
            eResult = pListElement->QueryDoubleAttribute("refraction", &dOutRefract);
            XMLCheckResult(eResult);
            assert(dOutReflect+dOutTranspa <= 1.0);            

            auto material = std::shared_ptr<Material>(new Material(strOutMatName, dOutBright, dOutReflect, dOutTranspa, dOutRefract )); // creates a material
            MaterialList.push_back(material);
            pListElement = pListElement->NextSiblingElement("Material");
        }
    }

    // ----- C O L O R S -----------------------------------------------------------------------
    
    std::map<std::string, int> mapOfColors;        
    std::vector<std::shared_ptr<Color>> ColorList;

    XMLElement *pColor = pRoot->FirstChildElement("Colors");
    if (pColor == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);    

    pListElement = pColor->FirstChildElement("Color");
    if (!pListElement)
    {
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT);
        exit(EXIT_FAILURE);
    }
    else
    {
        double dOutRedColor, dOutGreenColor, dOutBlueColor;
        int count=0;
        const char * szAttributeText = nullptr;
        std::string strOutColorName;

        while (pListElement != nullptr)
        {
            szAttributeText = pListElement->Attribute("colorName");
            strOutColorName = szAttributeText;
            mapOfColors[strOutColorName] = count;
            count+=1;

            eResult = pListElement->QueryDoubleAttribute("redColor", &dOutRedColor);
            XMLCheckResult(eResult);
            assert(dOutRedColor >= 0 && dOutRedColor <= 1);

            eResult = pListElement->QueryDoubleAttribute("greenColor", &dOutGreenColor);
            XMLCheckResult(eResult);
            assert(dOutGreenColor >= 0 && dOutGreenColor <= 1);

            eResult = pListElement->QueryDoubleAttribute("blueColor", &dOutBlueColor);
            XMLCheckResult(eResult);
            assert(dOutBlueColor >= 0 && dOutBlueColor <= 1);        

            auto color = std::shared_ptr<Color>(new Color(dOutRedColor, dOutGreenColor, dOutBlueColor ));
            ColorList.push_back(color);
            pListElement = pListElement->NextSiblingElement("Color");
        }
    }

    //----- O B J E C T S : all objects--------------------------------------------------------------------

    XMLElement *pObject = pRoot->FirstChildElement("Objects");
    if (pObject == nullptr)
        XMLCheckResult(XML_ERROR_PARSING_ELEMENT); // we don't allow a scene without any object

    // -----S P H E R E S -----------------------------------------------------------------------------------------

    XMLNode *pSpheres = pObject->FirstChildElement("Spheres"); // goes down from <Objects> to <Spheres>
    pListElement = pSpheres->FirstChildElement("Sphere");

    if (pListElement != nullptr) {
        double dOutRayon_s, dOutXCentre, dOutYCentre, dOutZCentre;
        const char * charOutMaterial;
        const char * charOutColor;
        std::string strOutMaterial, strOutColor;

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
            charOutColor = pListElement->Attribute("color"); // cont char *
            strOutColor = charOutColor; // cast
            charOutMaterial = pListElement->Attribute("material"); // cont char *
            strOutMaterial = charOutMaterial; // cast
            
            auto sphere = std::shared_ptr<Objet>   (new Sphere(dOutRayon_s,float3(dOutXCentre, dOutYCentre, dOutZCentre),
                Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                	  ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                	  ColorList[ mapOfColors[ strOutColor ] ]->getColorB()),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getBrightness(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getReflectivity(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getTransparency(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getRefraction()));

            ObjectList.push_back(sphere);

            pListElement = pListElement->NextSiblingElement("Sphere");
        }
    }

    // -----P L A N S -----------------------------------------------------------------------------------------

    XMLNode *pPlans = pObject->FirstChildElement("Plans");
    pListElement = pPlans->FirstChildElement("Plan");

    if (pListElement != nullptr) {
        double dOutAX, dOutAY, dOutAZ, dOutBX, dOutBY, dOutBZ, dOutCX, dOutCY, dOutCZ;
        const char * charOutMaterial;
        const char * charOutColor;
        std::string strOutMaterial, strOutColor;

        while (pListElement != nullptr) {
            eResult = pListElement->QueryDoubleAttribute("aX", &dOutAX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("aY", &dOutAY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("aZ", &dOutAZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bX", &dOutBX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bY", &dOutBY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bZ", &dOutBZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cX", &dOutCX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cY", &dOutCY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cZ", &dOutCZ);
            XMLCheckResult(eResult);
            
            charOutColor = pListElement->Attribute("color");
            strOutColor = charOutColor;
            charOutMaterial = pListElement->Attribute("material");
            strOutMaterial = charOutMaterial;

            auto plan = std::shared_ptr<Objet>   (new Plan(
                float3(dOutAX, dOutAY, dOutAZ),
                float3(dOutBX, dOutBY, dOutBZ),
                float3(dOutCX, dOutCY, dOutCZ),
                Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorB()),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getBrightness(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getReflectivity(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getTransparency(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getRefraction() ));

            ObjectList.push_back(plan);

            pListElement = pListElement->NextSiblingElement("Plan");
        }
    }

// -----T E T R A H E D R O N S ----------------------------------------------------------------------------

    XMLNode *pTetra = pObject->FirstChildElement("Tetrahedrons");
    pListElement = pTetra->FirstChildElement("Tetrahedron");

    if (pListElement != nullptr) {
        double dOutAX, dOutAY, dOutAZ, dOutBX, dOutBY, dOutBZ, dOutCX, dOutCY, dOutCZ, dOutDX, dOutDY, dOutDZ;
        const char * charOutMaterial;
        const char * charOutColor;
        std::string strOutMaterial, strOutColor;

        while (pListElement != nullptr) {
            eResult = pListElement->QueryDoubleAttribute("aX", &dOutAX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("aY", &dOutAY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("aZ", &dOutAZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bX", &dOutBX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bY", &dOutBY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bZ", &dOutBZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cX", &dOutCX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cY", &dOutCY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cZ", &dOutCZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("dX", &dOutDX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("dY", &dOutDY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("dZ", &dOutDZ);
            XMLCheckResult(eResult);
            
            charOutColor = pListElement->Attribute("color");
            strOutColor = charOutColor;
            charOutMaterial = pListElement->Attribute("material");
            strOutMaterial = charOutMaterial;

            auto tetra = std::shared_ptr<Objet> ( new Tetraedre(
                float3(dOutAX, dOutAY, dOutAZ),
                float3(dOutBX, dOutBY, dOutBZ),
                float3(dOutCX, dOutCY, dOutCZ),
                float3(dOutDX, dOutDY, dOutDZ),
                Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorB()),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getBrightness(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getReflectivity(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getTransparency(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getRefraction() ));

            ObjectList.push_back(tetra);

            pListElement = pListElement->NextSiblingElement("Tetrahedron");
        }
    }

// -----P A R A L L E P I P E D S --------------------------------------------------------------------------

    XMLNode *pPara = pObject->FirstChildElement("Parallelepipeds");
    pListElement = pPara->FirstChildElement("Parallelepiped");

    if (pListElement != nullptr) {
        double dOutAX, dOutAY, dOutAZ, dOutBX, dOutBY, dOutBZ, dOutCX, dOutCY, dOutCZ, dOutDX, dOutDY, dOutDZ;
        const char * charOutMaterial;
        const char * charOutColor;
        std::string strOutMaterial, strOutColor;

        while (pListElement != nullptr) {
            eResult = pListElement->QueryDoubleAttribute("aX", &dOutAX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("aY", &dOutAY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("aZ", &dOutAZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bX", &dOutBX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bY", &dOutBY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("bZ", &dOutBZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cX", &dOutCX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cY", &dOutCY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("cZ", &dOutCZ);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("dX", &dOutDX);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("dY", &dOutDY);
            XMLCheckResult(eResult);
            eResult = pListElement->QueryDoubleAttribute("dZ", &dOutDZ);
            XMLCheckResult(eResult);
            
            charOutColor = pListElement->Attribute("color"); // cont char *
            strOutColor = charOutColor; // cast
            charOutMaterial = pListElement->Attribute("material"); // cont char *
            strOutMaterial = charOutMaterial; // cast

            auto para = std::shared_ptr<Objet> (new Parallelepipede(
                float3(dOutAX, dOutAY, dOutAZ),
                float3(dOutBX, dOutBY, dOutBZ),
                float3(dOutCX, dOutCY, dOutCZ),
                float3(dOutDX, dOutDY, dOutDZ),
                Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorB()),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getBrightness(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getReflectivity(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getTransparency(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getRefraction() ));

            ObjectList.push_back(para);

            pListElement = pListElement->NextSiblingElement("Parallelepiped");
        }
    }

// -----S O L I D S (made out of triangles) ----------------------------------------------------------------

    XMLNode *pSolid = pObject->FirstChildElement("Solids");
    XMLElement *pListSolidElement = pSolid->FirstChildElement("Solid");
    std::vector<std::shared_ptr<Triangle>> TriangleList;

    while (pListSolidElement != nullptr)
    { // loop on the solids
        pListElement = pListSolidElement->FirstChildElement("Triangle");
        if (pListElement != nullptr)
        {
            double dOutAX, dOutAY, dOutAZ, dOutBX, dOutBY, dOutBZ, dOutCX, dOutCY, dOutCZ;
            const char * charOutMaterial;
            const char * charOutColor;
            std::string strOutMaterial, strOutColor;

            while (pListElement != nullptr) {
                eResult = pListElement->QueryDoubleAttribute("aX", &dOutAX);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("aY", &dOutAY);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("aZ", &dOutAZ);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("bX", &dOutBX);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("bY", &dOutBY);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("bZ", &dOutBZ);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("cX", &dOutCX);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("cY", &dOutCY);
                XMLCheckResult(eResult);
                eResult = pListElement->QueryDoubleAttribute("cZ", &dOutCZ);
                XMLCheckResult(eResult);
                
                charOutColor = pListElement->Attribute("color");
                strOutColor = charOutColor;
                charOutMaterial = pListElement->Attribute("material");
                strOutMaterial = charOutMaterial;

                auto triangle = std::shared_ptr<Triangle> (new Triangle(
                    float3(dOutAX, dOutAY, dOutAZ),
                    float3(dOutBX, dOutBY, dOutBZ),
                    float3(dOutCX, dOutCY, dOutCZ) ));

                TriangleList.push_back( triangle );

                pListElement = pListElement->NextSiblingElement("Triangle");
            }

            auto solid = std::shared_ptr<Objet> (new Solide(
                TriangleList,
                Color(ColorList[ mapOfColors[ strOutColor ] ]->getColorR(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorG(),
                      ColorList[ mapOfColors[ strOutColor ] ]->getColorB()),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getBrightness(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getReflectivity(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getTransparency(),
                MaterialList[ mapOfMaterials[ strOutMaterial ] ]->getRefraction() ));

            ObjectList.push_back(solid);

            pListSolidElement = pListSolidElement->NextSiblingElement("Solid");

        }
    }

    return ObjectList;
}

#endif