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

typedef enum {

    CAMINAR,
    ATACAR,
  
} tipo_animacion_enemigo;

typedef struct {
    float x, y;

    float ancho;
    float alto;

    float velocidadx;
    float velocidady;

    int vida;
    int vida_max;
    int dano;

    int direccionx;

    tipo_animacion_enemigo animacion;
    int frame_actual;
    int contador_animacion;

    bool en_suelo;
    bool activo;
} enemigo;

void dibujar_enemigo(enemigo* e, float camara_x, float camara_y);
void crear_enemigo(float x, float y);
void dibujar_enemigos_mapa(float camara_x, float camara_y);
void revisar_colisione_bala_enemigo();
void movimiento_enemigo(enemigo* e);
void fisicas_enemigo(enemigo* e);
void spawn_enemigos();

void fisicas_enemigos();


bool cargar_sprites_enemigos(void);
void liberar_sprites_enemigos(void);

void cambiar_animacion_enemigo(enemigo* e,tipo_animacion_enemigo nueva_animacion);

void actualizar_animacion_enemigo(enemigo* e);


#endif
