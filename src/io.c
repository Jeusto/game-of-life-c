/**
 * \file io.c
 * \brief Gestion des entrées et sorties dont l'affichage des grilles
 * \author Saday Arhun
 */

#include "io.h"

/*
TEXT MODE
*/
#if MODETEXTE

static char fichier_grille[100];
static int vieillissement_bool = 0;
static int cyclique_bool = 1;
static int temps_evolution = 0;
static int oscillation = -1;
static int erreur = 0;
int (*voisins_vivants)(int, int, grille) = compte_voisins_vivants_cyclique;

void affiche_trait(int c) {
  int i;
  for (i = 0; i < c; ++i) printf("|---");
  printf("|\n");
  return;
}

void affiche_ligne(int c, int *ligne) {
  int i;
  for (i = 0; i < c; ++i)
    if (ligne[i] == 0) {
      printf("|   ");
    } else if (ligne[i] == -1) {
      printf("| X ");
    } else {
      if (vieillissement_bool) {
        printf("| %d ", ligne[i]);
      } else {
        printf("| O ");
      }
    }
  printf("|\n");
  return;
}

void affiche_mode_cyclique() {
  if (cyclique_bool)
    printf("Mode cyclique : Activé\n");
  else
    printf("Mode cyclique : Désactivé\n");
}

void affiche_temps_evolution() {
  printf("Temps d'évolution : %d\n", temps_evolution);
}

void affiche_mode_vieillissement() {
  if (vieillissement_bool) {
    printf("Vieillissement : Activé\n");
  } else {
    printf("Vieillissement : Désactivé\n");
  }
}

void affiche_oscillation() {
  if (oscillation == -1) {
    printf("Oscillation : Non testée\n");
  } else if (oscillation == 0) {
    printf("Oscillation : Non oscillant\n");
  } else {
    printf("Oscillation : %d pas/période\n", oscillation);
  }
}

void affiche_grille(grille g) {
  int i, l = g.nbl, c = g.nbc;
  printf("\n");
  affiche_trait(c);
  for (i = 0; i < l; ++i) {
    affiche_ligne(c, g.cellules[i]);
    affiche_trait(c);
  }
  printf("\n");
  affiche_temps_evolution();
  affiche_mode_vieillissement();
  affiche_mode_cyclique();
  affiche_oscillation();
  printf("\n");

  return;
}

void efface_grille(grille g) {
  printf("\n\e[%dA", g.nbl * 2 + 5);
  printf("\e[1;1H\e[2J");
}

void debut_jeu(grille *g, grille *gc) {
  char c = getchar();
  int passer_evolution = 0;
  while (c != 'q') {
    switch (c) {
      case 'o': {
        passer_evolution = 1;
        oscillation =
            grille_oscillante(g, voisins_vivants, vieillissement_bool);
        printf("%d", oscillation);
        efface_grille(*g);
        affiche_grille(*g);
        break;
      }

      case 'v': {
        passer_evolution = 1;
        vieillissement_bool = !vieillissement_bool;
        oscillation = -1;
        efface_grille(*g);
        affiche_grille(*g);
        break;
      }

      case 'c': {
        passer_evolution = 1;
        cyclique_bool = !cyclique_bool;
        if (cyclique_bool) {
          voisins_vivants = &(compte_voisins_vivants_cyclique);
        } else {
          voisins_vivants = &(compte_voisins_vivants_non_cyclique);
        }
        oscillation = -1;
        efface_grille(*g);
        affiche_grille(*g);
        break;
      }

      case 'n': {
        passer_evolution = 1;
        printf("Indiquez le chemin vers la grille à charger : ");
        scanf("%s", fichier_grille);
        init_grille_from_file(fichier_grille, g);
        erreur = init_grille_from_file(fichier_grille, g);
        while (erreur) {
          printf("Fichier de grille introuvable. Essayez à nouveau :\n");
          if (*fichier_grille == 27) {
            break;
          } else {
            scanf("%s", fichier_grille);
            init_grille_from_file(fichier_grille, g);
            erreur = init_grille_from_file(fichier_grille, g);
          }
        }
        if (!erreur) {
          temps_evolution = 0;
          oscillation = -1;
        }
        alloue_grille(g->nbl, g->nbc, gc);
        efface_grille(*g);
        affiche_grille(*g);
        break;
      }
      case '\n': {
        if (passer_evolution == 1) {
          passer_evolution = 0;
        } else if (passer_evolution == 0) {
          evolue(g, gc, &temps_evolution, voisins_vivants, vieillissement_bool);
          efface_grille(*g);
          affiche_grille(*g);
          break;
        }
      }
      default: {
        printf("\n\e[1A");
        break;
      }
    }
    c = getchar();
  }
  return;
}

