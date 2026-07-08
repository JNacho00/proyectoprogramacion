#ifndef mapa_h
#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#define ancho_v 20
#define largo_v 20
#define columnas 120
#define filas 40

void dibujar_mapa(float camara_x, float camara_y);
extern char mapa[filas][columnas + 1];
extern int mapa_filas;
extern int mapa_col;
int fisicas_mapa(int pos_x, int pos_y);
void romper_plataforma(fila);
bool cargar_sprites_mapa();
void dibujar_tile_calle(int columna_sprite, int fila_sprite, float x, float y);
void liberar_sprites_mapa();


void dibujar_fondo(float camara_x);



#endif