#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include "mapa.h"
#define gravedad 0.3f


char mapa[filas][columnas + 1];
int mapa_filas = 0;
int mapa_col = 0;
char bloque;

int fisicas_mapa(int pos_x, int pos_y) {
    int col = pos_x / ancho_v;
    int fila = pos_y / largo_v;
    /*
    if (col < 0 || col >= mapa_cols) {
        return 1;
    }

    if (fila < 0 || fila >= mapa_filas) {
        return 0;
    }
    */
    
    if (col < 0 || col >= mapa_col || fila < 0 || fila >= mapa_filas) { // verifica colisiones 
        return 1;
    }

    bloque = mapa[fila][col];

    return (bloque == 'P');
}

void dibujar_mapa(float camara) {
    int fila;
    int col;
    char bloque;
    for (fila = 0; fila < mapa_filas; fila++) {
        for (col = 0; col < mapa_col; col++) {

            bloque = mapa[fila][col];
            float x = col * ancho_v;
            float y = fila * largo_v - camara;

            switch (bloque) {
            case 'P':
                al_draw_filled_rectangle(x, y, x + ancho_v, y + largo_v,
                    al_map_rgb(139, 90, 43));
                break;
            case 'a':
                al_draw_filled_rectangle(x, y, x + ancho_v, y + largo_v,
                    al_map_rgb(55, 215, 0));
                break;
            }
            

        }
    }
}