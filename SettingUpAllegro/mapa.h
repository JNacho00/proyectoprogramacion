#ifndef mapa.h
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define ancho_v   20
#define largo_v   20
#define columnas 40
#define filas 120

void dibujar_mapa(float camara);

extern char mapa[filas][columnas + 1];
extern int mapa_filas;
extern int mapa_col;
int es_solido(int px, int py);



#endif