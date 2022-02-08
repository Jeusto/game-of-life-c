/**
 * \file main.c
 * \brief Contient la fonction main
 * \author Saday Arhun
 */

#include <X11/Xlib.h>
#include <cairo-xlib.h>
#include <cairo.h>
#include <stdio.h>
#include <string.h>

#include "grille.h"
#include "io.h"
#include "jeu.h"

/*
TEXT MODE
*/
#if MODETEXTE

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Utilisation : bin/main <fichier grille>\n");
    return 1;
  }

  grille g, gc;
  init_grille_from_file(argv[1], &g);
  alloue_grille(g.nbl, g.nbc, &gc);
  affiche_grille(g);

  debut_jeu(&g, &gc);

  libere_grille(&g);
  libere_grille(&gc);

  return 0;
}

/*
CAIRO MODE
*/
#else

cairo_surface_t *surface;

int main(int argc, char **argv) {
  int erreur = 0;
  if (argc != 2) {
    printf("Utilisation : bin/main <fichier grille>\n");
    return 1;
  }

  grille g, gc;

  char fichier_grille[100] = "";
  strcat(fichier_grille, argv[1]);
  erreur = init_grille_from_file(fichier_grille, &g);
  if (erreur) {
    printf(
        "Erreur : Le fichier est introuvable, veuillez en choisir un autre\n");
    printf("Utilisation : bin/main <fichier grille>\n");
    return 1;
  }
  alloue_grille(g.nbl, g.nbc, &gc);
  surface = cairo_creer_surface(SIZEX, SIZEY);

  debut_jeu(&g, &gc);
  cairo_fermer_surface(surface);

  libere_grille(&g);
  libere_grille(&gc);

  return 0;
}

#endif
