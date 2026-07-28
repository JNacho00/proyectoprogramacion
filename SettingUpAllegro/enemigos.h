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
#define max_enemigos 20

typedef enum {

    CAMINAR,
    ATACAR,
  
} tipo_animacion_enemigo;

typedef enum {

    zombie_n,
    zombie_r,
    zombie_d,
    volador,

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
    int puntos;
    int cooldown_ataque;
    int cooldown_disparo;

    int direccionx;
    int direcciony;
    int direccionx_disp;
    int direcciony_disp;
    float rango_vision;
    float y_inicio;
    float dist;
    float x_inicio;

    tipo_enemigo tipo;
    tipo_animacion_enemigo animacion;
    int frame_actual;
    int contador_animacion;

    bool atacando;
    bool en_suelo;
    bool persiguiendo;  
    bool activo;

    bala balas_enemigo[max_balas_p];
} enemigo;

void dibujar_enemigo(enemigo* e, float camara_x, float camara_y);
void inicializar_enemigos(enemigo enemigos[]);
void crear_enemigo(enemigo enemigos[], float x, float y, tipo_enemigo tipo);
void dibujar_enemigos_mapa(enemigo enemigos[], float camara_x, float camara_y);
void revisar_colisiones_bala_enemigo(enemigo enemigos[], bala balas[], personaje* p);
void fisicas_enemigo(enemigo* e, personaje* p);
void spawn_enemigos(enemigo enemigos[]);
void fisicas_enemigos(enemigo enemigos[], personaje* p);
void actualizar_ataque_enemigo(enemigo* e, personaje* p);
void actualizar_ataques_enemigos(personaje* p, enemigo enemigos[]);
bool cargar_sprites_enemigos(void);
void liberar_sprites_enemigos(void);
bool colision_bala_enemigo(bala* b, enemigo* e, personaje* p);
bool colision_enemigo(enemigo* e, personaje* p);
void cambiar_animacion_enemigo(enemigo* e,tipo_animacion_enemigo nueva_animacion);
void actualizar_animacion_enemigo(enemigo* e);
void disparo_enemgio(enemigo* e, bala balas_enemigo[], personaje* p);
bool colision_bala_personaje(bala* b, personaje* p);
void revisar_colison_bala_personaje(bala balas_enemigo[], personaje* p, int dano);
void disparo_zombie_d(enemigo enemigos[], personaje* p);
void disparo_enemigo_v(enemigo* e, bala balas_enemigo[], personaje* p);
void disparo_zombie_v(enemigo enemigos[], personaje* p);

#endif