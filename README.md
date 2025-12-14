## 💡 RayCast-Simulation-2D : Moteur de Rendu Lumineux Minimaliste

**[[Lien de votre Dépôt GitHub ici]](https://github.com/junguck/RayCast-Simulation-2D-Moteur-de-Rendu-Lumineux-Minimaliste/**

Simulation interactive et didactique des interactions lumière-matière en 2D, développée en C++ en utilisant la bibliothèque SDL3. Ce projet implémente les algorithmes fondamentaux du **Ray Casting** pour calculer l'éclairage par pixel, offrant une compréhension concrète du rendu graphique à bas niveau.

-----

## ✨ Fonctionnalités Implémentées

Ce moteur de rendu minimaliste simule les principaux phénomènes d'éclairage :

  * **Rendu Pixel par Pixel :** Utilisation du **Ray Casting** pour déterminer la couleur de chaque point de l'écran.
  * **Modèle d'Éclairage de Phong :** Implémentation des trois composantes de la lumière :
      * **Ambiante :** Éclairage de base uniforme.
      * **Diffuse (Lambertienne) :** Simulation de la lumière réfléchie uniformément, basée sur $\mathbf{N} \cdot \mathbf{L}$.
      * **Spéculaire (Brillance) :** Création de reflets brillants pour les surfaces lisses, basée sur la formule de Phong.
  * **Ombres Dures (*Shadow Casting*) :** Calcul de l'occlusion des sources lumineuses par les objets.
  * **Géométrie 2D :** Support des objets simples (Cercles et Rectangles) avec des algorithmes d'intersection optimisés.
  * **Interactivité :** Déplacement en temps réel de la source lumineuse principale via la souris.
  * **Multi-Lumières :** Le moteur est capable de gérer et d'accumuler les contributions de plusieurs sources lumineuses.

-----

## 🛠️ Technologies Utilisées

| Technologie | Rôle dans le Projet |
| :--- | :--- |
| **C++ 17/20** | Langage principal pour la performance et l'implémentation des algorithmes d'éclairage. |
| **SDL3** | Bibliothèque de bas niveau pour la gestion de la fenêtre, des événements, et l'affichage des pixels (rendu 2D). |
| **CMake** | Système de construction pour la gestion des dépendances et la compilation multiplateforme. |
| **Python** | Utilisé pour le script d'automatisation de la compilation (`compile.py`). |

-----

## 🚀 Démarrage Rapide

Ce projet utilise CMake pour la compilation. Assurez-vous d'avoir installé CMake et la bibliothèque SDL3 sur votre système.

### 1\. Cloner le Dépôt

```bash
git clone https://github.com/votre_nom_utilisateur/RayCast-Simulation-2D.git
cd RayCast-Simulation-2D
```

### 2\. Compilation et Exécution (Recommandée)

Le fichier `compile.py` automatise les étapes de configuration et de construction du projet :

```bash
# Assurez-vous d'avoir Python installé et dans votre PATH
python compile.py
```

Le script :

1.  Configure le projet avec CMake (crée le dossier `build/`).
2.  Compile les sources C++ (`src/`).
3.  Exécute l'application générée.

### 3\. Compilation Manuelle (Alternative)

Si vous préférez compiler manuellement :

```bash
# 1. Configuration de CMake
cmake -S . -B build

# 2. Compilation (l'option --build est multiplateforme)
cmake --build build

# 3. Exécution (le chemin peut varier légèrement selon l'OS)
./build/RayCast_Simulation
```

-----

## 📐 Architecture du Code

L'architecture est modulaire pour séparer clairement les responsabilités (Mathématiques, Scène, Rendu).

  * **`src/Math.h`** : Contient les structures fondamentales (`Vector2D`, `Color`) et les fonctions vectorielles (produit scalaire, normalisation).
  * **`src/Object.h`** : Définit l'interface `Object` avec les méthodes virtuelles `getIntersection()` et `getNormal()`, assurant le polymorphisme.
  * **`src/Scene.cpp`** : Le gestionnaire de la scène, responsable de parcourir tous les objets pour trouver le point d'impact le plus proche pour chaque rayon.
  * **`src/Renderer.cpp`** : Contient la boucle d'itération par pixel. C'est ici que tous les calculs d'éclairage (Phong, Ombres) sont effectués.

-----

## 📚 Ressources et Crédits

Ce projet est fortement inspiré par les travaux fondamentaux en *Ray Tracing* et *Ray Casting*.

  * **SDL3 Documentation :** [https://www.libsdl.org/download-3.0.php](https://www.google.com/search?q=https://www.libsdl.org/download-3.0.php)
  * **Principes de Rendu (Phong) :** [https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-and-physically-based-shading/intro-phong-model.html](https://www.google.com/search?q=https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-and-physically-based-shading/intro-phong-model.html)

**Auteur :** JUN GUCK NGAPMOU BRYAN
**Statut :** EN COURS
**Licence :** MIT License 
