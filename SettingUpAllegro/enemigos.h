#ifndef enemigos_h
#define enemigos_h
#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define max_enemigos 8


typedef enum {

    CAMINAR,
    ATACAR,
  
} tipo_animacion_enemigo;

typedef enum {
    zombie_n,
    zombie_r,
} tipo_enemigo;

typedef struct {
    float x, y;

    float ancho;
    float alto;

    float velocidadx;
    float velocidady;

    int vida;
    int vida_max;
    int dano;
    int cooldown_ataque;

    int direccionx;
    float rango_vision;

    tipo_enemigo tipo;
    tipo_animacion_enemigo animacion;
    int frame_actual;
    int contador_animacion;

    bool atacando;
    bool en_suelo;
    bool persiguiendo;  
    bool activo;

    bala balas[max_balas_p];
} enemigo;

void dibujar_enemigo(enemigo* e, float camara_x, float camara_y);
void inicializar_enemigos(enemigo enemigos[]);
void crear_enemigo(enemigo enemigos[], float x, float y, tipo_enemigo tipo);
void dibujar_enemigos_mapa(enemigo enemigos[], float camara_x, float camara_y);
void revisar_colisiones_bala_enemigo(enemigo enemigos[], bala balas[]);
void fisicas_enemigo(enemigo* e, personaje* p);
void spawn_enemigos(enemigo enemigos[]);
void fisicas_enemigos(enemigo enemigos[], personaje* p);
void actualizar_ataque_enemigo(enemigo* e, personaje* p);
void actualizar_ataques_enemigos(personaje* p, enemigo enemigos[]);
bool cargar_sprites_enemigos(void);
void liberar_sprites_enemigos(void);
bool colision_bala_enemigo(bala* b, enemigo* e);
bool colison_enemigo(enemigo* e, personaje* p);
void cambiar_animacion_enemigo(enemigo* e,tipo_animacion_enemigo nueva_animacion);

void actualizar_animacion_enemigo(enemigo* e);


#endif