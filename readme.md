il ya plusieurs partie ,la partie interface graphique quon devra faire a la fin et la partie
scene qui aura la gestion des objects(point lumineux ,spot lumineux,object (forme simple))

### scene
notre scene sera composer des objects suivant : 
    Des objets opaques (qui bloquent la lumière → créent des ombres)
    Des objets visuels (sprites, textures, décor)
    Des lumières (sources lumineuses colorées)
    Des points de collision / sommets

# parmis les objects opaque on peut avoir:
    1- les rectangles {
        identifier par leur position et largeur et hauteur,puis couleur dans la suite
    }
    2- triangle {
        identifier par leur postion et leur longueur de cote puis couleur dans la suite(ce sont des triangle equilateral)
    }
    3- cercles{
        identifier par la position du centre et la longueur du rayon,puis ensuite par la couleur
    }
# parmis les objects visuel on a :
    (on en reparlera apres la conception du systeme de lumiere basique avec les formes)
# au niveau des lumieres nous avons:
    deux type de lumiere ici
    1 - spot ligth {
        c'est une lumiere qui a un angle precis,il est caracterise par sa positon ,son angle d'inclinaison,son angle ,son intenssite son rayon et la couleur
    }
    2 - point ligth{
        cest un splot ligth en 360,ici on aura juste besoin de la position, du rayon de lintensite et la couleur
    }
# le sol de base sera noir mais
    on peut y ajouter une autre couleur ou une texture ou un decore ou meme un grille



### UI - SCENE

### UI - panneau de controle
