#ifndef personaje_h
#define personaje_h
#define gravedad 0.3f
#define salto -11.0f
#include "balas.h"
#include "granadas.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "mapa.h"

typedef enum {

    ANIM_IDLE,
    ANIM_CORRER,
    ANIM_SALTAR,
    ANIM_DISPARAR

} tipo_animacion;

struct personaje {
    float x, y;

    float ancho;
    float alto;

    float velocidadx;
    float velocidady;

    int vida;
    int vida_max;
    int escudo;
    int escudo_max;
    int invulnerable;
    int dano;
    int municion;
    int municion_granadas;
    int llave;
    int llaves_nivel;

    int puntaje;

    int direccionx;
    int direcciony;

    tipo_animacion animacion;

    int frame_actual;
    int contador_animacion;

    bool mira_derecha;
    bool en_suelo;
    bool agarro_llave;
    bala balas[max_balas_p];
    granada granadas[max_granadas_p];
};

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado);
void saltar(personaje* p);
void fisicas(personaje* p);
void dibujo_personaje(personaje* p, float camara_x, float camara_Y);
void dibujar_barra_vida(personaje* p);
void spawn_personaje(personaje* p);
void actualizar_animacion_personaje(personaje* p, bool se_mueve);
bool cargar_sprites_personaje(void);
void liberar_sprites_personaje(void);
void sombra_personaje(personaje* p, float camara_x, float camara_y);
void recibir_dano_personaje(personaje* p, int dano_recibido);
void dibujar_puntaje(personaje* p, ALLEGRO_FONT* fuente);
bool disparo_mouse(personaje* p, float mouse_x, float mouse_y, float camara_x, float camara_y, float zoom);

bool disparo_granada_mouse(personaje* p, float mouse_x, float mouse_y, float camara_x, float camara_y, float zoom);


#endif