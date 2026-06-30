#ifndef enemigos_h
#define enemigos_h
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define max_enemigos

typedef struct {
    float x, y;

    float ancho;
    float alto;

    float velocidadx;
    float velocidady;

    int vida;
    int vida_max;
    int dano;

    bool activo;
} enemigo;

void dibujar_enemigo(enemigo* e, float camara);
void crear_enemigo(float x, float y);
void dibujar_enemigos_mapa(float camara);
//bool colision_bala_enemigo(bala* b, enemigo* e);
void revisar_colisione_bala_enemigo();

void movimiento_enemigo(enemigo* e);
void fisicas_enemigo(enemigo* e);
void spawn_enemigos();


#endif
