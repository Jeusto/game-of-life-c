/**
 * \file grille.c
 * \brief Gestion des grilles dont l'allocation, la libération de mémoire et
 * charger une grille depuis un fichier \author Saday Arhun
 */

#include "grille.h"

#include "jeu.h"

int init_grille_from_file(char *fichier_grille, grille *g) {
  FILE *pfile = NULL;
  pfile = fopen(fichier_grille, "r");
  if (pfile == NULL) return 1;

  int i, j, n, l, c, vivantes = 0, non_viable = 0, m, x, y;

  if (fscanf(pfile, "%d", &l) == 0) {
    printf("Erreur : Impossible de lire le fichier\n");
    exit(-1);
  }
  if (fscanf(pfile, "%d", &c) == 0) {
    printf("Erreur : Impossible de lire le fichier\n");
    exit(-1);
  }

  alloue_grille(l, c, g);

  if (fscanf(pfile, "%d", &vivantes) == 0) {
    printf("Erreur : Impossible de lire le fichier\n");
    exit(-1);
  }
  for (n = 0; n < vivantes; ++n) {
    if (fscanf(pfile, "%d", &i) == 0) {
      printf("Erreur : Impossible de lire le fichier\n");
      exit(-1);
    }
    if (fscanf(pfile, "%d", &j) == 0) {
      printf("Erreur : Impossible de lire le fichier\n");
      exit(-1);
    }
    set_vivante(i, j, *g);
  }

  if (fscanf(pfile, "%d", &non_viable) == 0) {
    printf("Erreur : Impossible de lire le fichier\n");
    exit(-1);
  }
  for (m = 0; m < non_viable; ++m) {
    if (fscanf(pfile, "%d", &x) == 0) {
      printf("Erreur : Impossible de lire le fichier\n");
      exit(-1);
    }
    if (fscanf(pfile, "%d", &y) == 0) {
      printf("Erreur : Impossible de lire le fichier\n");
      exit(-1);
    }
    set_non_viable(x, y, *g);
  }

  fclose(pfile);
  return 0;
}

void alloue_grille(int l, int c, grille *g) {
  if ((g->cellules = (int **)malloc(l * sizeof(int *))) == NULL) {
    printf("Erreur : Memoire non allouée\n");
    exit(-1);
  }
  for (int i = 0; i < l; i++) {
    if ((g->cellules[i] = (int *)malloc(c * sizeof(int))) == NULL) {
      printf("Erreur : Memoire non allouée\n");
      exit(-1);
    }
  }

  g->nbl = l;
  g->nbc = c;

  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      set_morte(i, j, *g);
    }
  }
}

void libere_grille(grille *g) {
  for (int i = 0; i < g->nbl; i++) {
    free(g->cellules[i]);
    g->cellules[i] = NULL;
  }

  free(g->cellules);
  g->cellules = NULL;
}

void copie_grille(grille gs, grille gd) {
  int i, j;
  for (i = 0; i < gs.nbl; ++i)
    for (j = 0; j < gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
  return;
}

int grilles_egales(grille *g1, grille *g2) {
  int i, j;
  for (i = 0; i < g1->nbl; i++) {
    for (j = 0; j < g1->nbc; j++) {
      if ((!est_vivante(i, j, *g1) && est_vivante(i, j, *g2)) ||
          (est_vivante(i, j, *g1) && !est_vivante(i, j, *g2))) {
        return 0;
      }
    }
  }
  return 1;
}

int grille_vide(grille *g) {
  int i, j;
  for (i = 0; i < g->nbl; i++) {
    for (j = 0; j < g->nbc; j++) {
      if (g->cellules[i][j] > 0) {
        return 0;
      }
    }
  }
  return 1;
}

int grille_oscillante(grille *g,
                      int (*compte_voisins_vivants)(int, int, grille),
                      int vieillissement_bool) {
  int temps_evolution_oscillation = 0;
  grille grille_copiee, grille_copiee_modifiee, tmp;
  alloue_grille(g->nbl, g->nbc, &grille_copiee);
  copie_grille(*g, grille_copiee);

  alloue_grille(g->nbl, g->nbc, &grille_copiee_modifiee);
  copie_grille(*g, grille_copiee_modifiee);

  alloue_grille(g->nbl, g->nbc, &tmp);
  copie_grille(*g, tmp);

  int max_interval = 1500;
  int max_delai = 1500;
  int i = 0;

  do {
    while (temps_evolution_oscillation < max_interval) {
      evolue(&grille_copiee_modifiee, &tmp, &temps_evolution_oscillation,
             compte_voisins_vivants, vieillissement_bool);
      if (grille_vide(&grille_copiee)) {
        libere_grille(&grille_copiee);
        libere_grille(&grille_copiee_modifiee);
        libere_grille(&tmp);
        return 0;
      }
      if (grilles_egales(&grille_copiee, &grille_copiee_modifiee)) {
        libere_grille(&grille_copiee);
        libere_grille(&grille_copiee_modifiee);
        libere_grille(&tmp);
        return temps_evolution_oscillation;
      }
    }

    evolue(&grille_copiee, &tmp, &temps_evolution_oscillation,
           compte_voisins_vivants, vieillissement_bool);
    copie_grille(grille_copiee, grille_copiee_modifiee);
    temps_evolution_oscillation = 0;
    i++;
  } while (i < max_delai);
  libere_grille(&grille_copiee);
  libere_grille(&grille_copiee_modifiee);
  libere_grille(&tmp);
  return 0;
}
