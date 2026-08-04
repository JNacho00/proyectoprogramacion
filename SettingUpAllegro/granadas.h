#ifndef granadas_h
#define granadas_h
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define max_granadas_p 2

typedef struct {
    float x;
    float y;
    float ancho;
    float alto;

    float velocidad_gx;
    float velocidad_gy;

    bool activa;
    bool explotar;
    int rango_explosion;

    int frame_actual;
    int contador_animacion;

} granada;

void dibujar_granada(granada* g, float camara_x, float camara_y);
void spawn_granadas(granada granadas[]);
void dibujar_granadas_mapa(granada granadas[], float camara_x, float camara_y);
void fisicas_granadas(granada granadas[]);

#endif 
