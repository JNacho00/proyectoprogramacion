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
#define velocidad_bala 15.0f
#define alto_bala 10.0f
#define ancho_bala 10.0f



typedef struct {
	float x, y;
    float alto;
    float ancho;
    float velocidad_bx; 
    float velocidad_by;

    int frame_actual;
    int contador_animacion;
    bool activa;
} bala;

typedef struct personaje personaje;

void dibujar_bala(bala* b, float camara_x, float camara_y);
bool crear_bala(personaje* p, bala balas[]);
void spawn_balas(bala balas[]);
void dibujar_balas_mapa(bala balas[], float camara_x, float camara_y);
void fisicas_balas(bala balas[]);

bool cargar_sprites_balas(void);
void liberar_sprites_balas(void);
void actualizar_animacion_bala(bala* b);
#endif