/*
CAIRO MODE
*/
#else

extern cairo_surface_t *surface;
XClassHint *classHint;

cairo_surface_t *cairo_creer_surface(int x, int y) {
  Display *dpy;
  Drawable win;
  int screen;
  Window rootwin;
  cairo_surface_t *surface;

  if ((dpy = XOpenDisplay(NULL)) == NULL) {
    fprintf(stderr, "Erreur : Affichage impossible\n");
    exit(1);
  }

  screen = DefaultScreen(dpy);
  rootwin = RootWindow(dpy, screen);

  unsigned long background = 0x00172B;
  win =
      XCreateSimpleWindow(dpy, rootwin, 1, 1, x, y, 0, background, background);

  XSelectInput(dpy, win, ExposureMask | ButtonPressMask | KeyPressMask);
  XMapWindow(dpy, win);

  XStoreName(dpy, win, "Jeu de la vie");

  classHint = XAllocClassHint();
  if (classHint) {
    classHint->res_name = "Jeu de la vie";
    classHint->res_class = "Jeu de la vie";
    XSetClassHint(dpy, win, classHint);
    XFree(classHint);
  }
  Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(dpy, win, &wm_delete_window, 1);

  surface =
      cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, screen), x, y);
  cairo_xlib_surface_set_size(surface, x, y);

  return surface;
}

void cairo_fermer_surface() {
  Display *dpy = cairo_xlib_surface_get_display(surface);
  cairo_surface_destroy(surface);
  XCloseDisplay(dpy);
}

void affiche_ligne(int c, int *ligne, int vieillissement_bool, int hauteur,
                   float taille_ligne) {
  int i;
  char age_cellule[255];
  float taille_colonne = (float)LARGEUR_GRILLE / (float)c;

  cairo_t *cr, *cairo_cellules;
  cr = cairo_create(surface);
  cairo_cellules = cairo_create(surface);

  cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
  cairo_set_source_rgb(cairo_cellules, 0.9, 0.9, 0.9);
  cairo_set_line_width(cr, CAIRO_LINE_WIDTH);

  cairo_line_to(cr, MG_GAUCHE, MG_HAUTE + (hauteur * 5));

  for (i = 0; i < c; ++i) {
    if (ligne[i] == 0) {
      cairo_line_to(cr, MG_GAUCHE + i * taille_colonne,
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_move_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne));
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
    } else if (ligne[i] == -1) {
      cairo_line_to(cr, MG_GAUCHE + i * taille_colonne,
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne) + 1);
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_line_to(cr, MG_GAUCHE + i * taille_colonne,
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_move_to(cr, MG_GAUCHE + ((i)*taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne));
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne));
    } else {
      cairo_line_to(cr, MG_GAUCHE + i * taille_colonne,
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_move_to(cr, MG_GAUCHE + ((i - 1) * taille_colonne) + 2,
                    MG_HAUTE + (hauteur * taille_colonne) + 2);
      cairo_move_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne));
      cairo_line_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne) + taille_colonne);
      cairo_rectangle(cairo_cellules, MG_GAUCHE + i * taille_colonne + 3,
                      MG_HAUTE + (hauteur * taille_colonne + 3),
                      taille_colonne - 6, taille_colonne - 6);
      cairo_fill(cairo_cellules);

      if (vieillissement_bool) {
        sprintf(age_cellule, "%d", ligne[i]);
        cairo_move_to(
            cr, MG_GAUCHE + ((i)*taille_colonne + (taille_colonne * 0.35)),
            MG_HAUTE + (hauteur * taille_colonne) + (taille_colonne * 2 / 3));
        cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
        cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, (taille_colonne / 1.6));
        cairo_show_text(cr, age_cellule);

        cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
      }

      cairo_move_to(cr, MG_GAUCHE + ((i + 1) * taille_colonne),
                    MG_HAUTE + (hauteur * taille_colonne));
    }
  }

  cairo_fill(cairo_cellules);
  cairo_stroke(cr);
  cairo_destroy(cr);
  cairo_destroy(cairo_cellules);

  return;
}

void affiche_trait(int c, int hauteur, float taille_ligne) {
  cairo_t *cr;
  cr = cairo_create(surface);
  float taille_colonne = (float)LARGEUR_GRILLE / (float)c;
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, CAIRO_LINE_WIDTH);

  cairo_move_to(cr, MG_GAUCHE, MG_HAUTE + (hauteur * taille_colonne));

  int i;
  for (i = 0; i < c; ++i) {
    cairo_line_to(cr, MG_GAUCHE + LARGEUR_GRILLE,
                  MG_HAUTE + (hauteur * taille_colonne));
    cairo_move_to(cr, MG_GAUCHE, MG_HAUTE + (hauteur * taille_colonne));
  }
  cairo_stroke(cr);
  cairo_destroy(cr);
}

