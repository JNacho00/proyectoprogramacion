#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
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
char bloque;

int fisicas_mapa(int pos_x, int pos_y) {
    int col = pos_x / ancho_v;
    int fila = pos_y / largo_v;
    char bloque;

    if (col < 0 || col >= mapa_col) {
        return 1; // para solidos return 1 y no s. return 0
    }

    if (fila < 0) {
        return 1;
    }

    if (fila >= mapa_filas) {
        return 0;
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
            case 't':
                al_draw_filled_triangle(x, y, x, y, x, y + ancho_v + largo_v,//////////////////////////////////////////////////////////////////////
                    al_map_rgb(250, 0, 0));
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
            }
            
        }
    }
}

void romper_plataforma(fila) {
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