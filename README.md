
# IN204
Ceci fait partie de notre projet de moteur de rendu 3D pour le cours d'IN204.
Il s'agit d'un programme générant une scène 3D à partir d'un fichier XML et la sauvant en une image bitmap.

Ceci est le manuel d'utilisateur.
## Manuel Utilisateur de notre moteur 3D
Plan :
1. Description d'une scène en XML
2. Compilation du programme et choix de la scène à lire

### 1. Description d'une scène en XML

Toute la scène est décrite dans un fichier XML intitulé par défaut `sceneDesciption.xml`.

Sa syntaxe est très peu flexible, il faut donc bien respecter la manière de déclarer votre scène et en particulier veiller à ne pas retirer les balises désignant des groupes d'objets du même type, comme par exemple la balise `<Spheres>`, au risque de causer une erreur de `Segmentation fault` . Cependant, vous pouvez omettre la balise `<Sphere>`à l'intérieur si votre scène ne comporte pas de sphères.

Voyons en détail les balises devant figurer dans votre XML et comment les compléter.

1. **La balise `<Root>`**

Elle ne doit pas être ommise.

2. **Les balises `<Width>`, `<Height>` et `<PictureToCameraDistance>`**

Ces balises concernent l'image.

La balise `<Width>`réfère à la largeur de l'image en pixels et contient un entier naturel.
La balise `<Height>`réfère à la hauteur de l'image en pixels et contient un entier naturel.


La balise `<PictureToCameraDistance>`est un double exprimant la distance entre la caméra et l'image (voir Figure 1). d=1 correspond à un angle de vue d'environ 53°.

