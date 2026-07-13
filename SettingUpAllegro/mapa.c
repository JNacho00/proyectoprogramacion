#define _CRT_SECURE_NO_WARNINGS
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mapa.h"

ALLEGRO_BITMAP* sprite_calle = NULL;
ALLEGRO_BITMAP* tierra1 = NULL;
ALLEGRO_BITMAP* tierra2 = NULL;
ALLEGRO_BITMAP* tierra3 = NULL;
ALLEGRO_BITMAP* fondo = NULL;

char mapa[filas][columnas + 1];
int mapa_estado[filas][columnas];
int mapa_filas = 0;
int mapa_col = 0;

int fisicas_mapa(int pos_x, int pos_y) {
    int col = pos_x / ancho_v;
    int fila = pos_y / largo_v;
    char bloque;

    if (col < 0 || col >= mapa_col) {
        return 1; // para solidos return 1 y no s. return 0
    }

    if (fila < 0) {
        return 1; //condicion para no subir mas de la fila "40"
    }

    if (fila >= mapa_filas) {
        return 1; // hacer suelo solido debajo de la fila 40
    }

    bloque = mapa[fila][col];

    switch (bloque) {
    case 'P':
        return 1;

    case 'r':
       romper_plataforma(fila);
        return 1;
    case '1':
        return 1;
    case '2':
        return 1;
    case '3':
        return 1;
    case '4':
        return 1;
    case '5':
        return 1;
    case '6':
        return 1;
    case 'A':
        return 1;
    case 'B':
        return 1;
    case 'C':
        return 1;
    }

    return 0;
}


void dibujar_mapa(float camara_x, float camara_y) {
    int fila;
    int col;
    char bloque;

    for (fila = 0; fila < mapa_filas; fila++) {
        for (col = 0; col < mapa_col; col++) {

            bloque = mapa[fila][col];

            float x = col * ancho_v - camara_x;
            float y = fila * largo_v - camara_y;

            switch (bloque) {
            case 'P':
                al_draw_filled_rectangle(x, y, x + ancho_v, y + largo_v,
                    al_map_rgb(139, 90, 43));
                break;
            case 'r':
                al_draw_filled_rectangle(x, y, x + ancho_v, y + largo_v,
                    al_map_rgb(250, 250, 250));
                break;
            case '1':
                dibujar_tile_calle(0, 0, x, y);
                break;

            case '2':
                dibujar_tile_calle(1, 0, x, y);
                break;

            case '3':
                dibujar_tile_calle(0, 1, x, y);
                break;

            case '4':
                dibujar_tile_calle(1, 1, x, y);
                break;

            case '5':
                dibujar_tile_calle(0, 2, x, y);
                break;

            case '6':
                dibujar_tile_calle(1, 2, x, y);
                break;
            case 'A':
                al_draw_bitmap(tierra1, x, y, 0);
                break;

            case 'B':
                al_draw_bitmap(tierra2, x, y, 0);
                break;

            case 'C':
                al_draw_bitmap(tierra3, x, y, 0);
                break;
            case 'Z':
                al_draw_filled_rectangle(
                    x,
                    y,
                    x + ancho_v,
                    y + largo_v,
                    al_map_rgb(0, 200, 255)
                );
                break;
            }
            
        }
    }
}

void romper_plataforma(int fila) {
    int j;

    for (j = 0; j < columnas; j++) {

        if (mapa[fila][j] == 'r') {
            mapa[fila][j] = '.';
        }

    }

}

bool cargar_sprites_mapa() {
    sprite_calle = al_load_bitmap("assets/sprite_calle1.png");

    if (sprite_calle == NULL) {
        printf("No se pudo cargar sprite_calle1.png\n");
        return false;
    }

    tierra1 = al_load_bitmap("assets/tierra1.png");
    tierra2 = al_load_bitmap("assets/tierra2.png");
    tierra3 = al_load_bitmap("assets/tierra3.png");

    if (tierra1 == NULL || tierra2 == NULL || tierra3 == NULL) {
        printf("No se pudo cargar una imagen de tierra\n");
        return false;
    }

    fondo = al_load_bitmap("assets/imagen_fondo2.png");

    if (fondo == NULL) {
        printf("No se pudo cargar fondo.png\n");
        liberar_sprites_mapa();
        return false;
    }

    return true;
}

void dibujar_tile_calle(int columna_sprite, int fila_sprite, float x, float y) {
   
    al_draw_bitmap_region(sprite_calle,columna_sprite * 20,fila_sprite * 20,20,20,x,y,0);

}

void liberar_sprites_mapa() {
    if (sprite_calle != NULL) {
        al_destroy_bitmap(sprite_calle);
        sprite_calle = NULL;
    }

    if (tierra1 != NULL) {
        al_destroy_bitmap(tierra1);
        fondo = NULL;
    }

    if (tierra2 != NULL) {
        al_destroy_bitmap(tierra2);
        fondo = NULL;
    }

    if (tierra3 != NULL) {
        al_destroy_bitmap(tierra3);
        fondo = NULL;
    }

    if (fondo != NULL) {
        al_destroy_bitmap(fondo);
        fondo = NULL;
    }
}


void dibujar_fondo(float camara_x) {
    int ancho_fondo = 1455;
    int mov;
    int x;

    if (fondo == NULL) {
        return;
    }

    mov = (int)(camara_x * 0.06f);

    x = -(mov % ancho_fondo);

    while (x < 1455) {

        al_draw_scaled_bitmap(fondo,0,0,al_get_bitmap_width(fondo),al_get_bitmap_height(fondo),x,0,ancho_fondo,800,0);

        x += ancho_fondo;
    }
}

void mapas(char ruta[], int nivel) {
    char numero[2];

    strcpy(ruta, "mapa");

    numero[0] = '0' + nivel;
    numero[1] = '\0';

    strcat(ruta, numero);
    strcat(ruta, ".txt");
}

int portal(personaje* p) {
    int col_i;
    int col_d;
    int fila_arriba;
    int fila_abajo;

    col_i = (int)(p->x) / ancho_v;
    col_d = (int)(p->x + p->ancho - 1) / ancho_v;

    fila_arriba = (int)(p->y) / largo_v;
    fila_abajo = (int)(p->y + p->alto - 1) / largo_v;

    if (fila_arriba < 0 || fila_abajo >= mapa_filas ||
        col_i < 0 || col_d >= mapa_col) {
        return 0;
    }

    if (mapa[fila_arriba][col_i] == 'Z' ||
        mapa[fila_arriba][col_d] == 'Z' ||
        mapa[fila_abajo][col_i] == 'Z' ||
        mapa[fila_abajo][col_d] == 'Z') {
        return 1;
    }

    return 0;
}
