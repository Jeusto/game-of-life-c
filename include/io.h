/**
 * \file io.h
 * \brief Header contenant les fonctions du fichier io.c
 * \author Saday Arhun
 */

#ifndef __IO_H
#define __IO_H
#include <stdio.h>
#include <string.h>

#include "grille.h"
#include "jeu.h"

/*
COMMUN AUX DEUX MODES
*/

/**
 * \brief Débute le jeu
 * \param g - Pointeur vers une grille
 * \param gc - Pointeur vers une copie de g
 */
void debut_jeu(grille *g, grille *gc);

/*
TEXT MODE
*/
#if MODETEXTE

/**
 * \brief Affiche une grille
 * \param g - Structure grille à afficher
 */
void affiche_grille(grille g);

/**
 * \brief Efface une grille
 * \param g - Grille à effacer
 */
void efface_grille(grille g);

/**
 * \brief Affiche un trait horizontal de la grille
 * \param c - Nombre total de colonnes
 */
void affiche_trait(int c);

/**
 * \brief Affiche une ligne verticale de la grille
 * \param c - Nombre total de colonnes
 * \param ligne - Pointeur vers le nombre de lignes (tableau d'entiers)
 */
void affiche_ligne(int c, int *ligne);

/**
 * \brief Affiche le temps d'évolution
 */
void affiche_temps_evolution();

/**
 * \brief Affiche si le mode cyclique est activé ou non
 */
void affiche_mode_cyclique();

/**
 * \brief Affiche si le vieillissement est activé ou non
 */
void affiche_mode_vieillissement();

/**
 * \brief Affiche le test d'oscillation
 */
void affiche_oscillation();

/*
CAIRO MODE
*/
#else

#define SIZEX 1300
#define SIZEY 800
#define MG_GAUCHE 550
#define MG_HAUTE 50
#define LARGEUR_GRILLE 700
#define HAUTEUR_GRILLE 700
#define CAIRO_LINE_WIDTH 2

/**
 * \brief Crée une surface Cairo
 * \param x - Largeur (en pixels)
 * \param y - Hauteur (en pixels)
 */
cairo_surface_t *cairo_creer_surface(int x, int y);

/**
 * Supprime et ferme la surface Cairo
 */
void cairo_fermer_surface();

/**
 * \brief Affiche une entrée de texte (mode cairo)
 * \param input - Saisie au clavier à afficher
 * \param erreur - Erreur à afficher
 */
void cairo_affiche_texte(char *input, char *erreur);

/**
 * \brief Affiche une grille (mode cairo)
 * \param g - Structure grille à afficher
 * \param temps_evolution - Indique le temps d'évolution de la grille
 * \param cyclique_bool - Indique l'état du mode cyclique (1 si activé, 0 sinon)
 * \param vieillissement_bool - Indique l'état du mode vieillissement (1 si
 * activé, 0 sinon) \param temps_oscillation - Indique le temps d'oscillation de
 * la grille (-1 si non testé, 0 si non oscillante)
 */
void affiche_grille(grille g, int temps_evolution, int cyclique_bool,
                    int vieillissement_bool, int temps_oscillation);

/**
 * \brief Efface une grille (mode cairo)
 */
void efface_grille();

/**
 * \brief Affiche un trait horizontal de la grille (mode cairo)
 * \param c - Nombre de cellules
 * \param hauteur - Numéro de la hauteur du trait à tracer
 * \param taille_ligne - Hauteur de la ligne (en pixels)
 */
void affiche_trait(int c, int hauteur, float taille_ligne);

/**
 * \brief Affiche une ligne de la grille (mode cairo)
 * \param c - Nombre de cellules
 * \param ligne - Tableau de cellules vivantes ou mortes
 * \param vieillissement_bool - Indique l'état du mode vieillissement (1 si
 * activé, 0 sinon) \param hauteur - Numéro de la hauteur du trait à tracer
 * \param taille_ligne - Hauteur de la ligne (en pixels)
 */
void affiche_ligne(int c, int *ligne, int vieillissement_bool, int hauteur,
                   float taille_ligne);

#endif
#endif
