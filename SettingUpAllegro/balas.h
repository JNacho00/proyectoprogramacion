#ifndef balas_h
#define balas_h
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define max_balas 50
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

extern bala balas[max_balas];
void dibujar_bala(bala* b, float camara);
void crear_bala(int x, int y, float belocicdadx, float belocidady);
//void actualizar_bala();
void dibujar_balas_mapa(float camara);
void fisicas_balas();
#endif