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
Afin de limiter les temps de calcul, mieux vaut ne pas dépasser 1500 pixels pour ces deux valeurs (ce qui prend déjà 8 secondes pour rendre la scène sur nos ordinateurs).

La balise `<PictureToCameraDistance>`est un double exprimant la distance entre la caméra et l'image (voir Figure 1).

Exemple :

  `<Width>500</Width>` 

  `<Height>500</Height>`
  
  `<PictureToCameraDistance>1.0</PictureToCameraDistance>`
  
  
3. **La balise `<Camera>`**

Il faut renseigner les trois coordonnées cartésiennes de la position de la caméra, ainsi que son orientation avec un triplet d'angles.
Toutes les valeurs sont des doubles.

Exemple :

`<Camera xCentre="7" yCentre="1" zCentre="2" theta="1" phi="-1" psi="-1"/>`

4. **La balise `<Lights>`**

Cette balise doit figurer ainsi que toutes ses balises filles.

5. **La balise `<Materials>`**

Cette balise doit figurer ainsi que toutes ses balises filles.

6. **La balise `<Colors>`**

Cette balise doit figurer ainsi que toutes ses balises filles.

7. **La balise `<Objects>`**

Cette balise doit figurer ainsi que toutes ses balises filles.

### 2. Compilation du programme et choix de la scène à lire
