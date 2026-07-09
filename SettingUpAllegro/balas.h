#ifndef balas_h
#define balas_h
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define max_balas_p 3
#define velocidad_bala 8.0f
#define alto_bala 20.0f
#define ancho_bala 20.0f


typedef struct {
	float x, y;
    float alto;
    float ancho;
    float velocidad_bx; 
    float velocidad_by;
    bool activa;
} bala;

extern bala balas[max_balas_p];
void dibujar_bala(bala* b, float camara_x, float camara_y);
bool crear_bala(int x, int y, float belocicdadx, float belocidady);
void dibujar_balas_mapa(float camara_x, float camara_y);
void fisicas_balas();
#endif