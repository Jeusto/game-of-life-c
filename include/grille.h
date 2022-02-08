/**
 * \file grille.h
 * \brief Header contenant les fonctions du fichier grille.c
 * \author Saday Arhun
 */

#ifndef __GRILLE_H
#define __GRILLE_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <cairo-xlib.h>
#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * \struct grille
 * \brief Structure grille: Contient le nombre de lignes et le nombre de
 * colonnes de la grille ainsi qu'un "tableau de tableau de cellules"
 * représentant la grille \typedef struct grille
 */
typedef struct {
  int nbl;         //!< Nombre de lignes
  int nbc;         //!< Nombre de colonnes
  int** cellules;  //!< Tableau de tableau de cellules (deux dimensions)
} grille;

/**
 * \brief Alloue et initalise la grille g à partir d'un fichier
 * \param fichier_grille - Pointeur vers le nom du fichier correspondant à la
 * grille \param g - Pointeur vers une structure grille
 */
int init_grille_from_file(char* fichier_grille, grille* g);

/**
 * \brief Alloue un espace mémoire pocycliqueur une grille de taille l*c, et
 * initialise toutes les cellules à 0 \param l - Nombre de lignes \param c -
 * Nombre de colonnes \param g - Pointeur vers une structure grille
 */
void alloue_grille(int l, int c, grille* g);

/**
 * \brief Libère l'espace mémoire pour une grille donnée
 * \param g - Pointeur vers une structure grille
 */
void libere_grille(grille* g);

/**
 * \brief Recopie la grille gs dans gd (sans allocation)
 * \param gs - La grille originale
 * \param gd - La copie de la grille
 */
void copie_grille(grille gs, grille gd);

/**
 * \brief Vérifie si une grille est vide
 * \param g - Pointeur sur la grille à tester
 */
int grille_vide(grille* g);

/**
 * \brief Vérifie si les grilles sont égales
 * \param g1 - Pointeur sur la première grille
 * \param g2 - Pointeur sur la deuxième grille
 */
int grilles_egales(grille* g1, grille* g2);

/**
 * \brief Vérifie si une grille est oscillante
 * \param g - Pointeur sur la grille à tester
 * \param compte_voisins_vivants - Pointeur sur la fonction qui compte les
 * voisins vivants \param vieillissement_bool - Indique l'état du mode
 * vieillissement (1 si activé, 0 sinon)
 */
int grille_oscillante(grille* g,
                      int (*compte_voisins_vivants)(int, int, grille),
                      int vieillissement_bool);

/**
 * \brief Rend vivante la cellule à la case indiqué de la grille
 * \param i - Indice ligne de la grille
 * \param j - Indice colonne de la grille
 * \param g - Structure grille
 */
static inline void set_vivante(int i, int j, grille g) { g.cellules[i][j] = 1; }

/**
 * \brief Rend morte la cellule à la case indiqué de la grille
 * \param i - Indice ligne de la grille
 * \param j - Indice colonne de la grille
 * \param g - Structure grille
 */
static inline void set_morte(int i, int j, grille g) { g.cellules[i][j] = 0; }

/**
 * \brief Rend temporairement morte la cellule
 * \param i - Indice ligne du tableau
 * \param j - Indice colonne du tableau
 * \param g - La grille
 */
static inline void set_temporaire_morte(int i, int j, grille g) {
  g.cellules[i][j] = 11;
}

/**
 * \brief Rend temporairement vivante la cellule
 * \param i - Indice ligne du tableau
 * \param j - Indice pour la colonne du tableau
 * \param g - La grille
 */
static inline void set_temporaire_vivante(int i, int j, grille g) {
  g.cellules[i][j] = 9;
}

/**
 * \brief Rend une case inhabitable (ne peut pas accueilir de cellules)
 * \param i - Indice ligne du tableau
 * \param j - Indice colonne du tableau
 * \param g - La grille
 */
static inline void set_non_viable(int i, int j, grille g) {
  g.cellules[i][j] = -1;
}

/**
 * \brief Teste si la cellule à la case indiqué de la grille est vivante
 * \param i - Indice ligne de la grille
 * \param j - Indice colonne de la grille
 * \param g - Structure grille
 */
static inline int est_vivante(int i, int j, grille g) {
  return g.cellules[i][j] >= 1 && g.cellules[i][j] != 9;
}

/**
 * \brief Teste si une case est inhabitable (ne peut pas accueilir des cellules)
 * \param i - Indice ligne du tableau
 * \param j - Indice colonne du tableau
 * \param g - La grille
 */
static inline int est_non_viable(int i, int j, grille g) {
  return g.cellules[i][j] == -1;
}

#endif
