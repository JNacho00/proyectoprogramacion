#ifndef personaje_h
#define personaje_h
#define gravedad 0.3f
#define salto -11.0f
#include "balas.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include "mapa.h"

typedef struct {
    float x, y;

    float ancho;
    float alto;

    float velocidadx;
    float velocidady;

    int vida;
    int dano;

    int direccionx;
    int direcciony;

    bool en_suelo;
} personaje;

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado);
void saltar(personaje* p);
void fisicas(personaje* p);
void dibujo_personaje(personaje* p, float camara);
void spawn_personaje(personaje* p);
void direccion(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado);
void disparar(personaje* p);
#endif