void affiche_grille(grille g, int temps_evolution, int cyclique_bool,
                    int vieillissement_bool, int temps_oscillation) {
  int i, l = g.nbl, c = g.nbc;
  char str_temps_evolution[255], str_mode_cyclique[255],
      str_mode_vieillissement[255], str_temps_oscillation[255];
  sprintf(str_temps_evolution, "► Temps d'évolution : %d", temps_evolution);
  sprintf(str_mode_cyclique, cyclique_bool ? "► Mode cyclique : Activé"
                                           : "► Mode cyclique : Désactivé");
  sprintf(str_mode_vieillissement, vieillissement_bool
                                       ? "► Vieillissement : Activé"
                                       : "► Vieillissement : Désactivé");

  if (temps_oscillation == -1) {
    sprintf(str_temps_oscillation, "► Oscillation : Non testée");
  } else if (temps_oscillation == 0) {
    sprintf(str_temps_oscillation, "► Oscillation : Non oscillant");
  } else {
    sprintf(str_temps_oscillation, "► Oscillation : %d pas/période",
            temps_oscillation);
  }

  cairo_t *cr;
  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
  cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 40);
  cairo_move_to(cr, 100, 80);
  cairo_show_text(cr, "Jeu de la vie");

  cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
  cairo_set_font_size(cr, 20);
  cairo_move_to(cr, MG_GAUCHE - 500, 150);
  cairo_show_text(cr, str_temps_evolution);
  cairo_move_to(cr, MG_GAUCHE - 500, 190);
  cairo_show_text(cr, str_mode_cyclique);
  cairo_move_to(cr, MG_GAUCHE - 500, 230);
  cairo_show_text(cr, str_mode_vieillissement);
  cairo_move_to(cr, MG_GAUCHE - 500, 270);
  cairo_show_text(cr, str_temps_oscillation);

  cairo_set_font_size(cr, 15);
  cairo_move_to(cr, MG_GAUCHE - 475, 295);
  cairo_show_text(cr, "(veuillez patienter durant le test d'oscillation)");

  cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
  cairo_set_font_size(cr, 20);
  cairo_move_to(cr, MG_GAUCHE - 500, 350);
  cairo_show_text(cr, "● entrée ou clic gauche : Faire évoluer la grille");
  cairo_move_to(cr, MG_GAUCHE - 500, 390);
  cairo_show_text(cr, "● q ou clic droit : Quitter le programme");
  cairo_move_to(cr, MG_GAUCHE - 500, 430);
  cairo_show_text(cr, "● c : Activer/désactiver le mode cyclique");
  cairo_move_to(cr, MG_GAUCHE - 500, 470);
  cairo_show_text(cr, "● v : Activer/désactiver le vieillissement");
  cairo_move_to(cr, MG_GAUCHE - 500, 510);
  cairo_show_text(cr, "● o : Tester si la grille est oscillante");
  cairo_move_to(cr, MG_GAUCHE - 500, 550);
  cairo_show_text(cr, "● n : Charger une nouvelle grille");

  cairo_set_font_size(cr, 15);
  cairo_move_to(cr, MG_GAUCHE - 485, 575);
  cairo_show_text(cr, "(echap: annuler le changement de grille)");

  cairo_destroy(cr);

  int hauteur = 0;
  float taille_colonne = (float)HAUTEUR_GRILLE / (float)g.nbl;
  affiche_trait(c, hauteur, taille_colonne);
  for (i = 0; i < l; ++i) {
    affiche_ligne(c, g.cellules[i], vieillissement_bool, hauteur,
                  taille_colonne);
    affiche_trait(c, ++hauteur, taille_colonne);
  }
  return;
}

void efface_grille() {
  cairo_t *cr;
  cr = cairo_create(surface);
  cairo_set_source_rgb(cr, 0, 0.09019, 0.16862);
  cairo_paint(cr);
  cairo_destroy(cr);
}

void cairo_affiche_texte(char *input, char *erreur) {
  cairo_t *cr;
  cr = cairo_create(surface);
  char input_label[255];
  if (strcmp(erreur, "") == 0) {
    sprintf(input_label, "Indiquez le chemin vers la grille à charger : ");
  } else {
    sprintf(input_label, "%s :", erreur);
  }

  cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
  cairo_rectangle(cr, MG_GAUCHE - 500, 630, 455, 70);
  cairo_fill(cr);

  cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
  cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 20);

  cairo_move_to(cr, MG_GAUCHE - 495, 660);
  cairo_show_text(cr, input_label);

  cairo_move_to(cr, MG_GAUCHE - 495, 680);
  cairo_show_text(cr, input);

  cairo_destroy(cr);
}