![alt text](https://github.com/ErwanBilien/IN204/blob/xml_support/fig1.png?raw=true)


Exemple :

  `<Width>500</Width>` 

  `<Height>500</Height>`
  
  `<PictureToCameraDistance>1.0</PictureToCameraDistance>`
  
  
3. **La balise `<Camera>`**

Il faut renseigner les trois coordonnées cartésiennes de la position de la caméra, ainsi que son orientation avec un triplet d'angles (en radians) correspondant aux rotations autour des axes Ox, Oy et Oz.
Toutes les valeurs sont des doubles.

Exemple :

`<Camera xCentre="7" yCentre="1" zCentre="2" theta="1" phi="-1" psi="-1"/>`

4. **La balise `<Lights>`**

Cette balise doit figurer ainsi que toutes ses balises filles.
Toutes les lumières possèdent une intensité lumineuse (flottant compris entre 0 et 1) ainsi qu'une couleur. 
**Les lumières ambiantes** : ces lumières s'appliquent à tous les objets indépendamment de leur géométrie et de leur position. Elles permettent de discerner un objet non éclairé. 
Exemple :

    <Ambiant>
        <AmbiantLight intensity="0.05" color="WHITE"/>
    </Ambiant>
**Les lumières directionnelles** : ce sont des lumières provenant de l'infini (par exemple la lumière du soleil) . Elles sont définies par leur direction.
Exemple :

    <Directional>
        <DirectionalLight intensity="0.7" xDirection="4" yDirection="4" zDirection="-4" color="WHITE"/>
    </Directional>
**Les lumières ponctuelles** : ces lumières proviennent d'un unique point lumineux à renseigner pour chaque lumière ponctuelle.
Exemple :

    <Punctual>
        <PunctualLight intensity="0.6" xCentre="0" yCentre="5" zCentre="0" color="WHITE"/>
    </Punctual>
**Les spots** : ce sont des cônes de lumière, paramétrés par le point d'origine des rayons lumineux, la direction du cône et l'angle du cône (en radiant).
Exemple :

    <SpotLights>
                <SpotLight intensity="0.9" xCentre="-1" yCentre="2.5" zCentre="0" xDirection="1" yDirection="-1" zDirection="0" angle="0.52" color="WHITE"/>
    </SpotLights>

5. **La balise `<Materials>`**`

Cette balise doit figurer.
Les matériaux sont décrits par leur nom et quatre flottants : Le champ "brightness" (réel positif) correspond à la brillance de l'objet : plus il augmente, plus l'éclairage par diffusion spéculaire sera focalisé et intense. "reflectivity" et "transparency" (réels de [0,1]) sont respectivement la part de lumière réfléchie  et la part de lumière réfractée. La somme de ces deux champs doit être inférieure à 1. Finalement,  "refraction" est l'indice optique du matériau (ou indice de réfraction).
Exemple :

    <Materials>
        <Material materialName="GLASS"   brightness="50"   reflectivity="0.1"  transparency="0.8" refraction="1.03"/>
        <Material materialName="METAL"   brightness="10"   reflectivity="0.4"  transparency="0"   refraction="1"/>
    </Materials>

7. **La balise `<Colors>`**

Cette balise doit figurer.
Chaque couleur est définie par son nom et son code RGB en décimal.
Exemple:
    
    <Colors>
            <Color colorName="RED"    redColor="1"   greenColor="0"   blueColor="0"/>
            <Color colorName="BLUE"   redColor="0"   greenColor="0"   blueColor="1"/>
    </Colors>

7. **La balise `<Objects>`**

Cette balise doit figurer ainsi que toutes ses balises filles.
Les **sphères** sont décrites par leur centre (xCentre,yCentre,zCentre), leur rayon Rayon_S ainsi qu'une couleur et un matériau
Exemple :

    <Spheres>
                <Sphere Rayon_S="1"    xCentre="-3"  yCentre="0"     zCentre="4" color="RED" 
                <Sphere Rayon_S="1"    xCentre="0" yCentre="0"     zCentre="4" color="GREEN"   material="METAL_2"/>    material="METAL"/>
    </Spheres>
Les **plans** sont définis par trois points par lesquels passe le plan.
Exemple : 

    <Plans>
        <Plan aX="1"  aY="-1" aZ="0" bX="0"  bY="-1" bZ="0" cX="0" cY="-1" cZ="1" color="WHITE"  material="METAL"/>
    </Plans>
Il existe plusieurs manières de déclarer des **solides à facettes** (des formes avec des faces définis par un ensemble de triangles). La première consiste à utiliser des formes prédéfinies (pour le moment, seul les tétraèdres et les parallélépipèdes sont fournis). Les tétraèdres sont simplement définis par leurs quatre sommets.
Exemple :

    <Tetrahedrons>
        <Tetrahedron aX="1"  aY="2" aZ="-2" bX="-1"  bY="2" bZ="1" cX="-2" cY="2" cZ="-1" dX="-2" dY="2" dZ="0" color="YELLOW" material="GLASS"/>
    </Tetrahedrons>
Pour définir un parallélépipède, vous devez indiquer un sommets d'origine et 3 arrètes. Il est crucial de se référer à la figure 2 pour bien déclarer le parallélépipède : les coordonnées (aX,aY,aZ) correspondent à l'origine, (bX,bY,bZ) à l'arrête 1, (cX,cY,cZ) à l'arrête 2 et (dX,dY,dZ) à l'arrête 3.

![alt text](https://github.com/ErwanBilien/IN204/blob/xml_support/fig2.png?raw=true)

Exemple :

    <Parallelepipeds>
        <Parallelepiped aX="-2"  aY="-1" aZ="0" bX="1"  bY="0" bZ="0" cX="0" cY="0" cZ="1" dX="0"  dY="2" dZ="0" color="BROWN" material="PLASTIC"/>
    </Parallelepipeds>
Pour les autres solides, vous devrez entrer chaque triangle le composant, ceux-ci étant définis par leurs trois sommets. Seul le matériau et la couleur du dernier triangle seront pris en compte.
Exemple :

    <Solids>
        <Solid>
            <Triangle aX="3"  aY="0" aZ="4" bX="1"  bY="0" bZ="4.5" cX="3" cY="3" cZ="4" color="WHITE" material="MIRROR"/>
            <Triangle aX="1"  aY="0" aZ="4.5" bX="1"  bY="3" bZ="4.5" cX="3" cY="3" cZ="4" color="WHITE" material="MIRROR"/>    
        </Solid>
    </Solids>

### 2. Compilation du programme et choix de la scène à lire

Pour utiliser le moteur de rendu, clonez la branche de rendu 

    git clone -b in204_3D_rendu https://github.com/ErwanBilien/IN204.git
   
   Compilez le programme

    g++ -fopenmp -o raytracer main.cpp tinyxml2.cpp

   
Executez le binaire avec en argument le nom de votre fichier de description de scène et le nom que vous souhaitez donner à votre image .L'exemple suivant charge le fichier de description de scène "sceneDescription3.xml" et enregistre le rendu sous le nom "scene_3"

    ./raytracer sceneDescription3.xml scene_3
Si aucun nom d'image n'est fourni, votre rendu se nommera par défaut"raytracing_Image.bmp"