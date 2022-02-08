# Implentation en C du jeu de la vie (ancien projet étudiant)

Le principe du jeu de la vie est simple. L’univers est défini sur une grille à deux dimensions, de taille variable, ou chaque case est une cellule pouvant prendre deux états différents: morte ou vivante. Le passage d’un état à l’autre est guidé par les règles d'évolution suivantes:

- Une cellule morte au temps t devient vivante au temps t + 1 si et seulement si elle a exactement 3
  cellules vivantes dans son voisinage.
- Une cellule vivante au temps t reste vivante au temps t + 1 si et seulement si elle a exactement 2
  ou 3 cellules vivantes dans son voisinage, sinon elle meurt.
- Le voisinage utilise est le 8-voisinage : pour une cellule donnée, ses voisines sont les 8 cellules
  qui l’entourent.

# Commandes importantes

### Mode graphique Cairo

Compiler

```
make
```

Lancer le jeu

```
bin/main <fichier grille>
```

Exemple

```
bin/main grilles/grille4.txt
```

### Mode texte

Compiler

```
make MODE=TEXTE
```

Lancer le jeu

```
bin/main <fichier grille>
```

Exemple

```
bin/main grilles/grille4.txt
```

# Touches importantes du programme

### Mode graphique Cairo

Faire évoluer la grille

```
entrée ou clic gauche
```

Activer/désactiver le mode cyclique

```
c
```

Activer/désactiver le vieillissement

```
v
```

Afficher la période d'oscillement

```
o
```

Quitter le programme

```
q ou clic droit
```

Charger une nouvelle grille

```
n puis <fichier grille>
```

Annuler le changement de grille

```
touche échap
```

### Mode texte

Faire evoluer la grille

```
entrée
```

Activer/désactiver le mode cyclique

```
c
```

Activer/désactiver le vieillissement

```
v
```

Quitter le programme

```
q
```

Charger une nouvelle grille

```
n puis <fichier grille>
```

Annuler le changement de grille

```
touche échap
```

# Générer la documentation

Le paquet Doxygen est requis pour générer la documentation

```
make docs
```

# Générer une archive (tarball)

```
make dist
```

# Nettoyer les artefacts de compilation et l'archive

```
make clean
```
