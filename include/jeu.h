/**
 * \file jeu.h
 * \brief Header contenant les fonction du fichier jeu.c
 * \author Saday Arhun
 */

#ifndef __JEU_H
#define __JEU_H
#include "grille.h"

/**
 * \brief Calcule les voisins en mode cyclique
 * \param i - Indice de la cellule
 * \param m - Maximum
 */
static inline int modulo(int i, int m) { return (i + m) % m; }

/**
 * \brief Compte le nombre de voisins d'une case (mode cyclique)
 * \param i - Indice ligne du tableau
 * \param j - Indice colonne du tableau
 * \param g - Grille
 */
int compte_voisins_vivants_cyclique(int i, int j, grille g);

/**
 * \brief Compte le nombre de voisins d'une case (mode non cyclique)
 * \param i - Indice ligne du tableau
 * \param j - Indice colonne du tableau
 * \param g - Grille
 */
int compte_voisins_vivants_non_cyclique(int i, int j, grille g);

/**
 * \brief Fait évoluer la grille d'un pas de temps
 * \param g - Pointeur sur la grille
 * \param gc - Pointeur sur la copie d'une grille
 * \param temps_evolution - Pointeur sur le temps d'évolution
 * \param compte_voisins_vivants - Pointeur sur la fonction qui compte les
 * voisins vivants \param vieillissement_bool - Indique l'état du mode
 * vieillissement (1 si activé, 0 sinon)
 */
void evolue(grille *g, grille *gc, int *temps_evolution,
            int (*compte_voisins_vivants)(int, int, grille),
            int vieillissement_bool);

#endif
