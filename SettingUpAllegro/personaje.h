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


typedef enum {

    ANIM_IDLE,
    ANIM_CORRER,
    ANIM_SALTAR,
    ANIM_DISPARAR

} tipo_animacion;


typedef struct {
    float x, y;

    float ancho;
    float alto;

    float velocidadx;
    float velocidady;

    int vida;
    int vida_max;
    int invulnerable;
    int dano;
    int municion;

    int direccionx;
    int direcciony;

    tipo_animacion animacion;

    int frame_actual;
    int contador_animacion;

    bool mira_derecha;
    bool en_suelo;
} personaje;

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado);
void saltar(personaje* p);
void fisicas(personaje* p);
void dibujo_personaje(personaje* p, float camara_x, float camara_Y);
void dibujar_barra_vida(personaje* p);
void spawn_personaje(personaje* p);
void direccion(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado);
void disparar(personaje* p);

bool cargar_sprites_personaje(void);
void liberar_sprites_personaje(void);

void recibir_dano_personaje(personaje* p, int dano_recibido);


#endif