void debut_jeu(grille *g, grille *gc) {
  int temps_evolution = 0;
  int vieillissement_bool = 0;
  int refresh_grille = 0;
  int fin_jeu = 0;
  int temps_oscillation = -1;
  int cyclique_bool = 1;
  int (*compte_voisins_vivants)(int, int, grille) =
      compte_voisins_vivants_cyclique;

  XEvent e;

  KeySym key;
  char input_buffer[255];

  while (1) {
    XNextEvent(cairo_xlib_surface_get_display(surface), &e);

    if (e.type == Expose && e.xexpose.count < 1) {
      affiche_grille(*g, temps_evolution, cyclique_bool, vieillissement_bool,
                     temps_oscillation);
    } else if (e.type == KeyPress) {                        // Touche pressée
      if (e.xkey.keycode == 36 || e.xkey.keycode == 104) {  // Touche entrée
        evolue(g, gc, &temps_evolution, compte_voisins_vivants,
               vieillissement_bool);
        refresh_grille = 1;
      } else if (e.xkey.keycode == 57) {  // n
        temps_oscillation = -1;
        cairo_affiche_texte("", "");
        int exit_grid_change = 0;
        int erreur = 0;
        FILE *fichier_grille_test = NULL;

        do {
          char fichier_grille[100] = "";

          while (1) {
            XNextEvent(cairo_xlib_surface_get_display(surface), &e);
            if (e.type == KeyPress &&
                XLookupString(&e.xkey, input_buffer, 255, &key, 0) == 1) {
              if (e.xkey.keycode == 36 || e.xkey.keycode == 104) {
                break;
              } else if (e.xkey.keycode == 22) {  // Effacer caractère
                fichier_grille[strlen(fichier_grille) - 1] = '\0';
              } else if (e.xkey.keycode == 9) {  // Echap
                exit_grid_change = 1;
                break;
              } else {
                strcat(fichier_grille, input_buffer);
              }
              cairo_affiche_texte(fichier_grille, "");
            }
          }

          if (!exit_grid_change) {
            fichier_grille_test = fopen(fichier_grille, "r");
            if (fichier_grille_test != NULL) {
              libere_grille(g);
              libere_grille(gc);
              erreur = init_grille_from_file(fichier_grille, g);
              if (erreur) {
                printf("Fichier de grille introuvable. Essayez à nouveau\n");
                cairo_affiche_texte("",
                                    "Erreur : Fichier de grille introuvable");
              }
              fclose(fichier_grille_test);
              fichier_grille_test = NULL;
            } else {
              erreur = 1;
              cairo_affiche_texte(
                  "", "Fichier de grille introuvable. Essayez à nouveau ");
            }
          }
        } while (erreur && !exit_grid_change);
        if (!exit_grid_change) {
          temps_evolution = 0;
          alloue_grille(g->nbl, g->nbc, gc);
        }
        refresh_grille = 1;

      } else if (e.xkey.keycode == 54) {  // c
        temps_oscillation = -1;
        if (cyclique_bool) {
          cyclique_bool = 0;
          compte_voisins_vivants = &(compte_voisins_vivants_non_cyclique);
        } else {
          cyclique_bool = 1;
          compte_voisins_vivants = &(compte_voisins_vivants_cyclique);
        }
        refresh_grille = 1;
      } else if (e.xkey.keycode == 55) {  // v
        temps_oscillation = -1;
        vieillissement_bool = !vieillissement_bool;
        refresh_grille = 1;
      } else if (e.xkey.keycode == 32) {  // o
        temps_oscillation =
            grille_oscillante(g, compte_voisins_vivants, vieillissement_bool);
        refresh_grille = 1;
      } else if (e.xkey.keycode == 38) {  // q
        fin_jeu = 1;
      }
    } else if (e.type == ButtonPress) {
      if (e.xbutton.button == 1) {  // Clic gauche
        evolue(g, gc, &temps_evolution, compte_voisins_vivants,
               vieillissement_bool);
        refresh_grille = 1;
      } else if (e.xbutton.button == 3) {  // Clic droit
        fin_jeu = 1;
      }
    } else if (e.type == ClientMessage) {
      fin_jeu = 1;
    }
    if (fin_jeu) {
      return;
    }
    if (refresh_grille) {
      efface_grille();
      affiche_grille(*g, temps_evolution, cyclique_bool, vieillissement_bool,
                     temps_oscillation);
      refresh_grille = 0;
    }
  }
  return;
}

#endif
